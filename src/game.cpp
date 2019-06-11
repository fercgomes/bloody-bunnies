#include "Camera.h"
#include "game.h"
#include "ECS/Components.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <matrices.h>
#include "GLManager.h"

Manager manager;
GLManager* glManager;
Camera* camera;

bool Game::isRunning = false;
double Game::dt = 0;
float Game::screenRatio = 1.0f;

#include "callbacks.h"

auto& testEntity(manager.addEntity());
auto& testEntity2(manager.addEntity());
auto& AITest(manager.addEntity());
auto& Terrain(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int width, int height)
{
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    /* Set game's screen ratio */
    Game::screenRatio = (float) width / height;

    glfwSetErrorCallback(ErrorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }
    
    Game::isRunning = true;

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    FramebufferSizeCallback(window, width, height);

    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glManager = new GLManager("../src/shader_vertex.glsl", "../src/shader_fragment.glsl");
    camera = new Camera();

    glManager->setActiveShader("default");

    testEntity.name = "player";
    testEntity.addComponent<TransformComponent>(0.001f, 0.002f, 0.001f);
    testEntity.addComponent<ModelComponent>("../bunny.obj", glManager, "default");
    testEntity.getComponent<ModelComponent>().loadTexture("../data/tc-earth_daymap_surface.jpg");
    testEntity.addComponent<KeyboardController>(camera);
    testEntity.addGroup(testGroup);

    /* AI test */
    AITest.name = "AI test";
    AITest.addComponent<TransformComponent>(1.0f, 0.002f, 0.001f);
    AITest.addComponent<ModelComponent>("../bunny.obj", glManager, "default");
    AITest.getComponent<ModelComponent>().loadTexture("../data/tc-earth_daymap_surface.jpg");
    AITest.addComponent<AIComponent>(&testEntity.getComponent<TransformComponent>());
    AITest.addGroup(testGroup);

    Terrain.name = "terrain";
    Terrain.addComponent<TransformComponent>();
    Terrain.getComponent<TransformComponent>().setStuff(0.0f, 0.0f, 0.0f, 50.0f, 50.0f, 50.0f);
    Terrain.addComponent<ModelComponent>("../data/grass/grass.obj", glManager, "default");
    Terrain.getComponent<ModelComponent>().loadTexture("../data/grass/grass.png");
    Terrain.getComponent<ModelComponent>().mappingType = 2;
    Terrain.addGroup(mapGroup);

    /* Camera set-up */
    camera->bindEntity(&testEntity);
    camera->setCameraMode(Camera::LookAt);

    // glManager->LoadTextureImage("../data/tc-earth_daymap_surface.jpg");      // TextureImage0
}


void Game::handleEvents()
{
    glfwPollEvents();
    
    int state;
    /* TODO: key manager */

    state = glfwGetKey(window, GLFW_KEY_F5);
    if(state == GLFW_PRESS)
        camera->cycleMode();

    if(camera->getCameraMode() == Camera::LookAt)
    {
        auto player = &testEntity.getComponent<KeyboardController>();
        state = glfwGetKey(window, GLFW_KEY_W);
        if(state == GLFW_PRESS)
            player->goingForward = true;
        else if(state == GLFW_RELEASE)
            player->goingForward = false;

        state = glfwGetKey(window, GLFW_KEY_S);
        if(state == GLFW_PRESS)
            player->goingBackwards = true;
        else if(state == GLFW_RELEASE)
            player->goingBackwards = false;

        state = glfwGetKey(window, GLFW_KEY_D);
        if(state == GLFW_PRESS)
            player->goingRight = true;
        else if(state == GLFW_RELEASE)
            player->goingRight = false;

        state = glfwGetKey(window, GLFW_KEY_A);
        if(state == GLFW_PRESS)
            player->goingLeft = true;
        else if(state == GLFW_RELEASE)
            player->goingLeft = false;

        state = glfwGetKey(window, GLFW_KEY_SPACE);
        if(state == GLFW_PRESS)
            player->jump = true;
    }
    else if(camera->getCameraMode() == Camera::FreeCamera)
    {
        int state = glfwGetKey(window, GLFW_KEY_W);
        if(state == GLFW_PRESS)
            camera->stepForward();

        state = glfwGetKey(window, GLFW_KEY_S);
        if(state == GLFW_PRESS)
            camera->stepBackward();

        state = glfwGetKey(window, GLFW_KEY_D);
        if(state == GLFW_PRESS)
            camera->stepRight();

        state = glfwGetKey(window, GLFW_KEY_A);
        if(state == GLFW_PRESS)
            camera->stepLeft();
    }
}

auto& tGroup(manager.getGroup(Game::testGroup));
auto& tGrass(manager.getGroup(Game::groupGrass));
auto& tMap(manager.getGroup(Game::mapGroup));

void Game::update()
{
    manager.refresh();
    manager.update();

    testEntity.getComponent<TransformComponent>().y_Rotation = 2.5f + g_CameraTheta;

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

    glUniformMatrix4fv(glManager->view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
    glUniformMatrix4fv(glManager->projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));

    for(auto& t : tMap)
        t->draw();

    for(auto& t : tGroup)
        t->draw();

    for(auto& t : tGrass)
        t->draw();

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
