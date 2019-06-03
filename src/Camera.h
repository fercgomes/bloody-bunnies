#include <matrices.h>
#include <glm/vec4.hpp>
#include "ECS/Components.h"

enum CameraMode
{
    lookAtEntity,
    lookAtEntityAndFollow,
    lookAtPosition
};

class Camera
{
private:
    float theta = 0.0f; // Ângulo no plano ZX em relação ao eixo Z
    float phi = 0.0f;   // Ângulo em relação ao eixo Y
    float distance = 3.5f; // Distância da câmera para a origem

    glm::vec4 camera_position_c;
    glm::vec4 camera_lookat_l;
    glm::vec4 camera_view_vector;
    glm::vec4 camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f); // Vetor "up" fixado para apontar para o "céu" (eito Y global)

    glm::mat4 viewMatrix;

    float nearplane = -0.1f;
    float farplane  = -15.0f;
    
    float field_of_view = 3.141592 / 3.0f;
    float screenRatio = 1.3333f;

    Entity* boundEntity = NULL;

    CameraMode cameraMode;
public:

    Camera();
    Camera(glm::vec4 camPos, glm::vec4 lookAtPos);

    void setCameraAngles(float distance, float theta, float phi);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    bool bindEntity(Entity* ent);
    void setCameraMode(CameraMode mode);

    void update();
};