#include "Camera.h"
#include "game.h"
#include "ECS/Components.h"
#include <GL/glew.h>
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

    /* TODO: glew manager */ 
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glManager = new GLManager("../src/shader_vertex.glsl", "../src/shader_fragment.glsl");
    camera = new Camera();

    glManager->setActiveShader("default");

    testEntity.addComponent<TransformComponent>(0.001f, 0.002f, 0.001f);
    testEntity.addComponent<ModelComponent>("../bunny.obj", glManager, "defaultShader");
    testEntity.getComponent<ModelComponent>().loadTexture("../data/tc-earth_daymap_surface.jpg");
    testEntity.addComponent<KeyboardController>(camera);
    testEntity.name = "player";
    testEntity.addGroup(testGroup);

    testEntity.getComponent<TransformComponent>().setStuff(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    TransformComponent transf = testEntity.getComponent<TransformComponent>();
    transf.setStuff(-PI/2.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f);

    testEntity2.addComponent<TransformComponent>(0.0f, 0.0f, 0.0f);
    testEntity2.getComponent<TransformComponent>().fixed = true;
    testEntity2.addComponent<ModelComponent>("../plant.obj", glManager, "defaultShader");
    testEntity2.name = "plant";
    testEntity2.addGroup(groupGrass);

    /* AI test */
    AITest.addComponent<TransformComponent>(1.0f, 0.002f, 0.001f);
    AITest.getComponent<TransformComponent>().setStuff(-1.6f, 0.0f, 0.0f, 0.4f, 0.4f, 0.4f);
    // AITest.getComponent<TransformComponent>().yOffset = 1.0f;
    AITest.addComponent<ModelComponent>("../data/mafiaguy/mafia.obj", glManager, "defaultShader");
    AITest.getComponent<ModelComponent>().loadTexture("../data/mafiaguy/Material.006 Diffuse Color.001.png");
    AITest.getComponent<ModelComponent>().mappingType = 3;
    AITest.addComponent<AIComponent>(&testEntity.getComponent<TransformComponent>());
    AITest.name = "AI test";
    AITest.addGroup(testGroup);

    /* Camera set-up */
    camera->bindEntity(&testEntity);
    camera->setCameraMode(Camera::FreeCamera);

    // glManager->LoadTextureImage("../data/tc-earth_daymap_surface.jpg");      // TextureImage0
}

auto& tGroup(manager.getGroup(Game::testGroup));
auto& tGrass(manager.getGroup(Game::groupGrass));

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

    for(auto& t : tGroup)
    {
        t->draw();
    }

    for(auto& t : tGrass)
    {
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
