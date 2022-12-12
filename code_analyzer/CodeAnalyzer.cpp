#include <algorithm>
#include <utility>
#include "CodeAnalyzer.h"
#include "iostream"

#include "StringHandler.h"

//#define CheckSubstr(str, line, chCount) (chCount + std::string(str).size()) <= line.size() \
//&& line.substr(chCount, std::string(str).size()) == str && (chCount == 0 || line[chCount - 1] == ' ')
//&& (chCount + std::string(str).size() >= line.size() || std::string(str)[chCount + std::string(str).size()] == ' ')

struct Variable
{
    std::string varType;
    std::string varName;
    Variable(std::string _varType, std::string _varName)
    {
        varName = std::move(_varName);
        varType = std::move(_varType);
    }
};

struct Method
{
    std::string returnType;
    std::string methodName;
    std::vector<Variable> parameters;
    Method(std::string _returnType, std::string _methodName, std::vector<Variable> _parameters)
    {
        returnType = std::move(_returnType);
        methodName = std::move(_methodName);
        parameters = std::move(_parameters);
    }
};

struct VariableType
{
    std::string varTypeName;

    VariableType(std::string _varTypeName)
    {
        varTypeName = std::move(_varTypeName);
    }
};

struct AnalyzingData
{
    std::vector<Variable> vars;
    std::vector<Method> methods;
    std::vector<VariableType> varTypes;
};

bool is_number(const std::string& s)
{
    return std::find_if(s.begin(),
        s.end(), [](unsigned char c)
        {
           return !std::isdigit(c);
        }) == s.end();
}

bool is_word_or_number(const std::string& s)
{
    return std::find_if(s.begin(),
                        s.end(), [](unsigned char c)
                        {
                            return !std::isdigit(c) && !std::isalpha(c);
                        }) == s.end();
}

void add_default_data(AnalyzingData& data)
{
    data.varTypes.push_back(VariableType("vec4"));
    data.varTypes.push_back(VariableType("vec3"));
    data.varTypes.push_back(VariableType("mat4"));

    data.vars.push_back(Variable("vec4", "gl_Position") );
    data.methods.push_back( Method("vec4", "vec4", { Variable("vec3", "xyz"), Variable("float", "w") }) );
    data.methods.push_back( Method("vec3", "abs", { Variable("vec3", "xyz") }) );
}

std::vector<VariableType>::iterator find_variable_type(const std::string& varType, AnalyzingData& data)
{
    return std::find_if(data.varTypes.begin(), data.varTypes.end(), [varType](VariableType type) { return varType == type.varTypeName; } );
}

std::vector<Variable>::iterator find_variable(const std::string& varName, AnalyzingData& data)
{
    return std::find_if(data.vars.begin(), data.vars.end(), [varName](Variable var) { return varName == var.varName; } );
}

std::vector<Method>::iterator find_function(const std::string& methodName, AnalyzingData& data)
{
    return std::find_if(data.methods.begin(), data.methods.end(), [methodName](Method meth) { return methodName == meth.methodName; } );
}

bool have_variable_type(const std::string& varType, AnalyzingData& data)
{
    return find_variable_type(varType, data) != data.varTypes.end();
}

bool have_variable(const std::string& varName, AnalyzingData& data)
{
    return find_variable(varName, data) != data.vars.end();
}

bool have_function(const std::string& methodName, AnalyzingData& data)
{
    return find_function(methodName, data) != data.methods.end();
}

bool is_keyword(const std::string& s)
{
    static const std::vector<std::string> keywords = { "const", "uniform", "void", "in", "out", "if", "while", "for", "do", "struct" };
    return std::find(keywords.begin(), keywords.end(), s) != keywords.end();
}

void check_important(StringHandler& handler, AnalyzingData& data, const std::string& avoidCharacters)
{
    handler.process_upto_words(avoidCharacters);
    if(handler.check_word("//")) { handler.process_entire_line(CommentColor); return; };
    if(handler.get_current_char() == '/') { handler.process_current_char(ErrorColor); return; }
    if(handler.get_word(avoidCharacters) == "#version") { handler.process_entire_line(PreProcessor); return; }
    if(handler.get_current_char() == '#') { handler.process_current_char(WhiteColor); return; }
}


ColorizedText analyze(std::vector<std::string> lines, size_t cursorLine, size_t cursorChar, std::vector<std::string>& availableVariants,
                      size_t& replaceWordSize, size_t& replaceWordChar)
{
    std::string avoidCharacters = " ";
    ColorizedText text;
    StringHandler handler(std::move(lines), text);
    AnalyzingData data;
    add_default_data(data);
    while(!handler.is_finished())
    {
        //Temp
        handler.process_word(WhiteColor, avoidCharacters);
    }
    return text;
}