#include "game.h"
#include "ECS/Components.h"
#include <GL/glew.h>
#include <sstream>
#include <matrices.h>
#include "GLManager.h"
#include "Camera.h"
#include "Audio.cpp"

// Razão de proporção da janela (largura/altura). Veja função FramebufferSizeCallback().
float g_ScreenRatio = 1.0f;

// Ângulos de Euler que controlam a rotação de um dos cubos da cena virtual
float g_AngleX = 0.0f;
float g_AngleY = 0.0f;
float g_AngleZ = 0.0f;

// "g_LeftMouseButtonPressed = true" se o usuário está com o botão esquerdo do mouse
// pressionado no momento atual. Veja função MouseButtonCallback().
bool g_LeftMouseButtonPressed = false;
bool g_RightMouseButtonPressed = false; // Análogo para botão direito do mouse
bool g_MiddleMouseButtonPressed = false; // Análogo para botão do meio do mouse

// Variáveis que definem a câmera em coordenadas esféricas, controladas pelo
// usuário através do mouse (veja função CursorPosCallback()). A posição
// efetiva da câmera é calculada dentro da função main(), dentro do loop de
// renderização.
float g_CameraTheta = 0.0f; // Ângulo no plano ZX em relação ao eixo Z
float g_CameraPhi = 0.0f;   // Ângulo em relação ao eixo Y
float g_CameraDistance = 3.5f; // Distância da câmera para a origem

// Variável que controla o tipo de projeção utilizada: perspectiva ou ortográfica.
bool g_UsePerspectiveProjection = true;

Manager manager;
// GLManager glManager("../src/shader_vertex.glsl", "../src/shader_fragment.glsl");
GLManager* glManager;
Camera* camera;

SDL_Event Game::event;

bool Game::isRunning = false;

// auto& player(manager.addEntity());
// auto& label(manager.addEntity());

auto& testEntity(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = SDL_WINDOW_OPENGL;
    if(fullscreen)
    {
        flags = flags | SDL_WINDOW_FULLSCREEN;
    }

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "SDL subsystems initialized." << std::endl;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(window)
        {
            std::cout << "Window created." << std::endl;
        }

        isRunning = true;
    }
    else
    {
        isRunning = false;
    }
    // /* player */
    // player.addComponent<TransformComponent>(4);
    // /* TODO: frame signature ? */
    // player.addComponent<SpriteComponent>("player", true);
    // player.addComponent<KeyboardController>();
    // player.addComponent<ColliderComponent>("player");
    // player.addGroup(groupPlayers);


    /* Setting up OpenGL stuff */
    /* TODO: put in a OpenGL manager */
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE
    );

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    context = SDL_GL_CreateContext(window);

    /* TODO: glew manager */ 
    glewExperimental = GL_TRUE;
    glewInit();

    // Habilitamos o Z-buffer. Veja slide 108 do documento "Aula_09_Projecoes.pdf".
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling. Veja slides 22-34 do documento "Aula_13_Clipping_and_Culling.pdf".
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glManager = new GLManager("../src/shader_vertex.glsl", "../src/shader_fragment.glsl");
    camera = new Camera();


    testEntity.addComponent<TransformComponent>(0.0f, 0.1f, 0.0f);
    testEntity.addComponent<ModelComponent>("bunny.obj", glManager);
    testEntity.addComponent<KeyboardController>();
    testEntity.addGroup(testGroup);

    Audio::play("../420.wav");
}

auto& tGroup(manager.getGroup(Game::testGroup));

void Game::handleEvents()
{
    SDL_PollEvent(&event);
    switch (event.type)
    {
        case SDL_QUIT:
            isRunning = false;
            break;
    
        default:
            break;
    }

}

void Game::update()
{
    manager.refresh();
    manager.update();
}


void Game::render()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(glManager->program_id);

    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection;

    // Note que, no sistema de coordenadas da câmera, os planos near e far
    // estão no sentido negativo! Veja slides 190-193 do documento "Aula_09_Projecoes.pdf".
    float nearplane = -0.1f;  // Posição do "near plane"
    float farplane  = -10.0f; // Posição do "far plane"

    if (g_UsePerspectiveProjection)
    {
        // Projeção Perspectiva.
        // Para definição do field of view (FOV), veja slide 227 do documento "Aula_09_Projecoes.pdf".
        float field_of_view = 3.141592 / 3.0f;
        projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);
    }
    else
    {
        // Projeção Ortográfica.
        // Para definição dos valores l, r, b, t ("left", "right", "bottom", "top"),
        // PARA PROJEÇÃO ORTOGRÁFICA veja slide 236 do documento "Aula_09_Projecoes.pdf".
        // Para simular um "zoom" ortográfico, computamos o valor de "t"
        // utilizando a variável g_CameraDistance.
        float t = 1.5f*g_CameraDistance/2.5f;
        float b = -t;
        float r = t*g_ScreenRatio;
        float l = -r;
        projection = Matrix_Orthographic(l, r, b, t, nearplane, farplane);
    }

    // Enviamos as matrizes "view" e "projection" para a placa de vídeo
    // (GPU). Veja o arquivo "shader_vertex.glsl", onde estas são
    // efetivamente aplicadas em todos os pontos.
    glUniformMatrix4fv(glManager->view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
    glUniformMatrix4fv(glManager->projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));

    #define SPHERE 0
    #define BUNNY  1
    #define PLANE  2
    glUniform1i(glManager->object_id_uniform, SPHERE);

    for(auto& t : tGroup)
    {
        /* Fetch the model matrix for this entity */
        glm::mat4 model = t->getComponent<TransformComponent>().getModelMatrix();
        /* Send it to the GPU */
        glUniformMatrix4fv(glManager->model_uniform, 1 , GL_FALSE , glm::value_ptr(model));

        t->draw();
    }

    SDL_GL_SwapWindow(window);
}

void Game::clean()
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}

bool Game::running()
{
    return isRunning;
}