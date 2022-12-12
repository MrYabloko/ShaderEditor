#include <iostream>
#include "PreviewWindow.h"
#include "../utils/primitives.h"
#include "../utils/global_vars.h"

#include "../Project.h"

const char* PreviewWindow::get_name()
{
    return "Preview";
}

ImGuiWindowFlags PreviewWindow::get_imgui_flags()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;
    return window_flags;
}

void PreviewWindow::on_draw()
{
    if(ImGui::BeginMenuBar())
    {
        if(ImGui::BeginMenu("Camera"))
        {
            if (ImGui::MenuItem("Settings"))
            {
                cameraSettingOpened = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if(cameraSettingOpened && ImGui::Begin("Camera Settings", &cameraSettingOpened,
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking))
    {
        ImGui::DragFloat("Near Clip", &cameraSettings.near, 1.0f, 0.01f, cameraSettings.far);
        ImGui::DragFloat("Far Clip", &cameraSettings.far, 1.0f, cameraSettings.near, 10000);
        ImGui::DragFloat("Field Of View", &cameraSettings.fov, 1.0f, 10.0f, 180.0f);

        ImGui::Spacing();

        if (ImGui::BeginCombo("Camera Type", cameraSettings.isFirstPerson ? "First Person" : "Orbit"))
        {
            bool preSelect = cameraSettings.isFirstPerson;

            if(ImGui::Selectable("Orbit", !cameraSettings.isFirstPerson)) cameraSettings.isFirstPerson = false;
            if(!preSelect) ImGui::SetItemDefaultFocus();

            if(ImGui::Selectable("First Person", cameraSettings.isFirstPerson)) cameraSettings.isFirstPerson = true;
            if(preSelect) ImGui::SetItemDefaultFocus();

            ImGui::EndCombo();
        }

        if(cameraSettings.isFirstPerson)
        {
            float speed = cameraSettings.fpSpeed * 100;
            ImGui::DragFloat("Speed", &speed);
            cameraSettings.fpSpeed = speed / 100;
        }

        ImGui::End();
    }

    ImVec2 size = ImGui::GetContentRegionAvail();
    renderSizeX = size.x;
    renderSizeY = size.y;
    ImGui::Image((void*)(intptr_t)rt->getRenderedTextureID(), size);

    static double lastX, lastY;
    static bool firstClick = true;

    if(ImGui::IsMouseDown(ImGuiMouseButton_Right) && (!firstClick || ImGui::IsItemHovered()))
    {
        glfwSetInputMode(get_main_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        double xpos, ypos;
        glfwGetCursorPos(get_main_window(), &xpos, &ypos);

        if(firstClick)
        {
            ImGui::SetWindowFocus();
            lastX = xpos;
            lastY = ypos;
            firstClick = false;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        double sensitivity = 1;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        if(!cameraSettings.isFirstPerson)
        {
            orbitRotY += xoffset;
            orbitRotX -= yoffset;

            orbitRotX = orbitRotX < 90 ? (orbitRotX > -90 ? orbitRotX : -90) : 90;
        }
        else
        {
            fpRotY += xoffset;
            fpRotX -= yoffset;

            fpRotX = fpRotX < 90 ? (fpRotX > -90 ? fpRotX : -90) : 90;

            glm::vec3 addPos = glm::vec3(0,0,0);

            glm::vec3 front = cam.get_front();
            glm::vec3 right = cam.get_right();
            glm::vec3 up = glm::vec3(0,1,0);

            if(ImGui::IsKeyDown(ImGuiKey_W)) addPos += front;
            if(ImGui::IsKeyDown(ImGuiKey_S)) addPos -= front;

            if(ImGui::IsKeyDown(ImGuiKey_A)) addPos += right;
            if(ImGui::IsKeyDown(ImGuiKey_D)) addPos -= right;

            if(ImGui::IsKeyDown(ImGuiKey_Space)) addPos -= up;
            if(ImGui::IsKeyDown(ImGuiKey_LeftShift)) addPos += up;

            addPos *= cameraSettings.fpSpeed;

            fpPosX += addPos.x;
            fpPosY += addPos.y;
            fpPosZ += addPos.z;
        }
    }
    else
    {
        firstClick = true;
        glfwSetInputMode(get_main_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

PreviewWindow::PreviewWindow()
    : mesh( {}, {} )
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    generate_cube(vertices, indices);
    mesh = Mesh(vertices, indices);

    orbitLength = 5;

    renderSizeX = 10;
    renderSizeY = 10;
    rt = new RenderTexture();
    cam.yaw = 260;
    cam.pitch = 0;

    fpPosX = 0;
    fpPosY = 0;
    fpPosZ = 0;

    fpRotX = 0;
    fpRotY = 0;
    orbitRotX = 0;
    orbitRotY = 0;

    cameraSettings.isFirstPerson = false;

    cameraSettings.fov = 90;
    cameraSettings.near = 0.01f;
    cameraSettings.far = 100;

    cameraSettings.fpSpeed = 0.1f;
}

PreviewWindow::~PreviewWindow()
{
    delete rt;
}

void PreviewWindow::draw_scene()
{
    rt->startRender(renderSizeX, renderSizeY);
    glClearColor(0.5f,0.5f,0.5f,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(!cameraSettings.isFirstPerson)
    {
        glm::vec3 direction;
        direction.x = cos(glm::radians(orbitRotY)) * cos(glm::radians(orbitRotX));
        direction.y = sin(glm::radians(orbitRotX));
        direction.z = sin(glm::radians(orbitRotY)) * cos(glm::radians(orbitRotX));
        direction = glm::normalize(direction);

        cam.yaw = orbitRotY;
        cam.pitch = orbitRotX;

        cam.position = -direction * orbitLength;
    }
    else
    {
        cam.yaw = fpRotY;
        cam.pitch = fpRotX;

        cam.position.x = fpPosX;
        cam.position.y = fpPosY;
        cam.position.z = fpPosZ;
    }

    cameraSettings.fov = cameraSettings.fov < 180.0f ? (cameraSettings.fov > 10.0f ? cameraSettings.fov : 10.0f) : 180.0f;
    cameraSettings.near = cameraSettings.near < cameraSettings.far ? (cameraSettings.near > 0.01f ? cameraSettings.near : 0.01f) : cameraSettings.far;
    cameraSettings.far = cameraSettings.far < 10000 ? (cameraSettings.far > cameraSettings.near ? cameraSettings.far : cameraSettings.near) : 10000;

    cam.fov = cameraSettings.fov;
    cam.near = cameraSettings.near;
    cam.far = cameraSettings.far;

    EditableShader& edit_shd = get_project()->get_shader();

    if(!edit_shd.isShaderNull())
    {
        Shader* shd = edit_shd.get_shader();
        glm::mat4 model = glm::mat4(1.0f);
        shd->use();
        shd->setMatrix4("view", cam.get_view_matrix());
        shd->setMatrix4("model", model);
        shd->setMatrix4("projection", cam.get_projection_matrix((float) renderSizeX / (float) renderSizeY));
        mesh.draw();
    }
    rt->endRender();
}
