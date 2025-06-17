#include <glad/glad.h>     
#include <GLFW/glfw3.h>   
#include <shader_s.h>     
#include <iostream>    

// ��������
void framebuffer_size_callback(GLFWwindow* window, int width, int height); // ���ڴ�С�仯ʱ�Ļص�����
void processInput(GLFWwindow* window); // �����û�����

// ���ô��ڵĿ�Ⱥ͸߶�
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // ��ʼ��GLFW
    glfwInit();
    // ����GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // ����OpenGL�����汾��Ϊ3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // ����OpenGL�Ĵΰ汾��Ϊ3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ʹ�ú���ģʽ

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // ��macOS��������ǰ����
#endif

    // ����GLFW����
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // ������ڴ���ʧ�ܣ���ֹGLFW
        return -1;
    }
    glfwMakeContextCurrent(window); // �����ڵ�����������Ϊ��ǰ�̵߳�������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // ���ô��ڴ�С�仯�Ļص�����

    // ��ʼ��GLAD������OpenGL����ָ��
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ������������������ɫ������
    Shader ourShader("shader.vs", "shader.fs"); // ʹ��ָ���Ķ�����ɫ����Ƭ����ɫ���ļ�������ɫ������

    // ���ö������ݣ��ͻ������������ö�������
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // ���½ǣ���ɫ
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // ���½ǣ���ɫ
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // ��������ɫ
    };

    unsigned int VBO, VAO; // ���嶥�㻺�����Ͷ����������
    glGenVertexArrays(1, &VAO); // ����һ�������������
    glGenBuffers(1, &VBO); // ����һ�����㻺�����
    // �󶨶����������Ȼ��󶨺����ö��㻺�壬������ö�������
    glBindVertexArray(VAO); // �󶨶����������

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // �󶨶��㻺�����
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // ���������ݸ��Ƶ�������

    // λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // ָ���������Եĸ�ʽ
    glEnableVertexAttribArray(0); // ���ö�����������
    // ��ɫ����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // ָ����ɫ���Եĸ�ʽ
    glEnableVertexAttribArray(1); // ������ɫ��������

    // ͨ������Ҫ��󶥵����������Ϊ�޸����������������ʱ��Ҫ�ٴΰ�
    // glBindVertexArray(0);

    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        // ��������
        processInput(window);

        // ��Ⱦ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // ���������ɫ
        glClear(GL_COLOR_BUFFER_BIT); // �����ɫ����

        // ��Ⱦ������
        ourShader.use(); //  ������ʹ����ɫ������
        glBindVertexArray(VAO); // �󶨶����������
        glDrawArrays(GL_TRIANGLES, 0, 3); // ����������

        // �������岢��ѯIO�¼�
        glfwSwapBuffers(window); // ������ɫ����
        glfwPollEvents(); // ��鲢�����¼�
    }

    // ��ѡ���ͷ�������Դ
    glDeleteVertexArrays(1, &VAO); // ɾ�������������
    glDeleteBuffers(1, &VBO); // ɾ�����㻺�����

    // ��ֹGLFW���������GLFW��Դ
    glfwTerminate();
    return 0;
}

// �������룺��ѯGLFW�Ƿ��а���������/�ͷţ���ִ����Ӧ�Ĳ���
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // �������ESC��
        glfwSetWindowShouldClose(window, true); // �رմ���
}

// ���ڴ�С�仯ʱ�Ļص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); // �����ӿڴ�С��ƥ�䴰�ڵ��³ߴ�
}
