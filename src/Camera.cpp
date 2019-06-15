#include "Camera.h"
#include "ECS/Components.h"
#include "game.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>
#include <matrices.h>

inline float to_degrees(float radians)
{
    return radians * (180.0 / PI);
}

Camera::Camera()
{
    position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    lookAt = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

Camera::Camera(glm::vec4 camPos, glm::vec4 lookAtPos)
{
    position = camPos;
    lookAt = lookAtPos;
}

void Camera::setCameraAngles(float distance, float theta, float phi)
{
    this->distance = distance;
    this->theta = theta;
    this->phi = phi;
}

glm::mat4 Camera::getViewMatrix()
{
    return Matrix_Camera_View(position, viewVector, upVector);
}

glm::mat4 Camera::getProjectionMatrix()
{
    return Matrix_Perspective(field_of_view, Game::screenRatio, nearplane, farplane);
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
    std::cout << "Switching camera mode to: ";
    switch(mode)
    {
        case LookAt:
            std::cout << "LookAt" << std::endl;
            break;
        case FreeCamera:
            std::cout << "FreeCamera" << std::endl;
            break;
        case FirstPerson:
            std::cout << "FirstPerson" << std::endl;
            break;
    }

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
        case FreeCamera:
            lookAt = glm::vec4(x + position.x, y + position.y, z + position.z, 1.0f);
            break;

        case LookAt:
            lookAt = entPos;
            position = glm::vec4(x + entPos.x, y + entPos.y, z + entPos.z, 1.0f);
            break;

        case FirstPerson:
            auto& playerModel = boundEntity->getComponent<ModelComponent>();
            auto& playerTransf = boundEntity->getComponent<TransformComponent>();
            glm::vec3 center = glm::vec3(
                playerModel.model.bbox_max - playerModel.model.bbox_min
            );

            position = glm::vec4(
                center.x + playerTransf.position.x,
                center.y + playerTransf.position.y,
                center.z + playerTransf.position.z,
                1.0f
                );

            lookAt = glm::vec4(
                x + playerTransf.position.x,
                y + playerTransf.position.y,
                z + playerTransf.position.z,
                1.0f
                );

            break;
    }

    /* Update view vector */
    viewVector = lookAt - position;

}

Camera::CameraMode Camera::getCameraMode()
{
    return cameraMode;
}

void Camera::stepForward()
{
    glm::vec4 dir = glm::normalize(viewVector);
    position = position + step * dir;
}

void Camera::stepBackward()
{
    glm::vec4 dir = glm::normalize(viewVector);
    position = position + step * (-dir);
}

void Camera::stepRight()
{
    glm::vec4 dir = glm::normalize(crossproduct(viewVector, upVector));
    position = position + step * (dir);
}

void Camera::stepLeft()
{
    glm::vec4 dir = glm::normalize(crossproduct(viewVector, upVector));
    position = position + step * (-dir);
}
