// The include file for GLAD includes the required OpenGL headers (like GL/gl.h) 
// so be sure to include GLAD before other header files that require OpenGL (like GLFW). 
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Shader source codes
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

// Settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;



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


    // VERTEX DATA (copy in vertex buffer, then processing method)

    // Triangle in XY space
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
          .5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Generate 1 vertex array and 1 vertex buffer, each with a unique ID 
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer. The VAO will save the config of the VBO.
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy vertex data in buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell OpenGL how the vertex data should be processed
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable the position attribute of the vertex
    glad_glEnableVertexAttribArray(0);


    // VERTEX SHADER

    // Create unique ID for the shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach shader source code to shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check if shader compilation worked
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // FRAGMENT SHADER

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check if shader compilation worked
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // LINK SHADERS WITH SHADER-PROGRAM OBJECT

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check if shader linking worked
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

    // Delete shaders because they are not needed anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Clear the frame buffer and apply new colour to window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader-program object
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front (img displayed on screen) and back (img being rendered) buffers to render img without flickering effect
        glfwSwapBuffers(window);
        // Check for events (keyboard, mouse etc), updates window state, calls corresponding functions (which we can register via callback methods) 
        glfwPollEvents();
    }

    // Delete all of GLFW's resources that were allocated
    glfwTerminate();

    return 0;
}



void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Callback fct that is called every time the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}