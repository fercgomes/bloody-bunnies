#include "Camera.h"

Camera::Camera()
{
    camera_position_c = glm::vec4(0.0f, 0.0f, 0.01f, 1.0f);
    camera_lookat_l = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    // camera_view_vector = camera_lookat_l - camera_position_c;
    camera_view_vector = camera_position_c - camera_lookat_l;
}

Camera::Camera(glm::vec4 camPos, glm::vec4 lookAtPos)
{
    camera_position_c = camPos;
    camera_lookat_l = lookAtPos;
    camera_view_vector = camera_lookat_l - camera_position_c;
}

glm::mat4 Camera::getViewMatrix()
{
    return Matrix_Camera_View(camera_position_c, camera_view_vector, camera_up_vector);
}