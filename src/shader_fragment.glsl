#version 330 core

in vec4 position_world;
in vec4 normal;
in vec4 position_model;

in vec2 texcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#define PLANAR_PROJECTION 0
#define SPHERICAL_PROJECTION  1
#define TEXCOORD_PROJECTION  2

uniform int object_id;
uniform int hasTexture;
uniform int mappingType;

uniform vec4 bbox_min;
uniform vec4 bbox_max;

uniform sampler2D tex;

out vec3 color;

#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;


    /* Iluminação */

    /* Espectro da fonte de iluminação */
    vec3 I = vec3(0.94, 0.66, 0.33);
    /* Espectro da luz ambiente */
    vec3 Ia = vec3(0.2, 0.2, 0.2);

    vec4 lightPos = vec4(0.0, 20.0, 1.0, 1.0);
    vec4 lightDir = vec4(0.0, -1.0, 0.0, 0.0);
    vec4 l = normalize(lightPos - p);
    vec4 r = normalize(-l + 2*n*(dot(n, l)));

    //Vetor usado para iluminação de Blinn-Phong
    vec4 h = normalize(l + v);

    if ( mappingType == 1 )
    {
        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

        float rho = length(position_model - bbox_center);
        float theta = atan(position_model.x, position_model.z);
        float phi = asin(position_model.y / rho);

        U = (theta + M_PI) / (2 * M_PI);
        V = (phi + M_PI_2) / M_PI;
    }
    else if ( mappingType == 0 )
    {
        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        // (u, v) = (x, y) normalizado em [0, 1]
        U = (position_model.x - minx) / (maxx - minx);
        V = (position_model.y - miny) / (maxy - miny);
        //V = (position_model.z - minz) / (maxz - minz);
    }
    else if ( mappingType == 2 )
    {
        U = texcoords.x;
        V = texcoords.y;
    }
    else if ( mappingType == 3 )
    {
        // (u, v) = (x, y) normalizado em [0, 1]
        U = (position_model.x) - floor(position_model.x);
        V = (position_model.y) - floor(position_model.y);
        //V = (position_model.z - minz) / (maxz - minz);
    }

    /* Refletância difusa */
    vec3 Kd0 = vec3(0.0, 0.0, 0.0);
    /* Refletancia especular */
    vec3 Ks = vec3(0.2, 0.2, 0.3);
    /* Refletancia ambiente */
    vec3 Ka = vec3(0.4, 0.4, 0.4);
    /* q */
    float q = 20.0;

    if(hasTexture == 1)
        Kd0 = texture(tex, vec2(U,V)).rgb;

    // Equação de Iluminação
    vec3 lambert_diffuse = Kd0 * I * max(0,dot(n,l));
    vec3 ambient = Ka * Ia;
    //vec3 phong = Ks * I * pow(max(0, dot(r, v)), q);
    vec3 blinnPhong = Ks * I * pow(max(0, dot(n, h)), q);

    color = lambert_diffuse + ambient + blinnPhong;

    color = pow(color, vec3(1.0,1.0,1.0)/2.2);
}
