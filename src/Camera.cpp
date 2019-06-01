#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>

Camera::Camera()
{
    camera_position_c = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    camera_lookat_l = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

Camera::Camera(glm::vec4 camPos, glm::vec4 lookAtPos)
{
    camera_position_c = camPos;
    camera_lookat_l = lookAtPos;
}

void Camera::setCameraAngles(float distance, float theta, float phi)
{
    this->distance = distance;
    this->theta = theta;
    this->phi = phi;
}

glm::mat4 Camera::getViewMatrix()
{
    return Matrix_Camera_View(camera_position_c, camera_view_vector, camera_up_vector);
}

glm::mat4 Camera::getProjectionMatrix()
{
    return Matrix_Perspective(field_of_view, screenRatio, nearplane, farplane);
}

bool Camera::bindEntity(Entity* ent)
{
    /* we bind entity to component only if it has a Transform component */
    if(ent->hasComponent<TransformComponent>())
    {
        std::cout << "Binding entity to camera." << std::endl;
        boundEntity = ent;
        return true;
    }
    else
        return false;
}

void Camera::setCameraMode(CameraMode mode)
{
    std::cout << "Switching camera mode." << std::endl;
    cameraMode = mode;
}

void Camera::update()
{
    float r = distance;
    float y = r*sin(phi);
    float z = r*cos(phi)*cos(theta);
    float x = r*cos(phi)*sin(theta);

    auto entPos = boundEntity->getComponent<TransformComponent>().getPos();


    switch(cameraMode)
    {

        case lookAtEntity:
            camera_lookat_l = entPos;
            x += entPos.x;
            y += entPos.y;
            z += entPos.z;
            camera_position_c = glm::vec4(x, y, z, 1.0f);
            break;
        
        case lookAtEntityAndFollow:
            camera_lookat_l = entPos;
            camera_position_c = glm::vec4(x, y, z, 1.0f);
            break;

        default:
            break;
    }

    /* Update view vector */
    camera_view_vector = camera_lookat_l - camera_position_c;
}