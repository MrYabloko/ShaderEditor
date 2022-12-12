#include "primitives.h"

#define add_face(vertex, normal, vertices, indices) \
{\
    int x, y, z;\
    z = 1;\
    size_t size = vertices.size();\
    x = 1; y = 1; vertices.push_back(Vertex { glm::vec3 vertex, glm::vec3 normal, glm::vec2(1,1) });\
    x = 1; y = -1; vertices.push_back(Vertex { glm::vec3 vertex, glm::vec3 normal, glm::vec2(1,0) });\
    x = -1; y = -1; vertices.push_back(Vertex { glm::vec3 vertex, glm::vec3 normal, glm::vec2(0,0) });\
    x = -1; y = 1; vertices.push_back(Vertex { glm::vec3 vertex, glm::vec3 normal, glm::vec2(0,1) });\
    indices.push_back(size + 0);indices.push_back(size + 1);indices.push_back(size + 3);\
    indices.push_back(size + 1);indices.push_back(size + 2);indices.push_back(size + 3);\
}
void generate_cube(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
    add_face((x,y,z),(0,0,-1),vertices,indices)
    add_face((z,y,x),(-1,0,0),vertices,indices)
    add_face((x,z,y),(0,-1,0),vertices,indices)

    add_face((x,y,-z),(0,0,1),vertices,indices)
    add_face((-z,y,x),(1,0,0),vertices,indices)
    add_face((x,-z,y),(0,1,0),vertices,indices)
}