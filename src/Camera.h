#pragma once
#include <matrices.h>
#include <glm/vec4.hpp>

class Entity;

class Camera
{
public:
    enum CameraMode
    {
        FreeCamera,
        LookAt,
        FirstPerson
    };

    glm::vec4 viewVector;

    Camera();
    Camera(glm::vec4 camPos, glm::vec4 lookAtPos);

    void setCameraAngles(float distance, float theta, float phi);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    bool bindEntity(Entity* ent);
    void setCameraMode(CameraMode mode);
    CameraMode getCameraMode();

    void update();

    void stepForward();
    void stepBackward();
    void stepRight();
    void stepLeft();

private:
    float theta = 0.0f; // Ângulo no plano ZX em relação ao eixo Z
    float phi = 0.0f;   // Ângulo em relação ao eixo Y
    float distance = 3.5f; // Distância da câmera para a origem

    glm::vec4 position;
    glm::vec4 lookAt;
    glm::vec4 upVector   = glm::vec4(0.0f,1.0f,0.0f,0.0f); // Vetor "up" fixado para apontar para o "céu" (eito Y global)

    glm::mat4 viewMatrix;

    float nearplane = -0.1f;
    float farplane  = -100.0f;
    
    float field_of_view = 3.141592 / 3.0f;

    float step = 0.5f;

    Entity* boundEntity = NULL;

    CameraMode cameraMode;
};