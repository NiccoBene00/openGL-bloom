#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/shader.h"

// dimensioni finestra
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// callback resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // ---------------- INIT GLFW ----------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bloom Project", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ---------------- INIT GLAD ----------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // ---------------- SHADER ----------------
    Shader shader("shaders/basic.vert", "shaders/basic.frag");

    // ---------------- VERTEX DATA ----------------
    float vertices[] = {
        // positions        // normals
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,
        0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,
        0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,
        0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,
        -0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,

        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,1.0f,
        0.5f,-0.5f, 0.5f,  0.0f,0.0f,1.0f,
        0.5f, 0.5f, 0.5f,  0.0f,0.0f,1.0f,
        0.5f, 0.5f, 0.5f,  0.0f,0.0f,1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f,0.0f,1.0f,
        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f,
        -0.5f, 0.5f,-0.5f, -1.0f,0.0f,0.0f,
        -0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,
        -0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,
        -0.5f,-0.5f, 0.5f, -1.0f,0.0f,0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f,

        0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f,
        0.5f, 0.5f,-0.5f,  1.0f,0.0f,0.0f,
        0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,
        0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,
        0.5f,-0.5f, 0.5f,  1.0f,0.0f,0.0f,
        0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f,

        -0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,
        0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,
        0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,
        0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,
        -0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,
        -0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,

        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,0.0f,
        0.5f, 0.5f,-0.5f,  0.0f,1.0f,0.0f,
        0.5f, 0.5f, 0.5f,  0.0f,1.0f,0.0f,
        0.5f, 0.5f, 0.5f,  0.0f,1.0f,0.0f,
        -0.5f, 0.5f, 0.5f,  0.0f,1.0f,0.0f,
        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,0.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ---------------- RENDER LOOP ----------------
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        
        glUniform3f(glGetUniformLocation(shader.ID, "objectColor"), 1.0f, 0.5f, 0.2f);
        glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), 1.2f, 1.0f, 2.0f);

        // MODEL
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));

        // VIEW (camera indietro)
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

        // PROJECTION (prospettiva)
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 
            0.1f, 
            100.0f);

        // manda agli shader
        shader.setMat4("model", glm::value_ptr(model));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}