// shader_s.h
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID; // 存储着色器程序的唯一标识符
    // 参数 vertexPath - 顶点着色器文件的路径
    // 参数 fragmentPath - 片段着色器文件的路径
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. 从文件路径中获取顶点/片段着色器源代码
        std::string vertexCode; // 存储顶点着色器源代码的字符串
        std::string fragmentCode; // 存储片段着色器源代码的字符串
        std::ifstream vShaderFile; // 用于读取顶点着色器文件的输入流
        std::ifstream fShaderFile; // 用于读取片段着色器文件的输入流
        // 确保ifstream对象可以抛出异常：
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // 打开文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // 将文件内容读入流中
            vShaderStream << vShaderFile.rdbuf(); // 读取顶点着色器文件内容
            fShaderStream << fShaderFile.rdbuf(); // 读取片段着色器文件内容
            // 关闭文件句柄
            vShaderFile.close();
            fShaderFile.close();
            // 将流转换为字符串
            vertexCode = vShaderStream.str(); // 将顶点着色器流转换为字符串
            fragmentCode = fShaderStream.str(); // 将片段着色器流转换为字符串
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str(); // 转换为C风格字符串的顶点着色器代码
        const char* fShaderCode = fragmentCode.c_str(); // 转换为C风格字符串的片段着色器代码
        // 2. 编译着色器
        unsigned int vertex, fragment;
        // 顶点着色器
        vertex = glCreateShader(GL_VERTEX_SHADER); // 创建顶点着色器对象
        glShaderSource(vertex, 1, &vShaderCode, NULL); // 将顶点着色器源代码附加到着色器对象
        glCompileShader(vertex); // 编译顶点着色器
        checkCompileErrors(vertex, "VERTEX"); // 检查顶点着色器编译错误
        // 片段着色器
        fragment = glCreateShader(GL_FRAGMENT_SHADER); // 创建片段着色器对象
        glShaderSource(fragment, 1, &fShaderCode, NULL); // 将片段着色器源代码附加到着色器对象
        glCompileShader(fragment); // 编译片段着色器
        checkCompileErrors(fragment, "FRAGMENT"); // 检查片段着色器编译错误
        // 着色器程序
        ID = glCreateProgram(); // 创建着色器程序对象
        glAttachShader(ID, vertex); // 将顶点着色器附加到程序
        glAttachShader(ID, fragment); // 将片段着色器附加到程序
        glLinkProgram(ID); // 链接着色器程序
        checkCompileErrors(ID, "PROGRAM"); // 检查着色器程序链接错误
        // 删除着色器，因为它们已经链接到我们的程序中，不再需要
        glDeleteShader(vertex); // 删除顶点着色器对象
        glDeleteShader(fragment); // 删除片段着色器对象
    }

    // 激活着色器
    // 使用当前着色器程序
    void use()
    {
        glUseProgram(ID); // 激活着色器程序
    }
    // setVec4
    // 参数 name - uniform变量的名称
    // 参数 v0, v1, v2, v3 - 要设置的四个浮点数值
    // 功能：设置着色器程序中名为name的vec4类型uniform变量的值
    void setVec4(const std::string& name, float v0, float v1, float v2, float v3) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3); // 设置uniform vec4值
    }

    // utility uniform functions

    // setBool
    // 参数 name - uniform变量的名称
    // 参数 value - 要设置的布尔值
    // 功能：设置着色器程序中名为name的bool类型uniform变量的值
    // 注意：OpenGL没有直接的bool类型uniform，所以实际转换为int(0或1)传递
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    // ------------------------------------------------------------------------

    // setInt
    // 参数 name - uniform变量的名称
    // 参数 value - 要设置的整数值
    // 功能：设置着色器程序中名为name的int类型uniform变量的值
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    // ------------------------------------------------------------------------

    // setFloat
    // 参数 name - uniform变量的名称
    // 参数 value - 要设置的浮点数值
    // 功能：设置着色器程序中名为name的float类型uniform变量的值
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
private:
    // checkCompileErrors：用于检查着色器编译/链接错误。
    // 参数 shader - 要检查的着色器对象或程序对象
    // 参数 type - 着色器或程序的类型
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success; // 用于存储编译或链接成功与否的标志
        char infoLog[1024]; // 用于存储错误信息的缓冲区
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // 获取着色器编译状态
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog); // 获取着色器编译错误信息
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success); // 获取程序链接状态
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog); // 获取程序链接错误信息
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif
