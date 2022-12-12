#include <algorithm>
#include <iostream>
#include "StringHandler.h"

StringHandler::StringHandler(std::vector<std::string> _text, ColorizedText& _colorizedText)
    : colorizedText(_colorizedText)
{
    text = _text;
}

void StringHandler::remove_last_segment()
{
    if(colorizedText.empty()) return;

    if(current_line.empty())
    {
        cursorLine--;
        current_line = colorizedText[colorizedText.size() - 1];
        colorizedText.pop_back();
        size_t newCursorChar = 0;
        for(ColorizedSegment seg : current_line)
        {
            newCursorChar += seg.text.size();
        }
        cursorChar = newCursorChar;
    }
    cursorChar -= current_line[current_line.size() - 1].text.size();
    current_line.pop_back();
    finished = false;
}

void StringHandler::remove_multiple_segments(size_t count)
{
    for(size_t i = 0; i < count;i++)
    {
        remove_last_segment();
    }
}

void StringHandler::set_cursor_char(size_t c)
{
    cursorChar = c;
    if(cursorChar >= text[cursorLine].size()) new_line();
}

ColorizedLine& StringHandler::get_line()
{
    return current_line;
}
bool StringHandler::new_line()
{
    cursorChar = 0;
    cursorLine++;
    colorizedText.push_back(current_line);
    current_line = std::vector<ColorizedSegment>();
    bool res = cursorLine < text.size();
    if(!res)
    {
        finished = true;
    }
    return res;
}
std::string StringHandler::from_to(const std::string &str, size_t from, size_t to)
{
    if(from > to) return std::string("");
    return str.substr(from, to - from + 1);
}

void StringHandler::process_current_char(ImU32 color)
{
    if(is_finished()) return;
    std::string line = text[cursorLine];
    ColorizedLine& colorLine = get_line();
    char ch = get_current_char();
    colorLine.push_back(ColorizedSegment(std::string("") + ch,color));
    set_cursor_char(cursorChar + 1);
}

std::string StringHandler::get_word(const std::string &stopCharsAfterWord)
{
    if(is_finished()) return "";
    size_t segments = process_upto_words(stopCharsAfterWord);
    size_t pos = std::string::npos;
    std::string line = text[cursorLine];
    for(char check : stopCharsAfterWord)
    {
        size_t aPos = line.find_first_of(check, cursorChar);
        pos = pos > aPos ? aPos : pos;
    }
    std::string res;
    if(pos != std::string::npos) res = from_to(line, cursorChar, pos - 1);
    else res = line.substr(cursorChar);
    remove_multiple_segments(segments);
    return res;
}

size_t StringHandler::process_word(ImU32 color, const std::string& stopCharsAfterWord, bool haveErrorMessage, std::string errorMessage)
{
    if(is_finished()) return 0;
    ColorizedLine& colorLine = get_line();
    size_t count = process_upto_words(stopCharsAfterWord);
    std::string word = get_word(stopCharsAfterWord);
    colorLine.push_back(ColorizedSegment(word, color, haveErrorMessage, errorMessage));
    set_cursor_char(cursorChar + word.size());
    return count + 1;
}

void StringHandler::process_entire_line(ImU32 color)
{
    if(is_finished()) return;
    std::string line = text[cursorLine];
    ColorizedLine& colorLine = get_line();
    std::string str = line.substr(cursorChar);
    colorLine.push_back(ColorizedSegment(str, color));
    new_line();
}

void StringHandler::process_upto_char(char ch, ImU32 color)
{
    start:
    if(is_finished()) return;
    std::string line = text[cursorLine];
    ColorizedLine& colorLine = get_line();
    size_t pos = line.find_first_of(ch, cursorChar);
    if(pos == std::string::npos)
    {
        std::string str = line.substr(cursorChar);
        colorLine.push_back(ColorizedSegment(str, color));
        if(new_line()) goto start;
        goto start;
    }
    std::string str = from_to(line, cursorChar, pos - 1);
    if(str.size() > 0) colorLine.push_back( ColorizedSegment(str, color) );
    set_cursor_char(pos);
}

size_t StringHandler::process_upto_words(const std::string &spaceChars)
{
    size_t segments_added = 0;
    start:
    if(is_finished()) return segments_added;
    std::string line = text[cursorLine];
    ColorizedLine& colorLine = get_line();
    size_t pos = line.find_first_not_of(spaceChars, cursorChar);
    if(pos == std::string::npos)
    {
        std::string str = line.substr(cursorChar);
        colorLine.push_back( ColorizedSegment(str, IM_COL32(0,0,0,255)) );
        segments_added++;
        if(new_line()) goto start;
        return segments_added;
    }
    std::string str = from_to(line, cursorChar, pos - 1);
    if(str.size() > 0)
    {
        colorLine.push_back( ColorizedSegment(str, IM_COL32(0,0,0,255)) );
        segments_added++;
    }
    set_cursor_char(pos);
    return segments_added;
}

bool StringHandler::check_word(std::string check)
{
    std::string line = text[cursorLine];
    return line.substr(cursorChar, check.size()) == check;
}