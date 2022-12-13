#ifndef SHADEREDITOR_SHADER_H
#define SHADEREDITOR_SHADER_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Shader
{
private:
    unsigned int ID;
public:
    Shader(const char* vertexShader, const char* fragmentShader);
    ~Shader();
    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMatrix4(const std::string &name, glm::mat4 value);
};


#endif
