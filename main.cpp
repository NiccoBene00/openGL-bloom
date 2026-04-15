#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// dimensioni finestra
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;


// callback resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

glm::vec3 cameraPos   = glm::vec3(0.0f, 1.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

float cameraSpeed = 0.05f;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
        // positions          // normals           // texcoords

        // back face (-Z)
        -0.5f,-0.5f,-0.5f,   0.0f, 0.0f,-1.0f,    0.0f,0.0f,
        0.5f,-0.5f,-0.5f,   0.0f, 0.0f,-1.0f,    1.0f,0.0f,
        0.5f, 0.5f,-0.5f,   0.0f, 0.0f,-1.0f,    1.0f,1.0f,
        0.5f, 0.5f,-0.5f,   0.0f, 0.0f,-1.0f,    1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,   0.0f, 0.0f,-1.0f,    0.0f,1.0f,
        -0.5f,-0.5f,-0.5f,   0.0f, 0.0f,-1.0f,    0.0f,0.0f,

        // front face (+Z)
        -0.5f,-0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    0.0f,0.0f,
        0.5f,-0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    1.0f,0.0f,
        0.5f, 0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    1.0f,1.0f,
        0.5f, 0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    1.0f,1.0f,
        -0.5f, 0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    0.0f,1.0f,
        -0.5f,-0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    0.0f,0.0f,

        // left face (-X)
        -0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,    1.0f,0.0f,
        -0.5f, 0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,    1.0f,1.0f,
        -0.5f,-0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,    0.0f,1.0f,
        -0.5f,-0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,    0.0f,1.0f,
        -0.5f,-0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,    0.0f,0.0f,
        -0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,    1.0f,0.0f,

        // right face (+X)
        0.5f, 0.5f, 0.5f,   1.0f, 0.0f, 0.0f,    1.0f,0.0f,
        0.5f, 0.5f,-0.5f,   1.0f, 0.0f, 0.0f,    1.0f,1.0f,
        0.5f,-0.5f,-0.5f,   1.0f, 0.0f, 0.0f,    0.0f,1.0f,
        0.5f,-0.5f,-0.5f,   1.0f, 0.0f, 0.0f,    0.0f,1.0f,
        0.5f,-0.5f, 0.5f,   1.0f, 0.0f, 0.0f,    0.0f,0.0f,
        0.5f, 0.5f, 0.5f,   1.0f, 0.0f, 0.0f,    1.0f,0.0f,

        // bottom face (-Y)
        -0.5f,-0.5f,-0.5f,   0.0f,-1.0f, 0.0f,    0.0f,1.0f,
        0.5f,-0.5f,-0.5f,   0.0f,-1.0f, 0.0f,    1.0f,1.0f,
        0.5f,-0.5f, 0.5f,   0.0f,-1.0f, 0.0f,    1.0f,0.0f,
        0.5f,-0.5f, 0.5f,   0.0f,-1.0f, 0.0f,    1.0f,0.0f,
        -0.5f,-0.5f, 0.5f,   0.0f,-1.0f, 0.0f,    0.0f,0.0f,
        -0.5f,-0.5f,-0.5f,   0.0f,-1.0f, 0.0f,    0.0f,1.0f,

        // top face (+Y)
        -0.5f, 0.5f,-0.5f,   0.0f, 1.0f, 0.0f,    0.0f,1.0f,
        0.5f, 0.5f,-0.5f,   0.0f, 1.0f, 0.0f,    1.0f,1.0f,
        0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,    1.0f,0.0f,
        0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,    1.0f,0.0f,
        -0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,    0.0f,0.0f,
        -0.5f, 0.5f,-0.5f,   0.0f, 1.0f, 0.0f,    0.0f,1.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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
    unsigned int colorBuffers[2];
    glGenTextures(2, colorBuffers);

    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
            SCR_WIDTH, SCR_HEIGHT,
            0, GL_RGBA, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i,
            GL_TEXTURE_2D,
            colorBuffers[i],
            0);
    }

    unsigned int attachments[2] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1
    };
    glDrawBuffers(2, attachments);
    

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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];

    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);

    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);

        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //------TEXTURE LOADING------
    int width, height, nrChannels;
    unsigned char *data = stbi_load("resources/textures/chip.jpg", &width, &height, &nrChannels, 0);

        if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture\n";
    }
    stbi_image_free(data);

    //--------------------------


    // ---------------- RENDER LOOP ----------------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        // =========================
        // PASS 1: render scena nel framebuffer
        // =========================
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glUniform1i(glGetUniformLocation(shader.ID, "isEmissive"), 0);

        glUniform3f(glGetUniformLocation(shader.ID, "viewPos"),
            cameraPos.x, cameraPos.y, cameraPos.z);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);


        glUniform3f(glGetUniformLocation(shader.ID, "objectColor"), 0.1f, 0.1f, 0.1f);
        //glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), 7.0f, 7.0f, 7.0f);
        float time = glfwGetTime();

        float r = sin(time) * 0.5f + 0.5f;
        float g = sin(time + 2.0f) * 0.5f + 0.5f;
        float b = sin(time + 4.0f) * 0.5f + 0.5f;
        //glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), 4.0f, 3.0f, 2.0f);
        glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), 0.2f, 0.6f, 1.0f);
        //glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), r *7.0f, g * 7.0f, b * 7.0f);
        //glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), 1.2f, 1.0f, 2.0f);
        float t = glfwGetTime();

        glm::vec3 lightPos = glm::vec3(
            sin(t) * 3.0f,
            2.0f,
            cos(t) * 3.0f
        );

        glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));

        //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -3.0f));
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f,
            100.0f);

        shader.setMat4("model", glm::value_ptr(model));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(VAO);

        // pavimento a griglia
        for (int x = -5; x < 5; x++)
        {
            for (int z = -30; z < 30; z++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x * 0.6f, -1.0f, z * 0.6f));
                model = glm::scale(model, glm::vec3(0.25f));

                shader.setMat4("model", glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        // pareti laterali
        for (int y = 0; y < 6; y++)
        {
            for (int z = -30; z < 30; z++)
            {
                // =========================
                // LATO SINISTRO
                // =========================
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-3.0f, y * 0.6f, z * 0.6f));
                model = glm::scale(model, glm::vec3(0.25f));

                //STRIP NEON (solo alcune righe)
                if ((y == 2 || y == 4) && (z % 4 == 0))
                    glUniform1i(glGetUniformLocation(shader.ID, "isEmissive"), 1);
                else
                    glUniform1i(glGetUniformLocation(shader.ID, "isEmissive"), 0);

                shader.setMat4("model", glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);

                // =========================
                // LATO DESTRO
                // =========================
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(3.0f, y * 0.6f, z * 0.6f));
                model = glm::scale(model, glm::vec3(0.25f));

                // STESSA LOGICA NEON
                if ((y == 2 || y == 4) && (z % 4 == 0))
                    glUniform1i(glGetUniformLocation(shader.ID, "isEmissive"), 1);
                else
                    glUniform1i(glGetUniformLocation(shader.ID, "isEmissive"), 0);

                shader.setMat4("model", glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        // cubo centrale
        glUniform1i(glGetUniformLocation(shader.ID, "isEmissive"), 1);

        // cubo centrale
        glm::mat4 centerModel = glm::mat4(1.0f);
        centerModel = glm::translate(centerModel, glm::vec3(0.0f, 1.0f, 0.0f));
        centerModel = glm::rotate(centerModel, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        //centerModel = glm::scale(centerModel, glm::vec3(1.4f));
        float pulse = sin(glfwGetTime() * 2.0f) * 0.3f + 1.5f;
        centerModel = glm::scale(centerModel, glm::vec3(pulse));

        shader.setMat4("model", glm::value_ptr(centerModel));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // DISEGNA LA LUCE (debug visivo)
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));

        glUniform1i(glGetUniformLocation(shader.ID, "isEmissive"), 1);
        shader.setMat4("model", glm::value_ptr(lightModel));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUniform1i(glGetUniformLocation(shader.ID, "isEmissive"), 0);

        // RESET (importantissimo)
        glUniform1i(glGetUniformLocation(shader.ID, "isEmissive"), 0);

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

            glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);

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
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glDisable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT);

        finalShader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);

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