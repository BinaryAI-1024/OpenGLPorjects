#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;

void main()
{
	// ����������֮��������Բ�ֵ (80% container, 20% awesomeface)
	FragColor = mix(texture(texSampler1, TexCoord), texture(texSampler2, TexCoord), 0.2);
}