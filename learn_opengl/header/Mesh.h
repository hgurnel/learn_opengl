#ifndef MESH_H
#define MESH_H

// Local headers
#include "../header/Shader.h"
#include "../header/Vertex.h"
#include "../header/Texture.h"
// Third-party headers
#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Standard library headers
#include <string>
#include <vector>

class Mesh
{
public:

    // Ctor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    // Mesh data
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;

    void draw(Shader& shader);

private:
    //  render data
    unsigned int m_VAO, m_VBO, m_EBO;

    void setupMesh();
};

#endif  // MESH_H

