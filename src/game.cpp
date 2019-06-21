#include "Camera.h"
#include "game.h"
#include "ECS/Components.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <matrices.h>
#include "GLManager.h"

#define THROW_ROCK_COOLDOWN 1.0f

#define NUM_ENEMIES 6

Manager manager;
GLManager* glManager;
Camera* camera;

bool Game::isRunning = false;
bool Game::gameLost = false;
double Game::dt = 0;
int Game::playerAmmo = 0;
float Game::screenRatio = 1.0f;
int Game::enemiesDefeated = 0;

double rockCooldown = 0.0f;

#include "callbacks.h"

auto& testEntity(manager.addEntity());
auto& testEntity2(manager.addEntity());
auto& AITest(manager.addEntity());
auto& Terrain(manager.addEntity());
auto& house(manager.addEntity());

auto& box1(manager.addEntity());
auto& box2(manager.addEntity());

auto &wall1(manager.addEntity());
auto &wall2(manager.addEntity());
auto &wall3(manager.addEntity());
auto &wall4(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}

void Game::addBrickWalls(){
    wall1.name = "wall1";
    wall1.addComponent<TransformComponent>(5.0f, 0.0f, 48.0f);
    wall1.getComponent<TransformComponent>().setStuff(0.0f, PI, 0.0f, 5.0f, 3.0f, 10.0f);
    wall1.addComponent<ModelComponent>("../data/BrickWall/wall2.obj", glManager, "gouraud", REPEAT_PLANAR);
    wall1.getComponent<ModelComponent>().loadTexture("../data/BrickWall/wallBrick.jpg");
    wall1.addGroup(mapGroup);

    wall2.name = "wall2";
    wall2.addComponent<TransformComponent>(-5.0f, 0.0f, -48.0f);
    wall2.getComponent<TransformComponent>().setStuff(0.0f, 0.0f, 0.0f, 5.0f, 3.0f, 10.0f);
    wall2.addComponent<ModelComponent>("../data/BrickWall/wall2.obj", glManager, "gouraud", REPEAT_PLANAR);
    wall2.getComponent<ModelComponent>().loadTexture("../data/BrickWall/wallBrick.jpg");
    wall2.addGroup(mapGroup);

    wall3.name = "wall3";
    wall3.addComponent<TransformComponent>(49.0f, 0.0f, 0.0f);
    wall3.getComponent<TransformComponent>().setStuff(0.0f, 3*PI/2, 0.0f, 5.0f, 3.0f, 10.0f);
    wall3.addComponent<ModelComponent>("../data/BrickWall/wall2.obj", glManager, "gouraud", REPEAT_PLANAR);
    wall3.getComponent<ModelComponent>().loadTexture("../data/BrickWall/wallBrick.jpg");
    wall3.addGroup(mapGroup);

    wall4.name = "wall4";
    wall4.addComponent<TransformComponent>(-49.0f, 0.0f, 0.0f);
    wall4.getComponent<TransformComponent>().setStuff(0.0f, PI/2, 0.0f, 5.0f, 3.0f, 10.0f);
    wall4.addComponent<ModelComponent>("../data/BrickWall/wall2.obj", glManager, "gouraud", REPEAT_PLANAR);
    wall4.getComponent<ModelComponent>().loadTexture("../data/BrickWall/wallBrick.jpg");
    wall4.addGroup(mapGroup);
}

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

    glManager = new GLManager();
    camera = new Camera();

    testEntity.name = "player";
    testEntity.addComponent<TransformComponent>(26.001f, 0.002f, 24.001f);
    testEntity.getComponent<TransformComponent>().setStuff(0.0f, 0.0f, 0.0f, 3.0f, 3.0f, 3.0f);
    testEntity.addComponent<ModelComponent>("../data/bunny.obj", glManager, "default", SPHERICAL_PROJECTION);
    testEntity.getComponent<ModelComponent>().loadTexture("../data/tc-earth_daymap_surface.jpg");
    testEntity.addComponent<KeyboardController>(camera);
    testEntity.addComponent<ColliderComponent>("colider");
    testEntity.addGroup(testGroup);

    /*
    AITest.name = "AI test";
    AITest.addComponent<TransformComponent>(36.001f, 0.002f, 20.001f);
    AITest.addComponent<ModelComponent>("../data/bunny.obj", glManager, "default");
    AITest.getComponent<ModelComponent>().loadTexture("../data/tc-earth_daymap_surface.jpg");
    AITest.addComponent<AIComponent>(&testEntity.getComponent<TransformComponent>());
    AITest.addComponent<ColliderComponent>("colider2");
	AITest.addComponent<BezierComponent>(2.0f,
										glm::vec4(2.0f, 10.0f, 0.0f, 1.0f),
										glm::vec4(10.0f, 10.0f, 0.0f, 1.0f),
										glm::vec4(12.0f, 0.0f, 0.0f, 1.0f)
										);
    AITest.addGroup(testGroup);
    */



    box1.name = "box1";
    box1.addComponent<TransformComponent>(42.001f, 0.002f, 30.001f);
    //box1.getComponent<TransformComponent>().setStuff(0.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.1f);
    box1.addComponent<ModelComponent>("../data/miscObj/box.obj", glManager, "default", SPHERICAL_PROJECTION);
    box1.getComponent<ModelComponent>().loadTexture("../data/miscObj/box_diffuse.jpg");
    box1.addComponent<ColliderComponent>("Box1Colider");
    box1.addGroup(testGroup);

    box2.name = "box2";
    box2.addComponent<TransformComponent>(58.001f, 0.002f, 30.001f);
    box2.getComponent<TransformComponent>().setStuff(0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 2.0f);
    box2.addComponent<ModelComponent>("../data/miscObj/box.obj", glManager, "default", SPHERICAL_PROJECTION);
    box2.getComponent<ModelComponent>().loadTexture("../data/miscObj/box_diffuse.jpg");
    box2.addComponent<ColliderComponent>("Box2Colider");
    box2.addGroup(testGroup);

    Terrain.name = "terrain";
    Terrain.addComponent<TransformComponent>();
    Terrain.getComponent<TransformComponent>().setStuff(0.0f, 0.0f, 0.0f, 50.0f, 50.0f, 50.0f);
    Terrain.addComponent<ModelComponent>("../data/grass/grass.obj", glManager, "default", SPHERICAL_PROJECTION);
    Terrain.getComponent<ModelComponent>().loadTexture("../data/grass/grass.png");
    Terrain.getComponent<ModelComponent>().mappingType = 2;
    Terrain.addGroup(mapGroup);

    house.addComponent<TransformComponent>();
    house.getComponent<TransformComponent>().setStuff(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.1f);
    house.addComponent<ModelComponent>("../data/house/house.obj", glManager, "default", SPHERICAL_PROJECTION);
    house.getComponent<ModelComponent>().loadTexture("../data/house/house.jpg");
    house.getComponent<ModelComponent>().mappingType = 2;
    house.addGroup(mapGroup);

    /* Camera set-up */
    camera->bindEntity(&testEntity);
    camera->setCameraMode(Camera::LookAt);

    // glManager->LoadTextureImage("../data/tc-earth_daymap_surface.jpg");      // TextureImage0

    Game::playerAmmo = 0;

    for(int i = 0; i < 10; i++)
        addRockToPick(-30.0f + 8.0f * i, 0.1f, -17.0f);

    for(int i = 0; i < NUM_ENEMIES; i++)
        addEnemy(-20.0f + 10.0f * i, 0.1f, -10.0f);

    addBrickWalls();
}

void Game::addEnemy(double x, double y, double z){
    auto& newEnemy(manager.addEntity());
    newEnemy.name = "Enemy";
    newEnemy.addComponent<TransformComponent>(x, y, z);
    newEnemy.addComponent<ModelComponent>("../data/bunny.obj", glManager, "default", SPHERICAL_PROJECTION);
    newEnemy.getComponent<ModelComponent>().loadTexture("../data/tc-earth_daymap_surface.jpg");
    newEnemy.addComponent<AIComponent>(&testEntity.getComponent<TransformComponent>());
    newEnemy.addComponent<ColliderComponent>("EnemyCollider");
	newEnemy.addComponent<BezierComponent>(2.0f,
										glm::vec4(2.0f, 10.0f, 0.0f, 1.0f),
										glm::vec4(10.0f, 10.0f, 0.0f, 1.0f),
										glm::vec4(12.0f, 0.0f, 0.0f, 1.0f)
										);
    newEnemy.addGroup(testGroup);
}

void Game::addRockToPick(double x, double y, double z){
    auto& rockPickable(manager.addEntity());

    rockPickable.name = "rockPickable";
    rockPickable.addComponent<TransformComponent>(x, y, z);
    rockPickable.getComponent<TransformComponent>().setStuff(0.0f, 0.0f, 0.0f, 0.02f, 0.02f, 0.02f);
    rockPickable.addComponent<ModelComponent>("../data/miscObj/Rock.obj", glManager, "default", SPHERICAL_PROJECTION);
    rockPickable.addComponent<ColliderComponent>("RockPickableColider");
    rockPickable.addGroup(testGroup);
}

void Game::throwRock(){
    if(rockCooldown <= 0.0f && Game::playerAmmo > 0){
        auto& rock(manager.addEntity());
        TransformComponent playerTransf = testEntity.getComponent<TransformComponent>();
        glm::vec4 viewUnit = camera->viewVector / norm(camera->viewVector) * 2.0f * playerTransf.x_Scale;

        rock.name = "rock";
        rock.addComponent<TransformComponent>(playerTransf.getPos().x + playerTransf.xOffset + viewUnit.x, playerTransf.getPos().y + playerTransf.yOffset + viewUnit.y, playerTransf.getPos().z + playerTransf.zOffset + viewUnit.z);
        rock.getComponent<TransformComponent>().setStuff(0.0f, 0.0f, 0.0f, 0.02f, 0.02f, 0.02f);
        rock.addComponent<ModelComponent>("../data/miscObj/Rock.obj", glManager, "default", SPHERICAL_PROJECTION);
        //rock.getComponent<ModelComponent>().loadTexture("../data/tc-earth_daymap_surface.jpg");
        rock.addComponent<ColliderComponent>("RockColider");
        rock.addComponent<AutoKillComponent>(4.0f);
        rock.addGroup(testGroup);

        //TODO: ...No match for operator = ... Vector3D and glm::vec4...
        rock.getComponent<TransformComponent>().velocity.x = camera->viewVector.x * 2.0f;
        rock.getComponent<TransformComponent>().velocity.y = camera->viewVector.y * 2.0f;
        rock.getComponent<TransformComponent>().velocity.z = camera->viewVector.z * 2.0f;

        rockCooldown = THROW_ROCK_COOLDOWN;
        Game::playerAmmo--;
    }
}

void Game::handleEvents()
{
    glfwPollEvents();

    int state;
    /* TODO: key manager */

    state = glfwGetKey(window, GLFW_KEY_F5);
    if(state == GLFW_PRESS)
    {
        camera->setCameraMode(Camera::LookAt);
        testEntity.getComponent<ModelComponent>().show();
    }

    state = glfwGetKey(window, GLFW_KEY_F6);
    if(state == GLFW_PRESS)
    {
        camera->setCameraMode(Camera::FreeCamera);
        testEntity.getComponent<ModelComponent>().show();
    }

    state = glfwGetKey(window, GLFW_KEY_F7);
    if(state == GLFW_PRESS)
    {
        camera->setCameraMode(Camera::FirstPerson);
        testEntity.getComponent<ModelComponent>().hide();
    }

    state = glfwGetKey(window, GLFW_KEY_F9);
    if(state == GLFW_PRESS)
    {
		AITest.getComponent<BezierComponent>().animate();
    }

    if(camera->getCameraMode() == Camera::LookAt ||
       camera->getCameraMode() == Camera::FirstPerson)
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

        //Throw rock
        state = glfwGetKey(window, GLFW_KEY_E);
        if(state == GLFW_PRESS)
            this->throwRock();

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

    rockCooldown -= this->dt;

    if(Game::gameLost){
        printf("Player Lost\n");
        Game::isRunning = false;
    }
    if(Game::enemiesDefeated == NUM_ENEMIES){
        printf("Player Won\n");
        Game::isRunning = false;
    }

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
