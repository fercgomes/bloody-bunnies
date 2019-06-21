#version 330 core

in vec3 vertex_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;

#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
    color = vertex_color;

    color = pow(color, vec3(1.0,1.0,1.0)/2.2);
}
