#include "Camera.h"

glm::mat4 Camera::get_projection_matrix(float aspect)
{
    return glm::perspective(glm::radians(fov), aspect, near, far);
}

glm::vec3 Camera::get_front()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::normalize(direction);
}

glm::vec3 Camera::get_right()
{
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    return glm::normalize(glm::cross(up, get_front()));
}

glm::mat4 Camera::get_view_matrix()
{
    glm::vec3 cameraFront = get_front();
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    return glm::lookAt(position, position + cameraFront, up);
}