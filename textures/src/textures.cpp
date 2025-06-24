#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // ���� STB ͼ����ؿ⣬���ڼ�������ͼ��
#include <filesystem.h> 
#include <shader_s.h>   
#include <iostream>     


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int WRAP_TYPE = 1;
const unsigned int FILTER_TYPE = 2;

int main()
{
    glfwInit(); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // ���� GLFW ����
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); 
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl; 
        glfwTerminate(); 
        return -1;
    }
    glfwMakeContextCurrent(window); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ��ʼ�� GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout << "Failed to initialize GLAD" << std::endl; 
        return -1;
    }

    Shader ourShader("texture.vs", "texture.fs"); // ������ɫ������

    // ���ö������ݣ��ͻ������������ö�������
    //float vertices[] = {
    //    // λ������          // ��ɫ           // ��������
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f, // ���Ͻ�
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, -1.0f, // ���½�
    //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   -1.0f, -1.0f, // ���½�
    //    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   -1.0f, 2.0f  // ���Ͻ� 
    //};
    float vertices[] = {
        // λ������          // ��ɫ           // �������꣨��תy�ᣩ
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // ���Ͻ�
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // ���½�
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // ���½�
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // ���Ͻ� 
    };
    unsigned int indices[] = {
        0, 1, 3, // ��һ��������
        1, 2, 3  // �ڶ���������
    };
    // ���嶥�㻺����󡢶�����������Ԫ�ػ������
    unsigned int VBO, VAO, EBO; 
    glGenVertexArrays(1, &VAO); // ���ɶ����������
    glGenBuffers(1, &VBO);     // ���ɶ��㻺�����
    glGenBuffers(1, &EBO);     // ����Ԫ�ػ������

    glBindVertexArray(VAO); // �󶨶����������

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // �󶨶��㻺�����
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // ���������ݸ��Ƶ�������

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // ��Ԫ�ػ������
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // ���������ݸ��Ƶ�������

    // ���ö�������ָ��:xyz(��������)rgb��������ɫ��st���������꣩
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // ����λ������ָ��
    glEnableVertexAttribArray(0); // ����λ������
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // ������ɫ����ָ��
    glEnableVertexAttribArray(1); // ������ɫ����
    // ������������
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // ����������������ָ��
    glEnableVertexAttribArray(2); // ����������������


    // ���ز���������
    unsigned int texture; // �����������
    glGenTextures(1, &texture); // �����������
    glBindTexture(GL_TEXTURE_2D, texture); // ������������� GL_TEXTURE_2D �����Ը�������Ч
    //  ���� S ���T��������Ʒ�ʽ
    if (WRAP_TYPE == 1) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else if (WRAP_TYPE == 2) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }
    else if (WRAP_TYPE == 3) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else if (WRAP_TYPE == 4) {
         // ���ѡ��GL_CLAMP_TO_BORDER�����ƣ���Ҫ���ñ�Ե��ɫ
        float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // ��ɫ
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    }

    if (FILTER_TYPE == 1) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // ����������С���˷�ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // ��������Ŵ���˷�ʽ
    }
    else if (FILTER_TYPE == 2) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    }
    else if (FILTER_TYPE == 3) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else if (FILTER_TYPE == 4) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else if (FILTER_TYPE == 5) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else if (FILTER_TYPE == 6) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    // ����ͼ�񣬴����������� Mipmap
    int width, height, nrChannels; // ����ͼ���ȡ��߶Ⱥ�ͨ����
    std::string imagePath = FileSystem::getPath("resources/grid2.jpg");
    std::cout << "Attempting to load texture from: " << imagePath << std::endl; 
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0); // ����ͼ������
    if (data)
    {
        // ��ͼ�������ϴ�������
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
        glGenerateMipmap(GL_TEXTURE_2D); // ���� Mipmap
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl; 
    }
    stbi_image_free(data); // �ͷ�ͼ������

    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window); 

        // ��Ⱦ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // ����������ɫ
        glClear(GL_COLOR_BUFFER_BIT); // �����ɫ����

        // ���������
        glBindTexture(GL_TEXTURE_2D, texture); 

        // ��Ⱦ����
        ourShader.use(); // ʹ����ɫ������
        glBindVertexArray(VAO); // �󶨶����������
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // ����������

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);     
    glDeleteBuffers(1, &EBO);    


    glfwTerminate(); 
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true); 
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); 
}