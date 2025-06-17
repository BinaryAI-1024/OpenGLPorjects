#version 330 core
out vec4 FragColor;
uniform vec4 ourColor; // 用于设置颜色的uniform变量

void main()
{
    FragColor = ourColor; // 使用uniform颜色
}