#include <matrices.h>
#include <glm/vec4.hpp>

class Camera
{
private:
    float g_CameraTheta = 0.0f; // Ângulo no plano ZX em relação ao eixo Z
    float g_CameraPhi = 0.0f;   // Ângulo em relação ao eixo Y
    float g_CameraDistance = 3.5f; // Distância da câmera para a origem

    // float r = g_CameraDistance;
    // float y = r*sin(g_CameraPhi);
    // float z = r*cos(g_CameraPhi)*cos(g_CameraTheta);
    // float x = r*cos(g_CameraPhi)*sin(g_CameraTheta);

    glm::vec4 camera_position_c;
    glm::vec4 camera_lookat_l;
    glm::vec4 camera_view_vector;
    glm::vec4 camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f); // Vetor "up" fixado para apontar para o "céu" (eito Y global)

    glm::mat4 viewMatrix;

public:
    Camera();
    Camera(glm::vec4 camPos, glm::vec4 lookAtPos);

    glm::mat4 getViewMatrix();
};