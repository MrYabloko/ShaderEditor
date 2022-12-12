#ifndef SHADEREDITOR_PROJECT_H
#define SHADEREDITOR_PROJECT_H

#include "project_windows/InProjectWindow.h"
#include "project_windows/PreviewWindow.h"
#include <string>
#include <vector>
#include "utils/EditableShader.h"

class Project
{
private:
    EditableShader editable_shader;
    PreviewWindow* previewWindow;
    bool firstShow;
    std::string name;
    std::vector<InProjectWindow*> windows;
public:
    EditableShader& get_shader();
    Project(std::string name);
    void show_windows(ImGuiID dockSpaceID);
    const char* get_name() { return name.c_str(); }
    void pre_imgui_render();
};


#endif
