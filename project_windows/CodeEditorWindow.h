#ifndef SHADEREDITOR_CODEEDITORWINDOW_H
#define SHADEREDITOR_CODEEDITORWINDOW_H

#include "InProjectWindow.h"
#include <string>
#include <vector>

class CodeEditorWindow : public InProjectWindow
{
    const char * get_name() override { return "Shader Editor"; }
    void on_draw() override;
private:
    double offsetY = 0;

    size_t cursorLine = 0;
    size_t cursorChar = 0;

    size_t availableSelected = 0;

    void draw_vertical_scrollbar(const ImVec2 &size, float lineSpace, const ImVec2 &canvas_p0, const ImVec2 &canvas_sz,
                                 const ImVec2 &canvas_p1, ImDrawList *drawList, const std::vector<std::string> &lines);

    void
    key_work(const ImVec2 &size, float lineSpace, const ImVec2 &canvas_sz, std::vector<std::string> &lines,
             const std::vector<std::string> &availableVariants, const size_t replaceWordSize, const size_t replaceWordChar);
};


#endif
