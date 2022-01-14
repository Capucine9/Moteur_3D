#version 450

layout( location = 0) out vec4 fragColor;
layout (location = 1) in vec4 aVertexCouleurOut;
uniform float lum;

void main() 
{
	fragColor = (aVertexCouleurOut*lum);
}
