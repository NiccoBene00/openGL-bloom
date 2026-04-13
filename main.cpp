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
    Shader screenShader("shaders/screen.vert", "shaders/screen.frag");
    Shader blurShader("shaders/screen.vert", "shaders/blur.frag");
    Shader finalShader("shaders/screen.vert", "shaders/bloom_final.frag");

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

    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // texture colore
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // depth + stencil buffer
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // check
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];

    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);

    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);

        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
    }

    // ---------------- RENDER LOOP ----------------
    while (!glfwWindowShouldClose(window))
    {
        // =========================
        // PASS 1: render scena nel framebuffer
        // =========================
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glUniform3f(glGetUniformLocation(shader.ID, "objectColor"), 1.0f, 0.5f, 0.2f);
        //glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), 7.0f, 7.0f, 7.0f);
        float time = glfwGetTime();

        float r = sin(time) * 0.5f + 0.5f;
        float g = sin(time + 2.0f) * 0.5f + 0.5f;
        float b = sin(time + 4.0f) * 0.5f + 0.5f;

        glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), r * 10.0f, g * 10.0f, b * 10.0f);
        //glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), 1.2f, 1.0f, 2.0f);
        float t = glfwGetTime();

        glm::vec3 lightPos = glm::vec3(
            sin(t) * 3.0f,
            1.0f,
            cos(t) * 3.0f
        );

        glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f,
            100.0f);

        shader.setMat4("model", glm::value_ptr(model));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        

        // =========================
        // PASS 2: BLUR
        // =========================
        bool horizontal = true, first_iteration = true;
        int amount = 7;

        blurShader.use();

        for (int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);

            glUniform1i(glGetUniformLocation(blurShader.ID, "horizontal"), horizontal);

            glBindTexture(GL_TEXTURE_2D, first_iteration ? textureColorbuffer : pingpongColorbuffers[!horizontal]);

            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            horizontal = !horizontal;

            if (first_iteration)
                first_iteration = false;
        }


        // =========================
        // PASS 3: OUTPUT
        // =========================
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT);

        finalShader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);

        glUniform1i(glGetUniformLocation(finalShader.ID, "scene"), 0);
        glUniform1i(glGetUniformLocation(finalShader.ID, "bloomBlur"), 1);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}