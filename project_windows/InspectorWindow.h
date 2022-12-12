#ifndef SHADEREDITOR_INSPECTORWINDOW_H
#define SHADEREDITOR_INSPECTORWINDOW_H

#include "InProjectWindow.h"

class InspectorWindow : public InProjectWindow
{
    const char * get_name() override;
    void on_draw() override;
};


#endif
