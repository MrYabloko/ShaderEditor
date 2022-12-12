#ifndef SHADEREDITOR_INPROJECTWINDOW_H
#define SHADEREDITOR_INPROJECTWINDOW_H

#include "imgui.h"
#include <string>

class Project;

class InProjectWindow
{
private:
    unsigned int drawId;
    Project* project;
protected:
    virtual ImGuiWindowFlags get_imgui_flags() { return 0; };
    virtual void on_draw() = 0;
    virtual const char* get_name() = 0;
public:
    void init(Project* prj, unsigned int _id) { this->project = prj; drawId = _id; }
    void draw() {
        ImGui::Begin((std::string(get_name()) + "###PrjWindow_" + std::to_string(drawId)).c_str(), nullptr, get_imgui_flags());
        on_draw(); ImGui::End(); }
    Project* get_project() { return project; }
};

#endif
