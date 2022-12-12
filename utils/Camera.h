#ifndef SHADEREDITOR_CAMERA_H
#define SHADEREDITOR_CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
public:
    glm::vec3 position;
    float yaw;
    float pitch;
    float fov = 90.0f;
    float near = 0.1f;
    float far = 100.0f;
    glm::vec3 get_front();
    glm::vec3 get_right();
    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix(float aspect);
};


#endif
