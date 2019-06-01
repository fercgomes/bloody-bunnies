#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include "../include/tiny_obj_loader.h"
#include "../include/matrices.h"
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ObjModel
{
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;

    // Este construtor lê o modelo de um arquivo utilizando a biblioteca tinyobjloader.
    // Veja: https://github.com/syoyo/tinyobjloader
    ObjModel(){};
    ObjModel(const char* filename, const char* basepath = NULL, bool triangulate = true);
    void computeNormals();
};

struct Object3D
{
private:

public:
    std::string name;
    void* first_index;
    int num_indices;
    GLenum rendering_mode;
    GLuint vertex_array_object_id;

    Object3D(){};
    Object3D(std::string name, void* fi, int ni, GLenum rm, GLuint vao_id) :
        name(name),
        first_index(fi),
        num_indices(ni),
        rendering_mode(rm),
        vertex_array_object_id(vao_id){}

};

class GLManager
{
public:
    std::map<std::string, GLuint> shaders;

    GLint model_uniform;
    GLint view_uniform;
    GLint projection_uniform;

    /* generates a object to use with DrawElements */
    Object3D generateObject3D(ObjModel* model);

    /* draws a object from a Object3D */
    void drawObject(Object3D& model);

    void LoadShadersFromFiles(); // Carrega os shaders de vértice e fragmento, criando um programa de GPU
    GLuint LoadShader_Vertex(const char* filename);   // Carrega um vertex shader
    GLuint LoadShader_Fragment(const char* filename); // Carrega um fragment shader
    void LoadShader(const char* filename, GLuint shader_id); // Função utilizada pelas duas acima
    GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id); // Cria um programa de GPU

    void setActiveShader(std::string shaderName);

    GLManager() {}
    GLManager(const char* vertexShader, const char* fragmentShader);
};