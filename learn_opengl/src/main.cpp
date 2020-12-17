// The include file for GLAD includes the required OpenGL headers (like GL/gl.h) 
// so be sure to include GLAD before other header files that require OpenGL (like GLFW). 
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// CAUTION (GLM has a column-major convention): (COL, ROW) and not (row, col) as usual
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "../header/Shader.h"
#include "../header/Camera.h"

// ----- CONSTANTS

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;

const char* PATH_COLOR_VS = "1.colors.vs";
const char* PATH_COLOR_FS = "1.colors.fs";
const char* PATH_LIGHT_CUBE_VS = "1.light_cube.vs";
const char* PATH_LIGHT_CUBE_FS = "1.light_cube.fs";

// ----- CALLBACKS

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// ----- CAMERA

Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));

// ----- MOUSE

// Last XY positions of the mouse, init to be at the centre of the window
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// ----- TIMING (to have the same camera speed regardless of the computer the code is run on)

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ----- LIGHT

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


int main()
{
    //  ----- WINDOW 

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Use core profile to get access to a smaller subset of OpenGL features without 
    // backwards-compatible features we no longer need
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the context of the window the main context of the current thread
    glfwMakeContextCurrent(window);

    // ----- CALLBACKS

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // ----- MOUSE CAPTURE

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // ----- GLAD: load OpenGL function pointers

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // ----- Z BUFFER 
    
    glEnable(GL_DEPTH_TEST);

    // ----- SHADER PROGRAMS (build and compile)

    // Files are written by default in the dir containing "srd" and "header"
    Shader lightingShader(PATH_COLOR_VS, PATH_COLOR_FS);
    Shader lightCubeShader(PATH_LIGHT_CUBE_VS, PATH_LIGHT_CUBE_FS);    

    // ----- VERTEX DATA

    // Vertices representing a cube, with position and normal vector attributes
    // Col 0 -> 2 = vertex position
    // Col 3 -> 5 = normal vector for the cube face the vertex belongs to
    // To render a cube, we need 36 vertices (6 faces * 2 triangles * 3 vertices each)
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // ----- VAO AND VBO for cube container object 

    // 1/ Unique IDs for VBO and VAO
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    // 2/ Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 3/ Create and init a buffer object's data store
    // STATIC_DRAW: the data store contents will be modified once and used many times
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 4/ Bind VAO
    // Bind Vertex Array Object 1st. Any VBO, EBO or calls to glVertexAttribPointer and glEnableVertexAttribArray 
    // will be stored in the currently-bound VAO
    glBindVertexArray(cubeVAO);

    // 5/ Structure of the vertex attribute(s) (vertex position and normal here)
    // from docs.GL: define an array of generic vertex attribute data
    // Tell OpenGL how the vertex attributes are stored in one vertex
    // glVertexAttribPointer(attributePos, nbChannelsInAttribute, dataType, shouldDataBeNormalised, strideAttribLength, offsetWhereAttribBegins)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    // 6/ Enable attribute(s)
    // Enable position and normal attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // ----- LIGHT CUBE

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    // We only need to bind to the VBO, the cube's VBO's data already contains the data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Set the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // Enable position attribute (no need to use the normal attribute for the light cube)
    glEnableVertexAttribArray(0);

    // ----- RENDER LOOP

    while (!glfwWindowShouldClose(window))
    {
        // ---- TIMING

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // ----- WINDOW

        processInput(window);

        // Clear the frame and depth buffers and apply new color to window
        // The depth buffer (z-buffer) contains the depth (z coord) of each fragment
        // The z-buffer should be cleared at each new render. Depths are considered only for the current frame.
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ----- SHADER PROGRAM COLORED CUBE

        lightingShader.use();

        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // LIGHT properties
        // Make light color change over time
        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);  // Decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);// Low influence

        lightingShader.setVec3("light.ambient", ambientColor);
        lightingShader.setVec3("light.diffuse", diffuseColor);  // Darken diffuse light a bit
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // MATERIAL properties
        lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        lightingShader.setFloat("material.shininess", 32.0f);

        // ----- TRANSFORMS

        // PROJECTION and VIEW matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_PLANE, FAR_PLANE);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // MODEL matrix (used in the draw call)
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // ----- DRAW COLORED CUBE

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ----- DRAW LIGHT CUBE

        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        // Translate the light source cube to the light source's position and scale it down before rendering it
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);

        // A smaller cube
        model = glm::scale(model, glm::vec3(1.0f)); 
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Swap front (img displayed on screen) and back (img being rendered) buffers to render img without flickering effect
        glfwSwapBuffers(window);
        // Check for events (keyboard, mouse etc), updates window state, calls corresponding functions 
        // (which we can register via callback methods) 
        glfwPollEvents();
    }

    // De-allocate resources
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    glfwSwapBuffers(window);
    glfwPollEvents();

    // ----- FREE MEMORY

    // Delete all of GLFW's resources that were allocated
    glfwTerminate();

    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

// Callback fct that is called every time the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = (float)(xpos - lastX);
    float yoffset = (float)(lastY - ypos); // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)yoffset);
}