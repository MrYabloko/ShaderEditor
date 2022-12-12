#ifndef SHADEREDITOR_CODEANALYZER_H
#define SHADEREDITOR_CODEANALYZER_H

#include <vector>
#include <string>
#include <imgui.h>

#define KeywordColor IM_COL32(255, 140, 0, 255)
#define ErrorColor IM_COL32(255, 0, 0, 255)
#define VarTypeColor IM_COL32(75, 75, 255, 255)
#define VarColor IM_COL32(150, 100, 255, 255)
#define FunctionColor IM_COL32(255, 255, 100, 255)
#define NumberColor IM_COL32(200, 200, 200, 255)
#define WhiteColor IM_COL32(255, 255, 255, 255)
#define PreProcessor IM_COL32(200, 200, 50, 255)
#define CommentColor IM_COL32(0, 255, 0, 255)

struct ColorizedSegment
{
    std::string text;
    ImU32 color;
    bool haveError;
    std::string errorMessage;
    ColorizedSegment(std::string _text, ImU32 _color, bool _haveError = false, std::string _errorMessage = "")
    {
        text = _text;
        color = _color;
        haveError = _haveError;
        errorMessage = _errorMessage;
    }
};

typedef std::vector<ColorizedSegment> ColorizedLine;
typedef std::vector<ColorizedLine> ColorizedText;

ColorizedText analyze(std::vector<std::string> lines, size_t cursorLine, size_t cursorChar, std::vector<std::string>& availableVariants,
                      size_t& replaceWordSize, size_t& replaceWordChar);

#endif
