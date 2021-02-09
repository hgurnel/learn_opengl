// CAUTION: the include file for GLAD includes the required OpenGL headers (like GL/gl.h) 
// so be sure to include GLAD before other header files that require OpenGL (like GLFW). 
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../header/stb_image.h"
// CAUTION (GLM has a column-major convention): (COL, ROW) and not (row, col) as usual
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "../header/Shader.h"
#include "../header/Camera.h"
#include "../header/Model.h"

// ----- CONSTANTS

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;

const char* PATH_COLOR_VS = "1.colors.vs";
const char* PATH_COLOR_FS = "1.colors.fs";
const char* PATH_LIGHT_CUBE_VS = "1.light_cube.vs";
const char* PATH_LIGHT_CUBE_FS = "1.light_cube.fs";
const char* PATH_TEXTURE_DIFFUSE = "../textures/container2_diffuse_map.png";
const char* PATH_TEXTURE_SPECULAR = "../textures/container2_specular_map.png";
const char* PATH_TEXTURE_EMISSIVE = "../textures/container2_emissive_map.jpg";
//const char* PATH_MODEL = "../model/backpack/backpack.obj";
//const char* PATH_MODEL = "C://Users//Aero//Desktop//OneDrive//Documents//dev//c++//EPFL//learn_opengl//model//backpack//backpack.obj";
const char* PATH_MODEL = "C:/Users/Aero/Desktop/OneDrive/Documents/dev/c++/EPFL/learn_opengl/model/backpack/backpack.obj";

// ----- CALLBACKS & FUNCTIONS

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

unsigned int loadTexture(const char* path);

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


int main()
{
    //  ----- WINDOW

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Use core profile to have access to a smaller subset of OpenGL features without 
    // backwards-compatible features we no longer need
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window object creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // ----- OPENGL CONTEXT

    // Make the context of the window the main context of the current thread
    glfwMakeContextCurrent(window);

    // ----- CALLBACKS

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // ----- MOUSE CAPTURE

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // ----- GLAD: load the OpenGL function pointers before calling any OpenGl function
    // GLFW gives us glfwGetProcAddress that defines the correct function based on 
    // which OS we're compiling for. 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // ----- Z BUFFER 
    
    glEnable(GL_DEPTH_TEST);

    // ----- SHADER PROGRAMS (build and compile)

    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

    // ----- MODEL LOADING

    Model ourModel(PATH_MODEL);
    

    // ----- RENDER LOOP

    while (!glfwWindowShouldClose(window))
    {
        // ----- TIMING

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

        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.draw(ourShader);

        // Swap front (img displayed on screen) and back (img being rendered) buffers to render img without flickering effect
        glfwSwapBuffers(window);
        // Check for events (keyboard, mouse etc), updates window state, calls corresponding functions 
        // (which we can register via callback methods) 
        glfwPollEvents();
    }

    // glfwPollEvents() checks if any events are triggered (like keyboard input or mouse movement events), 
    // updates the window state, and calls the corresponding functions (which we can register via callback methods)
    // glfwSwapBuffers(window) swaps the color buffer (a large 2D buffer that contains color values for each pixel 
    // in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.
    // Swap between front and back buffers if double buffering is enabled (to avoid image flickering).
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

// Callback fct that resizes the viewport every time the window is resized. 
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

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}