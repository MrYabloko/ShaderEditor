#ifndef SHADEREDITOR_STRINGHANDLER_H
#define SHADEREDITOR_STRINGHANDLER_H

#include "CodeAnalyzer.h"

#include <vector>
#include <string>

class StringHandler
{
private:
    ColorizedText& colorizedText;
    std::vector<std::string> text;

    size_t cursorLine = 0;
    size_t cursorChar = 0;

    bool finished = false;

    ColorizedLine current_line;

    ColorizedLine& get_line();
    bool new_line();
    std::string from_to(const std::string &str, size_t from, size_t to);
    void set_cursor_char(size_t c);
public:
    void get_cursor(size_t& line, size_t& ch) { line = cursorLine; ch = cursorChar; };
    StringHandler(std::vector<std::string> _text, ColorizedText& _colorizedText);
    std::string get_word(const std::string &stopCharsAfterWord = {' '});
    char get_current_char() { if(is_finished()) return ' '; return text[cursorLine][cursorChar]; }
    void process_current_char(ImU32 color);
    bool is_finished() const { return finished; }
    size_t process_word(ImU32 color, const std::string& stopCharsAfterWord = {' '},  bool haveErrorMessage = false, std::string errorMessage = "");
    size_t process_upto_words(const std::string &spaceChars);
    void process_upto_char(char ch, ImU32 color);
    void process_entire_line(ImU32 color);
    void remove_last_segment();
    void remove_multiple_segments(size_t count);
    bool check_word(std::string check);
};


#endif
