#include "GLManager.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "../include/tiny_obj_loader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <dejavufont.h>
#include <utils.h>

ObjModel::ObjModel(const char* filename, const char* basepath, bool triangulate)
{
    printf("Carregando modelo \"%s\"... ", filename);

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, basepath, triangulate);

    if (!err.empty())
        fprintf(stderr, "\n%s\n", err.c_str());

    if (!ret)
        throw std::runtime_error("Erro ao carregar modelo.");
    
    printf("OK.\n");
}

void ObjModel::computeNormals()
{
    auto model = this;
    if ( !model->attrib.normals.empty() )
        return;

    // Primeiro computamos as normais para todos os TRIÂNGULOS.
    // Segundo, computamos as normais dos VÉRTICES através do método proposto
    // por Gouraud, onde a normal de cada vértice vai ser a média das normais de
    // todas as faces que compartilham este vértice.

    size_t num_vertices = model->attrib.vertices.size() / 3;

    std::vector<int> num_triangles_per_vertex(num_vertices, 0);
    std::vector<glm::vec4> vertex_normals(num_vertices, glm::vec4(0.0f,0.0f,0.0f,0.0f));

    for (size_t shape = 0; shape < model->shapes.size(); ++shape)
    {
        size_t num_triangles = model->shapes[shape].mesh.num_face_vertices.size();

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(model->shapes[shape].mesh.num_face_vertices[triangle] == 3);

            glm::vec4  vertices[3];
            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];
                const float vx = model->attrib.vertices[3*idx.vertex_index + 0];
                const float vy = model->attrib.vertices[3*idx.vertex_index + 1];
                const float vz = model->attrib.vertices[3*idx.vertex_index + 2];
                vertices[vertex] = glm::vec4(vx,vy,vz,1.0);
            }

            const glm::vec4  a = vertices[0];
            const glm::vec4  b = vertices[1];
            const glm::vec4  c = vertices[2];

            // PREENCHA AQUI o cálculo da normal de um triângulo cujos vértices
            // estão nos pontos "a", "b", e "c", definidos no sentido anti-horário.
            const glm::vec4  n = crossproduct(b - a, c - a);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];
                num_triangles_per_vertex[idx.vertex_index] += 1;
                vertex_normals[idx.vertex_index] += n;
                model->shapes[shape].mesh.indices[3*triangle + vertex].normal_index = idx.vertex_index;
            }
        }
    }

    model->attrib.normals.resize( 3*num_vertices );

    for (size_t i = 0; i < vertex_normals.size(); ++i)
    {
        glm::vec4 n = vertex_normals[i] / (float)num_triangles_per_vertex[i];
        n /= norm(n);
        model->attrib.normals[3*i + 0] = n.x;
        model->attrib.normals[3*i + 1] = n.y;
        model->attrib.normals[3*i + 2] = n.z;
    }
}

Object3D GLManager::generateObject3D(ObjModel* model)
{
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);
    glBindVertexArray(vertex_array_object_id);

    std::vector<GLuint> indices;
    std::vector<float>  model_coefficients;
    std::vector<float>  normal_coefficients;
    std::vector<float>  texture_coefficients;

    Object3D theobject;

    for (size_t shape = 0; shape < model->shapes.size(); ++shape)
    {
        size_t first_index = indices.size();
        size_t num_triangles = model->shapes[shape].mesh.num_face_vertices.size();

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(model->shapes[shape].mesh.num_face_vertices[triangle] == 3);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];

                indices.push_back(first_index + 3*triangle + vertex);

                const float vx = model->attrib.vertices[3*idx.vertex_index + 0];
                const float vy = model->attrib.vertices[3*idx.vertex_index + 1];
                const float vz = model->attrib.vertices[3*idx.vertex_index + 2];
                //printf("tri %d vert %d = (%.2f, %.2f, %.2f)\n", (int)triangle, (int)vertex, vx, vy, vz);
                model_coefficients.push_back( vx ); // X
                model_coefficients.push_back( vy ); // Y
                model_coefficients.push_back( vz ); // Z
                model_coefficients.push_back( 1.0f ); // W

                // Inspecionando o código da tinyobjloader, o aluno Bernardo
                // Sulzbach (2017/1) apontou que a maneira correta de testar se
                // existem normais e coordenadas de textura no ObjModel é
                // comparando se o índice retornado é -1. Fazemos isso abaixo.

                if ( idx.normal_index != -1 )
                {
                    const float nx = model->attrib.normals[3*idx.normal_index + 0];
                    const float ny = model->attrib.normals[3*idx.normal_index + 1];
                    const float nz = model->attrib.normals[3*idx.normal_index + 2];
                    normal_coefficients.push_back( nx ); // X
                    normal_coefficients.push_back( ny ); // Y
                    normal_coefficients.push_back( nz ); // Z
                    normal_coefficients.push_back( 0.0f ); // W
                }

                if ( idx.texcoord_index != -1 )
                {
                    const float u = model->attrib.texcoords[2*idx.texcoord_index + 0];
                    const float v = model->attrib.texcoords[2*idx.texcoord_index + 1];
                    texture_coefficients.push_back( u );
                    texture_coefficients.push_back( v );
                }
            }
        }

        size_t last_index = indices.size() - 1;

        theobject = Object3D(model->shapes[shape].name, (void*)first_index, last_index - first_index + 1, GL_TRIANGLES, vertex_array_object_id);
    }

    GLuint VBO_model_coefficients_id;
    glGenBuffers(1, &VBO_model_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
    GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
    GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if ( !normal_coefficients.empty() )
    {
        GLuint VBO_normal_coefficients_id;
        glGenBuffers(1, &VBO_normal_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, normal_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, normal_coefficients.size() * sizeof(float), normal_coefficients.data());
        location = 1; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if ( !texture_coefficients.empty() )
    {
        GLuint VBO_texture_coefficients_id;
        glGenBuffers(1, &VBO_texture_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, texture_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, texture_coefficients.size() * sizeof(float), texture_coefficients.data());
        location = 2; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 2; // vec2 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // XXX Errado!
    //

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);

    return theobject;
}

void GLManager::drawObject(Object3D& model)
{
    glBindVertexArray(model.vertex_array_object_id);
    glDrawElements(
        model.rendering_mode,
        model.num_indices,
        GL_UNSIGNED_INT,
        (void*)model.first_index
    );
    glBindVertexArray(0);
}


void GLManager::LoadShadersFromFiles()
{
    const char* defaultVertex = "../src/shader_vertex.glsl";
    const char* defaultFragment = "../src/shader_fragment.glsl";

    auto vertex_shader_id = LoadShader_Vertex(defaultVertex);
    auto fragment_shader_id = LoadShader_Fragment(defaultFragment);

    auto program_id = CreateGpuProgram(vertex_shader_id, fragment_shader_id);
    shaders.emplace(std::pair<std::string, GLuint>("defaultShader", program_id));

    // Buscamos o endereço das variáveis definidas dentro do Vertex Shader.
    // Utilizaremos estas variáveis para enviar dados para a placa de vídeo
    // (GPU)! Veja arquivo "shader_vertex.glsl" e "shader_fragment.glsl".
    model_uniform           = glGetUniformLocation(program_id, "model"); // Variável da matriz "model"
    view_uniform            = glGetUniformLocation(program_id, "view"); // Variável da matriz "view" em shader_vertex.glsl
    projection_uniform      = glGetUniformLocation(program_id, "projection"); // Variável da matriz "projection" em shader_vertex.glsl
}

GLuint GLManager::LoadShader_Vertex(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos vértices.
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, vertex_shader_id);

    // Retorna o ID gerado acima
    return vertex_shader_id;
}

GLuint GLManager::LoadShader_Fragment(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos fragmentos.
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, fragment_shader_id);

    // Retorna o ID gerado acima
    return fragment_shader_id;
}

void GLManager::LoadShader(const char* filename, GLuint shader_id)
{
    // Lemos o arquivo de texto indicado pela variável "filename"
    // e colocamos seu conteúdo em memória, apontado pela variável
    // "shader_string".
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::failbit);
        file.open(filename);
    } catch ( std::exception& e ) {
        fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }
    std::stringstream shader;
    shader << file.rdbuf();
    std::string str = shader.str();
    const GLchar* shader_string = str.c_str();
    const GLint   shader_string_length = static_cast<GLint>( str.length() );

    // Define o código do shader GLSL, contido na string "shader_string"
    glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

    // Compila o código do shader GLSL (em tempo de execução)
    glCompileShader(shader_id);

    // Verificamos se ocorreu algum erro ou "warning" durante a compilação
    GLint compiled_ok;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    // Alocamos memória para guardar o log de compilação.
    // A chamada "new" em C++ é equivalente ao "malloc()" do C.
    GLchar* log = new GLchar[log_length];
    glGetShaderInfoLog(shader_id, log_length, &log_length, log);

    // Imprime no terminal qualquer erro ou "warning" de compilação
    if ( log_length != 0 )
    {
        std::string  output;

        if ( !compiled_ok )
        {
            output += "ERROR: OpenGL compilation of \"";
            output += filename;
            output += "\" failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }
        else
        {
            output += "WARNING: OpenGL compilation of \"";
            output += filename;
            output += "\".\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        fprintf(stderr, "%s", output.c_str());
    }
    else
        std::cout << "Shader compiled." << std::endl;

    // A chamada "delete" em C++ é equivalente ao "free()" do C
    delete [] log;
}

GLuint GLManager::CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
{
    // Criamos um identificador (ID) para este programa de GPU
    GLuint program_id = glCreateProgram();

    // Definição dos dois shaders GLSL que devem ser executados pelo programa
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    // Linkagem dos shaders acima ao programa
    glLinkProgram(program_id);

    // Verificamos se ocorreu algum erro durante a linkagem
    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    // Imprime no terminal qualquer erro de linkagem
    if ( linked_ok == GL_FALSE )
    {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos memória para guardar o log de compilação.
        // A chamada "new" em C++ é equivalente ao "malloc()" do C.
        GLchar* log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "\n== End of link log\n";

        // A chamada "delete" em C++ é equivalente ao "free()" do C
        delete [] log;

        fprintf(stderr, "%s", output.c_str());
    }

    // Os "Shader Objects" podem ser marcados para deleção após serem linkados 
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    // Retornamos o ID gerado acima
    return program_id;
}

GLManager::GLManager(const char* vertexShader, const char* fragmentShader)
{
    LoadShadersFromFiles();
}

void GLManager::setActiveShader(std::string shaderName)
{
    if(shaderName.empty())
    {
        glUseProgram(0);
    }
    else
    {
        glUseProgram(shaders[shaderName]);
    }
}


void GLManager::TextRendering_LoadShader(const GLchar* const shader_string, GLuint shader_id)
{
    // Define o código do shader, contido na string "shader_string"
    glShaderSource(shader_id, 1, &shader_string, NULL);

    // Compila o código do shader (em tempo de execução)
    glCompileShader(shader_id);

    // Verificamos se ocorreu algum erro ou "warning" durante a compilação
    GLint compiled_ok;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    // Alocamos memória para guardar o log de compilação.
    // A chamada "new" em C++ é equivalente ao "malloc()" do C.
    GLchar* log = new GLchar[log_length];
    glGetShaderInfoLog(shader_id, log_length, &log_length, log);

    // Imprime no terminal qualquer erro ou "warning" de compilação
    if ( log_length != 0 )
    {
        std::string  output;

        if ( !compiled_ok )
        {
            output += "ERROR: OpenGL compilation failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }
        else
        {
            output += "ERROR: OpenGL compilation failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        fprintf(stderr, "%s", output.c_str());
    }

    // A chamada "delete" em C++ é equivalente ao "free()" do C
    delete [] log;
}

void GLManager::TextRendering_Init()
{
    GLuint sampler;

    glGenBuffers(1, &textVBO);
    glGenVertexArrays(1, &textVAO);
    glGenTextures(1, &texttexture_id);
    glGenSamplers(1, &sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glCheckError();

    GLuint textvertexshader_id = glCreateShader(GL_VERTEX_SHADER);
    TextRendering_LoadShader(textvertexshader_source, textvertexshader_id);
    // glCheckError();

    GLuint textfragmentshader_id = glCreateShader(GL_FRAGMENT_SHADER);
    TextRendering_LoadShader(textfragmentshader_source, textfragmentshader_id);
    // glCheckError();

    textprogram_id = CreateGpuProgram(textvertexshader_id, textfragmentshader_id);
    glLinkProgram(textprogram_id);
    // glCheckError();

    GLuint texttex_uniform;
    texttex_uniform = glGetUniformLocation(textprogram_id, "tex");
    // glCheckError();

    GLuint textureunit = 31;
    glActiveTexture(GL_TEXTURE0 + textureunit);
    glBindTexture(GL_TEXTURE_2D, texttexture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, dejavufont.tex_width, dejavufont.tex_height, 0, GL_RED, GL_UNSIGNED_BYTE, dejavufont.tex_data);
    glBindSampler(textureunit, sampler);
    // glCheckError();

    glBindVertexArray(textVAO);

    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    // glCheckError();

    glUseProgram(textprogram_id);
    glUniform1i(texttex_uniform, textureunit);
    glUseProgram(0);
    // glCheckError();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // glCheckError();
}

float textscale = 1.5f;

void GLManager::TextRendering_PrintString(GLFWwindow* window, const std::string &str, float x, float y, float scale = 1.0f)
{
    scale *= textscale;
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float sx = scale / width;
    float sy = scale / height;

    for (size_t i = 0; i < str.size(); i++)
    {
        // Find the glyph for the character we are looking for
        texture_glyph_t *glyph = 0;
        for (size_t j = 0; j < dejavufont.glyphs_count; ++j)
        {
            if (dejavufont.glyphs[j].codepoint == (uint32_t)str[i])
            {
                glyph = &dejavufont.glyphs[j];
                break;
            }
        }
        if (!glyph) {
            continue;
        }
        x += glyph->kerning[0].kerning;
        float x0 = (float) (x + glyph->offset_x * sx);
        float y0 = (float) (y + glyph->offset_y * sy);
        float x1 = (float) (x0 + glyph->width * sx);
        float y1 = (float) (y0 - glyph->height * sy);

        float s0 = glyph->s0 - 0.5f/dejavufont.tex_width;
        float t0 = glyph->t0 - 0.5f/dejavufont.tex_height;
        float s1 = glyph->s1 - 0.5f/dejavufont.tex_width;
        float t1 = glyph->t1 - 0.5f/dejavufont.tex_height;

        struct {float x, y, s, t;} data[6] = {
            { x0, y0, s0, t0 },
            { x0, y1, s0, t1 },
            { x1, y1, s1, t1 },
            { x0, y0, s0, t0 },
            { x1, y1, s1, t1 },
            { x1, y0, s1, t0 }
        };

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDepthFunc(GL_ALWAYS);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(float), data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glUseProgram(textprogram_id);
        glBindVertexArray(textVAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
        glUseProgram(0);
        glDepthFunc(GL_LESS);

        glDisable(GL_BLEND);

        x += (glyph->advance_x * sx);
    }
}

float GLManager::TextRendering_LineHeight(GLFWwindow* window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return dejavufont.height / height * textscale;
}

float GLManager::TextRendering_CharWidth(GLFWwindow* window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return dejavufont.glyphs[32].advance_x / width * textscale;
}

void GLManager::TextRendering_PrintMatrix(GLFWwindow* window, glm::mat4 M, float x, float y, float scale = 1.0f)
{
    char buffer[40];
    float lineheight = TextRendering_LineHeight(window) * scale;

    snprintf(buffer, 40, "[%+0.2f %+0.2f %+0.2f %+0.2f]", M[0][0], M[1][0], M[2][0], M[3][0]);
    TextRendering_PrintString(window, buffer, x, y, scale);
    snprintf(buffer, 40, "[%+0.2f %+0.2f %+0.2f %+0.2f]", M[0][1], M[1][1], M[2][1], M[3][1]);
    TextRendering_PrintString(window, buffer, x, y - lineheight, scale);
    snprintf(buffer, 40, "[%+0.2f %+0.2f %+0.2f %+0.2f]", M[0][2], M[1][2], M[2][2], M[3][2]);
    TextRendering_PrintString(window, buffer, x, y - 2*lineheight, scale);
    snprintf(buffer, 40, "[%+0.2f %+0.2f %+0.2f %+0.2f]", M[0][3], M[1][3], M[2][3], M[3][3]);
    TextRendering_PrintString(window, buffer, x, y - 3*lineheight, scale);
}

void GLManager::TextRendering_PrintVector(GLFWwindow* window, glm::vec4 v, float x, float y, float scale = 1.0f)
{
    char buffer[10];
    float lineheight = TextRendering_LineHeight(window) * scale;

    snprintf(buffer, 10, "[%+0.2f]", v.x);
    TextRendering_PrintString(window, buffer, x, y, scale);
    snprintf(buffer, 10, "[%+0.2f]", v.y);
    TextRendering_PrintString(window, buffer, x, y - lineheight, scale);
    snprintf(buffer, 10, "[%+0.2f]", v.z);
    TextRendering_PrintString(window, buffer, x, y - 2*lineheight, scale);
    snprintf(buffer, 10, "[%+0.2f]", v.w);
    TextRendering_PrintString(window, buffer, x, y - 3*lineheight, scale);
}
