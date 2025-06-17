#include <glad/glad.h>     
#include <GLFW/glfw3.h>   
#include <shader_s.h>     
#include <iostream>    

// 函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height); // 窗口大小变化时的回调函数
void processInput(GLFWwindow* window); // 处理用户输入

// 设置窗口的宽度和高度
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // 初始化GLFW
    glfwInit();
    // 配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 设置OpenGL的主版本号为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 设置OpenGL的次版本号为3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 在macOS上启用向前兼容
#endif

    // 创建GLFW窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // 如果窗口创建失败，终止GLFW
        return -1;
    }
    glfwMakeContextCurrent(window); // 将窗口的上下文设置为当前线程的上下文
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 设置窗口大小变化的回调函数

    // 初始化GLAD，加载OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ！！！创建并编译着色器程序
    Shader ourShader("shader.vs", "shader.fs"); // 使用指定的顶点着色器和片段着色器文件创建着色器对象

    // 设置顶点数据（和缓冲区）并配置顶点属性
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // 右下角，红色
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // 左下角，绿色
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // 顶部，蓝色
    };

    unsigned int VBO, VAO; // 定义顶点缓冲对象和顶点数组对象
    glGenVertexArrays(1, &VAO); // 生成一个顶点数组对象
    glGenBuffers(1, &VBO); // 生成一个顶点缓冲对象
    // 绑定顶点数组对象，然后绑定和设置顶点缓冲，最后配置顶点属性
    glBindVertexArray(VAO); // 绑定顶点数组对象

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定顶点缓冲对象
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 将顶点数据复制到缓冲中

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // 指定顶点属性的格式
    glEnableVertexAttribArray(0); // 启用顶点属性数组
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // 指定颜色属性的格式
    glEnableVertexAttribArray(1); // 启用颜色属性数组

    // 通常不需要解绑顶点数组对象，因为修改其他顶点数组对象时需要再次绑定
    // glBindVertexArray(0);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清除颜色
        glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲

        // 渲染三角形
        ourShader.use(); //  ！！！使用着色器程序
        glBindVertexArray(VAO); // 绑定顶点数组对象
        glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形

        // 交换缓冲并查询IO事件
        glfwSwapBuffers(window); // 交换颜色缓冲
        glfwPollEvents(); // 检查并调用事件
    }

    // 可选：释放所有资源
    glDeleteVertexArrays(1, &VAO); // 删除顶点数组对象
    glDeleteBuffers(1, &VBO); // 删除顶点缓冲对象

    // 终止GLFW，清除所有GLFW资源
    glfwTerminate();
    return 0;
}

// 处理输入：查询GLFW是否有按键被按下/释放，并执行相应的操作
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 如果按下ESC键
        glfwSetWindowShouldClose(window, true); // 关闭窗口
}

// 窗口大小变化时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); // 调整视口大小以匹配窗口的新尺寸
}
