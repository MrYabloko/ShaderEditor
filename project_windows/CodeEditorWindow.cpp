#include <iostream>
#include "CodeEditorWindow.h"
#include "imgui_internal.h"

#include "../Project.h"
#include "../code_analyzer/CodeAnalyzer.h"

#include <cmath>

struct Keyword
{
    std::string keyword;
    ImU32 color;
};

void CodeEditorWindow::on_draw()
{
    ImFont* font = ImGui::GetFont();
    float fontSize = 15;
    ImVec2 size = font->CalcTextSizeA(fontSize, 150, -1.0f, "9999");
    float lineSpace = 2.5f;

    ImGui::BeginTabBar("#VertexAndFragmentTabs");

    std::vector<std::string>* lns = nullptr;

    if(ImGui::BeginTabItem("Vertex"))
    {
        lns = &get_project()->get_shader().vertexShaderLines;
        ImGui::EndTabItem();
    }
    if(ImGui::BeginTabItem("Fragment"))
    {
        lns = &get_project()->get_shader().fragmentShaderLines;
        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();

    if(ImGui::Button("Compile"))
    {
        get_project()->get_shader().recompile();
    }

    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

    ImGui::PushClipRect(canvas_p0, canvas_p1, false);

    ImVec2 editor_p0 = ImVec2(canvas_p0.x + size.x, canvas_p0.y);

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    drawList->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(25, 25, 25, 255), 5);

    if(lns == nullptr) return;
    std::vector<std::string>& lines = *lns;

    float numberOffset = 5;

    drawList->AddRectFilled(canvas_p0, ImVec2(editor_p0.x + numberOffset * 2, canvas_p1.y), IM_COL32(50, 50, 50, 255), 0);

    drawList->AddLine(ImVec2(editor_p0.x + numberOffset * 2, canvas_p0.y - 1),
                      ImVec2(editor_p0.x + numberOffset * 2, canvas_p1.y + 1),
                      IM_COL32(100, 100, 100, 255), 2.0f);

    float editorTextOffset = 5;

    std::vector<std::string> availableVariants { };
    size_t replaceWordSize = 0;
    size_t replaceWordChar = 0;
    ColorizedText text = analyze(lines, cursorLine, cursorChar, availableVariants, replaceWordSize, replaceWordChar);

    float errorX0;
    float errorX1;
    float errorY0;
    std::string error = "";

    size_t i = 0;
    for(ColorizedLine line : text)
    {
        float y = canvas_p0.y + (size.y + lineSpace) * i - offsetY;
        drawList->AddText(font, fontSize,
                          ImVec2(canvas_p0.x + numberOffset, y),
                          IM_COL32(150, 150, 150, 150), std::to_string(i + 1).c_str());

        float xOffset = 0;
        size_t charsProcessed = 0;
        for(const ColorizedSegment& segment : line)
        {
            float segmentSize = font->CalcTextSizeA(fontSize, 100000000.0f, -1, segment.text.c_str()).x;
            drawList->AddText(font, fontSize,
                              ImVec2(editor_p0.x + numberOffset * 2 + 1 + editorTextOffset + xOffset, y),
                              segment.color, segment.text.c_str());
            if(segment.haveError)
                //&& cursorLine == i
                //&& charsProcessed < cursorChar && cursorChar < (charsProcessed + segment.text.size()))
            {
                float lineX1 = editor_p0.x + numberOffset * 2 + 1 + editorTextOffset + xOffset;
                float lineX2 = editor_p0.x + numberOffset * 2 + 1 + editorTextOffset + xOffset + segmentSize;
                float lineY = canvas_p0.y + (size.y + lineSpace) * (i + 1) - offsetY;

                if(cursorLine == i && charsProcessed <= cursorChar && cursorChar <= (charsProcessed + segment.text.size()))
                {
                    error = segment.errorMessage;
                    errorX0 = lineX1;
                    errorX1 = lineX2;
                    errorY0 = lineY + 2;
                }

                drawList->AddLine(ImVec2(lineX1, lineY), ImVec2(lineX2, lineY), ErrorColor, 2);
            }
            xOffset += segmentSize;
            charsProcessed += segment.text.size();
        }
        i++;
    }
    if(ImGui::IsWindowFocused())
    {
        std::string line1 = lines[cursorLine];
        std::string cursorOffset = line1.substr(0, cursorChar);
        ImVec2 cursorOffsetSize = font->CalcTextSizeA(fontSize, 1000000, -1.0f, cursorOffset.c_str());
        float cursorX = editor_p0.x + numberOffset * 2 + 1 + editorTextOffset + cursorOffsetSize.x;
        float cursorY0 = canvas_p0.y + (size.y + lineSpace) * cursorLine - offsetY;
        float cursorY1 = canvas_p0.y + (size.y + lineSpace) * (cursorLine + 1) - offsetY;

        drawList->AddLine(ImVec2(cursorX, cursorY0), ImVec2(cursorX, cursorY1), IM_COL32(255, 255, 255, 255));
    }

    if((size.y + lineSpace) * lines.size() > canvas_sz.y)
    {
        draw_vertical_scrollbar(size, lineSpace, canvas_p0, canvas_sz, canvas_p1, drawList, lines);
    }
    if(offsetY < 0) offsetY = 0;

    ImGui::PopClipRect();

    key_work(size, lineSpace, canvas_sz, lines, availableVariants, replaceWordSize, replaceWordChar);

    ImDrawList* foregroundDrawList = ImGui::GetForegroundDrawList();
    if(error != "")
    {
        float rectSize = 500;
        float offsetX = 10;
        float offsetY = 5;
        ImVec2 size = font->CalcTextSizeA(fontSize, rectSize, rectSize, error.c_str());
        float middlePosX = (errorX0 + errorX1) / 2.0f;
        ImVec2 rectP0 = ImVec2( middlePosX - size.x / 2.0f - offsetX, errorY0 );
        ImVec2 rectP1 = ImVec2( middlePosX + size.x / 2.0f + offsetX, errorY0 +  size.y + offsetY * 2 );
        foregroundDrawList->AddRectFilled(rectP0, rectP1, IM_COL32(65, 65, 65, 255), 5);
        foregroundDrawList->AddRect(rectP0, rectP1, IM_COL32(255, 255, 255, 255), 5, 0, 0.5f);

        foregroundDrawList->AddText(font, fontSize, ImVec2( middlePosX - size.x / 2.0f, errorY0 + offsetY ),
                                    IM_COL32(255, 255, 255, 255), error.c_str(), NULL, rectSize);
    }
    if(!availableVariants.empty() && ImGui::IsWindowFocused())
    {
        std::string line1 = lines[cursorLine];
        std::string cursorOffset = line1.substr(0, cursorChar);
        ImVec2 cursorOffsetSize = font->CalcTextSizeA(fontSize, 1000000, -1.0f, cursorOffset.c_str());
        float cursorX = editor_p0.x + numberOffset * 2 + 1 + editorTextOffset + cursorOffsetSize.x;
        float cursorY = canvas_p0.y + (size.y + lineSpace) * (cursorLine + 1) - offsetY;
        ImVec2 rectP0 = ImVec2(cursorX, cursorY);
        const float sizeX = 100;
        const float sizeY = 50;
        ImVec2 rectP1 = ImVec2(cursorX + sizeX, cursorY + sizeY);
        foregroundDrawList->PushClipRect(rectP0, rectP1, false);
        foregroundDrawList->AddRectFilled(rectP0, rectP1, IM_COL32(65, 65, 65, 255), 5);
        foregroundDrawList->AddRect(rectP0, rectP1, IM_COL32(255, 255, 255, 255), 5, 0, 0.5f);

        availableSelected = availableSelected >= availableVariants.size() ? availableVariants.size() - 1 : availableSelected;

        float yOffset = 0;
        if((size.y + lineSpace) * (availableSelected) > sizeY)
        {
            yOffset = (size.y + lineSpace) * (availableSelected + 1) - sizeY;
        }

        for(size_t k = 0; k < availableVariants.size();k++)
        {
            float y =rectP0.y + (size.y + lineSpace) * k - yOffset;
            float y2 =rectP0.y + (size.y + lineSpace) * (k + 1) - yOffset;

            foregroundDrawList->AddText(font, fontSize, ImVec2(rectP0.x + 5, y),
                                        WhiteColor, availableVariants[k].c_str());

            if(k == availableSelected)
            {
                foregroundDrawList->AddRectFilled(ImVec2(rectP0.x, y), ImVec2(rectP1.x, y2), IM_COL32(0, 0, 0, 75), 5);
            }

            foregroundDrawList->AddLine(ImVec2(rectP0.x, y), ImVec2(rectP1.x, y), IM_COL32(125, 125, 125, 255), 0.5f);
        }

        foregroundDrawList->PopClipRect();
    }

    //drawList->AddRectFilled(window_pos, ImVec2(window_pos.x + 50, window_pos.y + 50), IM_COL32(255, 0, 0, 255), 5);
}

void CodeEditorWindow::key_work(const ImVec2 &size, float lineSpace, const ImVec2 &canvas_sz, std::vector<std::string> &lines,
                           const std::vector<std::string> &availableVariants, const size_t replaceWordSize,
                           const size_t replaceWordChar) {
    if(ImGui::IsWindowFocused())
    {
        bool isChanged = false;
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
        {
            if(availableVariants.empty())
            {
                isChanged = true;
                if (cursorLine > 0)
                    cursorLine--;
            }
            else if(availableSelected > 0)
            {
                availableSelected--;
            }
        }
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
        {
            if(availableVariants.empty())
            {
                isChanged = true;
                if (cursorLine < lines.size() - 1)
                    cursorLine++;
            }
            else
            {
                availableSelected++;
            }
        }
        if (cursorChar > lines[cursorLine].size())
            cursorChar = lines[cursorLine].size();
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow))
        {
            if (cursorChar < lines[cursorLine].size())
                cursorChar++;
            isChanged = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow))
        {
            if (cursorChar > 0)
                cursorChar--;
            isChanged = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Tab))
        {
            std::string &line = lines[cursorLine];
            line.insert(cursorChar, "   ");
            cursorChar += 3;
            isChanged = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Backspace))
        {
            if (cursorChar > 0)
            {
                std::string &line = lines[cursorLine];
                line.erase(cursorChar - 1, 1);
                cursorChar--;
                isChanged = true;
            } else if (cursorLine > 0)
            {
                std::string line = lines[cursorLine];
                lines.erase(lines.cbegin() + cursorLine);
                cursorLine--;
                cursorChar = lines[cursorLine].size();
                lines[cursorLine] += line;
                isChanged = true;
            }
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Enter))
        {
            if(availableVariants.empty())
            {
                std::string lineToMove = lines[cursorLine].substr(cursorChar);
                lines[cursorLine].erase(cursorChar);
                lines.insert(lines.cbegin() + cursorLine + 1, lineToMove);
                cursorChar = 0;
                cursorLine++;
            }
            else
            {
                lines[cursorLine].erase(replaceWordChar, replaceWordSize);
                availableSelected = availableSelected >= availableVariants.size() ? availableVariants.size() - 1 : availableSelected;
                lines[cursorLine].insert(replaceWordChar, availableVariants[availableSelected] + " ");
                cursorChar = replaceWordChar + availableVariants[availableSelected].size();
            }
            isChanged = true;
        }
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        if(!io.InputQueueCharacters.empty())
        {
            char chr = io.InputQueueCharacters[0];
            lines[cursorLine].insert(cursorChar, {chr });
            cursorChar++;
            io.InputQueueCharacters.clear();
            isChanged = true;
        }
        if(isChanged)
        {
            if((size.y + lineSpace) * (cursorLine + 1) > canvas_sz.y + offsetY)
            {
                offsetY = (size.y + lineSpace) * (cursorLine + 1) - canvas_sz.y;
            }
            else if((size.y + lineSpace) * ((cursorLine > 3 ? cursorLine : 3) - 3) < offsetY)
            {
                offsetY = (size.y + lineSpace) * (cursorLine);
            }
        }
    }
}

void CodeEditorWindow::draw_vertical_scrollbar(const ImVec2 &size, float lineSpace, const ImVec2 &canvas_p0,
                                               const ImVec2 &canvas_sz, const ImVec2 &canvas_p1, ImDrawList *drawList,
                                               const std::vector<std::string> &lines) {
    float scrollBarBGXSize = 15;
    ImVec2 scrollBarBGP0 = ImVec2(canvas_p1.x - scrollBarBGXSize, canvas_p0.y);

    drawList->AddRectFilled(scrollBarBGP0, canvas_p1, IM_COL32(10, 10, 10, 255));
    double scrollBarYSize = canvas_sz.y / ((size.y + lineSpace) * lines.size()) * (canvas_p1.y - scrollBarBGP0.y);
    double t = offsetY / ((size.y + lineSpace) * lines.size() - canvas_sz.y);
    double a = canvas_p0.y + scrollBarYSize / 2;
    double b = canvas_p1.y - scrollBarYSize / 2;
    double scrollBarYCenter = a + t * (b - a);
    ImVec2 scrollBarP0 = ImVec2(scrollBarBGP0.x + 2, scrollBarYCenter - scrollBarYSize / 2);
    ImVec2 scrollBarP1 = ImVec2(canvas_p1.x - 2, scrollBarYCenter + scrollBarYSize / 2);
    drawList->AddRectFilled(scrollBarP0, scrollBarP1, IM_COL32(50, 50, 50, 255));

    ImVec2 mp = ImGui::GetMousePos();
    static bool isMovingScrollBar = false;
    static double mouseOffsetFromTop = 0;
    if(ImGui::IsWindowFocused())
    {
        offsetY -= ImGui::GetIO().MouseWheel * 25;

        if(scrollBarP0.x < mp.x && mp.x < scrollBarP1.x
            && scrollBarP0.y < mp.y && mp.y < scrollBarP1.y && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            isMovingScrollBar = true;
            mouseOffsetFromTop = mp.y - scrollBarP0.y;
            std::cout << mouseOffsetFromTop << std::endl;
        }
    }
    if(!ImGui::IsMouseDown(ImGuiMouseButton_Left)) isMovingScrollBar = false;
    if(isMovingScrollBar)
    {
        double fromLow = canvas_p0.y;
        double fromHigh = canvas_p1.y;
        double toLow = 0;
        double toHigh = 1;
        double mapped = ((mp.y - mouseOffsetFromTop) - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
        offsetY = mapped * (size.y + lineSpace) * lines.size();
    }

    if(offsetY + canvas_sz.y > (size.y + lineSpace) * lines.size()) offsetY = (size.y + lineSpace) * lines.size() - canvas_sz.y;
}
