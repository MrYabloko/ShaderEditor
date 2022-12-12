#include "EditableShader.h"

std::string join(std::vector<std::string> const &strings)
{
    std::string delim = "\n";

    std::stringstream ss;
    std::copy(strings.begin(), strings.end(),
              std::ostream_iterator<std::string>(ss, delim.c_str()));
    return ss.str();
}

void EditableShader::recompile()
{
    if(shader != nullptr) delete shader;
    shader = new Shader( join(vertexShaderLines).c_str(), join(fragmentShaderLines).c_str() );
}

bool EditableShader::isShaderNull()
{
    return shader == nullptr;
}

void EditableShader::use()
{
    if(shader == nullptr) return; shader->use();
}

Shader* EditableShader::get_shader()
{
    return shader;
}