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
        unsigned int id = 0;

        ImGui::SetNextWindowDockID(0);
        CodeEditorWindow* codeEditor = new CodeEditorWindow();
        codeEditor->init(this, id);
        codeEditor->draw();
        windows.push_back(codeEditor);
        id++;

        ImGui::SetNextWindowDockID(0);
        PreviewWindow* preview = new PreviewWindow();
        preview->init(this, id);
        preview->draw();
        previewWindow = preview;
        windows.push_back(preview);
        id++;

        ImGui::SetNextWindowDockID(0);
        InspectorWindow* inspector = new InspectorWindow();
        inspector->init(this, id);
        inspector->draw();
        windows.push_back(inspector);
        id++;

        for (ImGuiWindow* wnd : ImGui::DockBuilderGetNode(dockSpaceID)->Windows)
        {
            ImGui::SetWindowDock(wnd, 0, 0);
        }

        //firstShow = false; return;

        ImGuiID dockRightOfInspector = dockSpaceID;
        ImGuiID dockInspector = ImGui::DockBuilderSplitNode(dockRightOfInspector, ImGuiDir_Left, 0.25f, nullptr, &dockRightOfInspector);
        ImGuiID dockCodeEditor = ImGui::DockBuilderSplitNode(dockRightOfInspector, ImGuiDir_Left, 0.5f, nullptr, &dockRightOfInspector);

        ImGui::DockBuilderDockWindow(inspector->get_id().c_str(), dockInspector);
        ImGui::DockBuilderDockWindow(codeEditor->get_id().c_str(), dockCodeEditor);
        ImGui::DockBuilderDockWindow(preview->get_id().c_str(), dockRightOfInspector);

        ImGui::DockBuilderFinish(dockRightOfInspector);

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
