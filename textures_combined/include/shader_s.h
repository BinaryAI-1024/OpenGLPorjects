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
    unsigned int ID; // �洢��ɫ�������Ψһ��ʶ��
    // ���� vertexPath - ������ɫ���ļ���·��
    // ���� fragmentPath - Ƭ����ɫ���ļ���·��
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��Դ����
        std::string vertexCode; // �洢������ɫ��Դ������ַ���
        std::string fragmentCode; // �洢Ƭ����ɫ��Դ������ַ���
        std::ifstream vShaderFile; // ���ڶ�ȡ������ɫ���ļ���������
        std::ifstream fShaderFile; // ���ڶ�ȡƬ����ɫ���ļ���������
        // ȷ��ifstream��������׳��쳣��
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // ���ļ�
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // ���ļ����ݶ�������
            vShaderStream << vShaderFile.rdbuf(); // ��ȡ������ɫ���ļ�����
            fShaderStream << fShaderFile.rdbuf(); // ��ȡƬ����ɫ���ļ�����
            // �ر��ļ����
            vShaderFile.close();
            fShaderFile.close();
            // ����ת��Ϊ�ַ���
            vertexCode = vShaderStream.str(); // ��������ɫ����ת��Ϊ�ַ���
            fragmentCode = fShaderStream.str(); // ��Ƭ����ɫ����ת��Ϊ�ַ���
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str(); // ת��ΪC����ַ����Ķ�����ɫ������
        const char* fShaderCode = fragmentCode.c_str(); // ת��ΪC����ַ�����Ƭ����ɫ������
        // 2. ������ɫ��
        unsigned int vertex, fragment;
        // ������ɫ��
        vertex = glCreateShader(GL_VERTEX_SHADER); // ����������ɫ������
        glShaderSource(vertex, 1, &vShaderCode, NULL); // ��������ɫ��Դ���븽�ӵ���ɫ������
        glCompileShader(vertex); // ���붥����ɫ��
        checkCompileErrors(vertex, "VERTEX"); // ��鶥����ɫ���������
        // Ƭ����ɫ��
        fragment = glCreateShader(GL_FRAGMENT_SHADER); // ����Ƭ����ɫ������
        glShaderSource(fragment, 1, &fShaderCode, NULL); // ��Ƭ����ɫ��Դ���븽�ӵ���ɫ������
        glCompileShader(fragment); // ����Ƭ����ɫ��
        checkCompileErrors(fragment, "FRAGMENT"); // ���Ƭ����ɫ���������
        // ��ɫ������
        ID = glCreateProgram(); // ������ɫ���������
        glAttachShader(ID, vertex); // ��������ɫ�����ӵ�����
        glAttachShader(ID, fragment); // ��Ƭ����ɫ�����ӵ�����
        glLinkProgram(ID); // ������ɫ������
        checkCompileErrors(ID, "PROGRAM"); // �����ɫ���������Ӵ���
        // ɾ����ɫ������Ϊ�����Ѿ����ӵ����ǵĳ����У�������Ҫ
        glDeleteShader(vertex); // ɾ��������ɫ������
        glDeleteShader(fragment); // ɾ��Ƭ����ɫ������
    }

    // ������ɫ��
    // ʹ�õ�ǰ��ɫ������
    void use()
    {
        glUseProgram(ID); // ������ɫ������
    }
    // setVec4
    // ���� name - uniform����������
    // ���� v0, v1, v2, v3 - Ҫ���õ��ĸ�������ֵ
    // ���ܣ�������ɫ����������Ϊname��vec4����uniform������ֵ
    void setVec4(const std::string& name, float v0, float v1, float v2, float v3) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3); // ����uniform vec4ֵ
    }

    // utility uniform functions

    // setBool
    // ���� name - uniform����������
    // ���� value - Ҫ���õĲ���ֵ
    // ���ܣ�������ɫ����������Ϊname��bool����uniform������ֵ
    // ע�⣺OpenGLû��ֱ�ӵ�bool����uniform������ʵ��ת��Ϊint(0��1)����
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    // ------------------------------------------------------------------------

    // setInt
    // ���� name - uniform����������
    // ���� value - Ҫ���õ�����ֵ
    // ���ܣ�������ɫ����������Ϊname��int����uniform������ֵ
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    // ------------------------------------------------------------------------

    // setFloat
    // ���� name - uniform����������
    // ���� value - Ҫ���õĸ�����ֵ
    // ���ܣ�������ɫ����������Ϊname��float����uniform������ֵ
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
private:
    // checkCompileErrors�����ڼ����ɫ������/���Ӵ���
    // ���� shader - Ҫ������ɫ�������������
    // ���� type - ��ɫ������������
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success; // ���ڴ洢��������ӳɹ����ı�־
        char infoLog[1024]; // ���ڴ洢������Ϣ�Ļ�����
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // ��ȡ��ɫ������״̬
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog); // ��ȡ��ɫ�����������Ϣ
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success); // ��ȡ��������״̬
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog); // ��ȡ�������Ӵ�����Ϣ
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif
