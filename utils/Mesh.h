#ifndef SHADEREDITOR_MESH_H
#define SHADEREDITOR_MESH_H

#include "Vertex.h"
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO, VBO, EBO;
    void setupMesh();
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Mesh();
    void draw();
};


#endif
