#version 450

layout (location = 0) in vec2 aVertexPosition;
layout (location = 1) in vec4 aVertexCouleurIn;
layout (location = 1) out vec4 aVertexCouleurOut;

uniform float uTranslationX;

void main() 
{
	gl_Position = vec4(aVertexPosition, 1.f, 1.f) + vec4(uTranslationX, 0.f, 0.f, 0.f);
	aVertexCouleurOut=aVertexCouleurIn;
}
