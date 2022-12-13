#ifndef SHADEREDITOR_PREVIEWWINDOW_H
#define SHADEREDITOR_PREVIEWWINDOW_H

#include "InProjectWindow.h"
#include "../resource_manager/RenderTexture.h"
#include "../resource_manager/Mesh.h"
#include "../resource_manager/Shader.h"
#include "../utils/Camera.h"

struct CameraSettings
{
    bool isFirstPerson;
    float fov;
    float near;
    float far;

    float fpSpeed;
};

class PreviewWindow : public InProjectWindow
{
    float renderSizeX, renderSizeY;
    void on_draw() override;
    const char * get_name() override;
    ImGuiWindowFlags get_imgui_flags() override;

    RenderTexture* rt;
    Mesh mesh;
    Camera cam;

    CameraSettings cameraSettings;

    float orbitRotX;
    float orbitRotY;
    float orbitLength;

    float fpRotX;
    float fpRotY;
    float fpPosX;
    float fpPosY;
    float fpPosZ;

    bool cameraSettingOpened = false;
public:
    PreviewWindow();
    ~PreviewWindow();
    void draw_scene();
};


#endif
