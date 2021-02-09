#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "../header/Shader.h"

#include <string>
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path; // we store the path of the texture to compare with other textures
};

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

#endif

