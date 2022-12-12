#ifndef SHADEREDITOR_EDITABLESHADER_H
#define SHADEREDITOR_EDITABLESHADER_H

#include "Shader.h"
#include <vector>
#include <string>

#include <algorithm>
#include <sstream>
#include <iterator>

class EditableShader
{
private:
    Shader* shader = nullptr;
public:
    std::vector<std::string> vertexShaderLines { "", "", "" };
    std::vector<std::string> fragmentShaderLines { "", "", "" };
    Shader* get_shader();
    void use();
    void recompile();
    bool isShaderNull();
};

#endif
