#version 400
in vec4 vPosition;
in vec4 vColor;
out vec4 color;

uniform mat4 Model_View;
uniform mat4 projection;
void main()
{
    gl_Position =  projection * Model_View * vPosition;
	color=vColor;
}//