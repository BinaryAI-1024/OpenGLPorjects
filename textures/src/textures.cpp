#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // 包含 STB 图像加载库，用于加载纹理图像
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

    // 创建 GLFW 窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); 
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl; 
        glfwTerminate(); 
        return -1;
    }
    glfwMakeContextCurrent(window); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 初始化 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout << "Failed to initialize GLAD" << std::endl; 
        return -1;
    }

    Shader ourShader("texture.vs", "texture.fs"); // 创建着色器对象

    // 设置顶点数据（和缓冲区）并配置顶点属性
    //float vertices[] = {
    //    // 位置坐标          // 颜色           // 纹理坐标
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f, // 右上角
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, -1.0f, // 右下角
    //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   -1.0f, -1.0f, // 左下角
    //    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   -1.0f, 2.0f  // 左上角 
    //};
    // OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部
    float vertices[] = {
        // 位置坐标          // 颜色           // 纹理坐标（翻转y轴）
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // 右上角
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // 右下角
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // 左下角
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // 左上角 
    };
    unsigned int indices[] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    // 定义顶点缓冲对象、顶点数组对象和元素缓冲对象
    unsigned int VBO, VAO, EBO; 
    glGenVertexArrays(1, &VAO); // 生成顶点数组对象
    glGenBuffers(1, &VBO);     // 生成顶点缓冲对象
    glGenBuffers(1, &EBO);     // 生成元素缓冲对象

    glBindVertexArray(VAO); // 绑定顶点数组对象

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定顶点缓冲对象
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 将顶点数据复制到缓冲区

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 绑定元素缓冲对象
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // 将索引数据复制到缓冲区

    // 设置顶点属性指针:xyz(顶点坐标)rgb（顶点颜色）st（纹理坐标）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 设置位置属性指针
    glEnableVertexAttribArray(0); // 启用位置属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // 设置颜色属性指针
    glEnableVertexAttribArray(1); // 启用颜色属性
    // 纹理坐标属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // 设置纹理坐标属性指针
    glEnableVertexAttribArray(2); // 启用纹理坐标属性


    // 加载并创建纹理
    unsigned int texture; // 定义纹理对象
    glGenTextures(1, &texture); // 生成纹理对象
    glBindTexture(GL_TEXTURE_2D, texture); // 绑定纹理对象，所有 GL_TEXTURE_2D 操作对该纹理生效
    //  设置 S 轴和T轴的纹理环绕方式
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
         // 如果选择GL_CLAMP_TO_BORDER纹理环绕，需要设置边缘颜色
        float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // 红色
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    }

    if (FILTER_TYPE == 1) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 设置纹理缩小过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 设置纹理放大过滤方式
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
    // 加载图像，创建纹理并生成 Mipmap
    int width, height, nrChannels; // 定义图像宽度、高度和通道数
    std::string imagePath = FileSystem::getPath("resources/grid2.jpg");
    std::cout << "Attempting to load texture from: " << imagePath << std::endl; 
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0); // 加载图像数据
    if (data)
    {
        // 将图像数据上传到纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
        glGenerateMipmap(GL_TEXTURE_2D); // 生成 Mipmap
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl; 
    }
    stbi_image_free(data); // 释放图像数据

    // 渲染循环
    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window); 

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT); // 清空颜色缓冲

        // 绑定纹理对象  在调用glDrawElements之前绑定纹理，它会自动把纹理赋值给片段着色器的采样器sampler2D
        glBindTexture(GL_TEXTURE_2D, texture); 

        // 渲染容器
        ourShader.use(); // 使用着色器程序
        glBindVertexArray(VAO); // 绑定顶点数组对象
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制三角形

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
