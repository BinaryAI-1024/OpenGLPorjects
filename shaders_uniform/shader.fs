#version 330 core
out vec4 FragColor;
uniform vec4 ourColor; // ����������ɫ��uniform����

void main()
{
    FragColor = ourColor; // ʹ��uniform��ɫ
}