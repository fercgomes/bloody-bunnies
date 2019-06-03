#include "game.h"
#include "ECS/Components.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <matrices.h>
#include "GLManager.h"
#include "Camera.h"
#include "Audio.cpp"

#include "InputManager.cpp"

Manager manager;
// GLManager glManager("../src/shader_vertex.glsl", "../src/shader_fragment.glsl");
GLManager* glManager;
Camera* camera;

bool Game::isRunning = false;
double Game::dt = 0;
float Game::screenRatio = 1.0f;

#include "callbacks.h"

// auto& player(manager.addEntity());
// auto& label(manager.addEntity());

auto& testEntity(manager.addEntity());
auto& testEntity2(manager.addEntity());

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(ErrorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "INF01047 - Seu Cartao - Seu Nome", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }
    
    Game::isRunning = true;

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    FramebufferSizeCallback(window, 800, 600); // Forçamos a chamada do callback acima, para definir g_ScreenRatio.

    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    /* TODO: glew manager */ 
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glManager = new GLManager("../src/shader_vertex.glsl", "../src/shader_fragment.glsl");
    camera = new Camera();

    testEntity.addComponent<TransformComponent>(0.001f, 0.002f, 0.001f);
    testEntity.addComponent<ModelComponent>("../bunny.obj", glManager, "defaultShader");
    testEntity.addComponent<KeyboardController>();
    testEntity.addGroup(testGroup);

    TransformComponent transf = testEntity.getComponent<TransformComponent>();
    transf.setStuff(-PI/2.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f);

    testEntity2.addComponent<TransformComponent>(0.0f, 0.0f, 0.0f);
    testEntity2.getComponent<TransformComponent>().fixed = true;
    testEntity2.addComponent<ModelComponent>("../plant.obj", glManager, "defaultShader");
    testEntity2.addGroup(groupGrass);

    camera->bindEntity(&testEntity);
    camera->setCameraMode(lookAtEntityAndFollow);

    // Audio::play("../420.wav");
}

auto& tGroup(manager.getGroup(Game::testGroup));
auto& tGrass(manager.getGroup(Game::groupGrass));

void Game::handleEvents()
{
    glfwPollEvents();
}

void Game::update()
{
    manager.refresh();
    manager.update();

    camera->setCameraAngles(g_CameraDistance, g_CameraTheta, g_CameraPhi);
    camera->update();

    Game::isRunning = !glfwWindowShouldClose(window);
}


void Game::render()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();

    // Enviamos as matrizes "view" e "projection" para a placa de vídeo
    // (GPU). Veja o arquivo "shader_vertex.glsl", onde estas são
    // efetivamente aplicadas em todos os pontos.
    glUniformMatrix4fv(glManager->view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
    glUniformMatrix4fv(glManager->projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));

    for(auto& t : tGroup)
    {
        /* Fetch the model matrix for this entity */
        glm::mat4 model = t->getComponent<TransformComponent>().getModelMatrix();
        /* Send it to the GPU */
        glUniformMatrix4fv(glManager->model_uniform, 1 , GL_FALSE , glm::value_ptr(model));

        t->draw();
    }

    for(auto& t : tGrass)
    {
        /* Fetch the model matrix for this entity */
        glm::mat4 model = t->getComponent<TransformComponent>().getModelMatrix();
        /* Send it to the GPU */
        glUniformMatrix4fv(glManager->model_uniform, 1 , GL_FALSE , glm::value_ptr(model));

        t->draw();
    }

    glfwSwapBuffers(window);
}

void Game::clean()
{
    glfwTerminate();
    std::cout << "Game cleaned." << std::endl;
}

bool Game::running()
{
    return isRunning;
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    float walkSpeed = 10.0f;

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        testEntity.getComponent<TransformComponent>().velocity.z = walkSpeed;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        testEntity.getComponent<TransformComponent>().velocity.z = 0.0f;

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        testEntity.getComponent<TransformComponent>().velocity.z = -walkSpeed;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        testEntity.getComponent<TransformComponent>().velocity.z = 0.0f;

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        testEntity.getComponent<TransformComponent>().velocity.x = -walkSpeed;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        testEntity.getComponent<TransformComponent>().velocity.x = 0.0f;

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        testEntity.getComponent<TransformComponent>().velocity.x = walkSpeed;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        testEntity.getComponent<TransformComponent>().velocity.x = 0.0f;

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        testEntity.getComponent<TransformComponent>().velocity.y = 5.0f;
}