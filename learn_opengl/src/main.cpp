// The include file for GLAD includes the required OpenGL headers (like GL/gl.h) 
// so be sure to include GLAD before other header files that require OpenGL (like GLFW). 
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../header/stb_image.h"

// CAUTION (GLM has a coluém-major convention): (COL, ROW) and not (row, col) as usual
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../header/Shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


// Settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// Percentage of mix between the two textures
float mixPercent = 0.0;


int main()
{
    // Window init 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Use core profile to get access to a smaller subset of OpenGL features without backwards-compatible features we no longer need
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window creation
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the context of the window the main context of the current thread
    glfwMakeContextCurrent(window);
    // Register the resizing callback fct in GLFW
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL fct ptrs
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // SHADER PROGRAM
    Shader ourShader("shader.vs", "shader.fs");

    // VERTEX DATA (copy in vertex buffer, then processing method)

// Vertices representing a rectangle 
    float vertices[] = {
        // positions          // colors           // texture coords (note that we changed them to 'zoom in' on our texture image)
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[] =
    {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    // Generate 1 vertex array, 1 vertex buffer and 1 element buffer, each with a unique ID 
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first
    glBindVertexArray(VAO);

    // Copy vertex data in buffer's memory. The VAO will save the config of the bound VBO.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Tell OpenGL how the vertex data should be processed
    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);
    // Color attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glad_glEnableVertexAttribArray(1);
    // Texture attribute (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glad_glEnableVertexAttribArray(2);

    // TEXTURE 1

    // Generate a texture object and bind it
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // Texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load image 1
    int width, height, nrChannels;
    unsigned char* data = stbi_load("../textures/container.jpg", &width, &height, &nrChannels, 0);
    
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    
    // Free image memory
    stbi_image_free(data);

    // TEXTURE 2

    // Generate a texture object and bind it
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image 2
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../textures/awesomeface.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        // Diff w/ image1 (jpg): image2 = png so it contains an alpha channel, so we use the GL_RGBA option now
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // Free image memory
    stbi_image_free(data);

    ourShader.use();
    // Tell OpenGL to which texture unit each shader sampler belongs to
    glUniform1i(glGetUniformLocation(ourShader.m_ID, "fsTexture1"), 0); // manual setting
    ourShader.setInt("fsTexture2", 1); // setting through shader class


    // RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Clear the frame buffer and apply new color to window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TRANSFORM

        // CAUTION: the first transfo to be applied is the last one, so here it is the rotation
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));

        // Activate texture units and bing corresponding texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();

        // Change percentage of mix between the two texture
        ourShader.setFloat("mixPercentage", mixPercent);

        // Apply transform to rectangle and smiley
        unsigned int transformLoc = glGetUniformLocation(ourShader.m_ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap front (img displayed on screen) and back (img being rendered) buffers to render img without flickering effect
        glfwSwapBuffers(window);
        // Check for events (keyboard, mouse etc), updates window state, calls corresponding functions (which we can register via callback methods) 
        glfwPollEvents();
    }

    // Optional: de-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Delete all of GLFW's resources that were allocated
    glfwTerminate();

    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixPercent += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixPercent >= 1.0f)
            mixPercent = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixPercent -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixPercent <= 0.0f)
            mixPercent = 0.0f;
    }
}

// Callback fct that is called every time the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}