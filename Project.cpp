#include "Project.h"
#include "imgui_internal.h"

#include "project_windows/InspectorWindow.h"
#include "project_windows/PreviewWindow.h"
#include "project_windows/CodeEditorWindow.h"

Project::Project(std::string name)
{
    editable_shader.vertexShaderLines =
            {
                    "#version 330",
                    "",
                    "layout (location = 0) in vec3 aPos;",
                    "layout (location = 1) in vec3 aNorm;",
                    "layout (location = 2) in vec2 aUv;",
                    "",
                    "uniform mat4 model;",
                    "uniform mat4 view;",
                    "uniform mat4 projection;",
                    "",
                    "out vec4 clr;",
                    "",
                    "void main()",
                    "{",
                    "   clr = vec4(abs(aNorm), 1.0);",
                    "   gl_Position = projection * view * model * vec4(aPos, 1.0);",
                    "}"
            };
    editable_shader.fragmentShaderLines =
            {
                    "#version 330",
                    "",
                    "out vec4 FragColor;",
                    "in vec4 clr;",
                    "",
                    "void main()",
                    "{",
                    "   FragColor = clr;",
                    "}"
            };
    firstShow = true;
    this->name = name;
    previewWindow = nullptr;
}

#include <iostream>

void Project::show_windows(ImGuiID dockSpaceID)
{
    if(firstShow)
    {
        //static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

        //ImGui::DockBuilderRemoveNode(dockSpaceID);
        //ImGui::DockBuilderAddNode(dockSpaceID, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
        //ImGui::DockBuilderSetNodeSize(dockSpaceID, ImGui::GetMainViewport()->Size);

        //ImGuiID inspectorDock = ImGui::DockBuilderSplitNode(dockSpaceID, ImGuiDir_Left,
        //    0.25f, nullptr, &dockSpaceID);
        //ImGuiID previewDock = ImGui::DockBuilderSplitNode(dockSpaceID, ImGuiDir_Right,
        //    0.25f, nullptr, &dockSpaceID);

        unsigned int id = 0;

        CodeEditorWindow* codeEditor = new CodeEditorWindow();
        codeEditor->init(this, id);
        codeEditor->draw();
        windows.push_back(codeEditor);
        id++;

        PreviewWindow* preview = new PreviewWindow();
        preview->init(this, id);
        preview->draw();
        //ImGui::DockBuilderDockWindow(("###PrjWindow_" + std::to_string(id)).c_str(), previewDock);
        previewWindow = preview;
        windows.push_back(preview);
        id++;

        InspectorWindow* inspector = new InspectorWindow();
        inspector->init(this, id);
        //ImGui::SetNextWindowDockID(dockSpaceID);
        inspector->draw();
        //ImGui::DockBuilderDockWindow(("###PrjWindow_" + std::to_string(id)).c_str(), inspectorDock);
        windows.push_back(inspector);
        id++;

        //ImGui::DockBuilderFinish(dockSpaceID);

        firstShow = false;
    }
    for(InProjectWindow* wnd : windows)
    {
        wnd->draw();
    }
}

void Project::pre_imgui_render()
{
    if(previewWindow != nullptr) previewWindow->draw_scene();
}

EditableShader& Project::get_shader()
{
    return editable_shader;
}
