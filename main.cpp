#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <vector>
#include "utils/global_vars.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Project.h"

Project* opened_project = nullptr;

void open_project(Project* prj)
{
    if(opened_project != nullptr) delete opened_project;
    opened_project = prj;
}

void draw_imgui(GLFWwindow* window)
{
    static bool isNewProjectWindowOpened = false;
    float menuBarHeight = 0;
    if (ImGui::BeginMainMenuBar())
    {
        menuBarHeight = ImGui::GetWindowHeight();
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {
                isNewProjectWindowOpened = true;
            }
            if (ImGui::MenuItem("Open"))
            {

            }
            if (ImGui::BeginMenu("Open Recent"))
            {
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if(isNewProjectWindowOpened)
    {
        ImGui::Begin("New project", &isNewProjectWindowOpened, ImGuiWindowFlags_AlwaysAutoResize);
        static char project_name[128] = "New project";
        ImGui::InputText("Project name", project_name, IM_ARRAYSIZE(project_name));

        if(ImGui::Button("Create"))
        {
            open_project(new Project(project_name));
            isNewProjectWindowOpened = false;
        }

        ImGui::End();
    }

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    int xpos, ypos;
    glfwGetWindowPos(window, &xpos, &ypos);
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - menuBarHeight));
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + menuBarHeight));

    // ImGui::Begin("###MainWindow", NULL,
    //             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove
    //             | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

     //ImGui::End();

    //ImGuiID dockID = ImGui::GetID("###MainWindow");
    ImGuiID dockID = ImGui::DockSpaceOverViewport();

    if(opened_project != nullptr) opened_project->show_windows(dockID);

  //  ImGui::End();
}

int main() {

#pragma region OpenGlInit
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow*& window = get_main_window();
    window = glfwCreateWindow(800, 600, "Shader Editor", nullptr, nullptr);
    if(window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

#pragma endregion

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if(opened_project != nullptr) opened_project->pre_imgui_render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        draw_imgui(window);

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
