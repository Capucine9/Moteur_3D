#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;

uniform mat4 uMVPMatrix; // Projection * View * Model
uniform mat4 normal_matrix;

out vec3 new_normal_matrix;
out vec3 fragPosition;
out vec3 lPos;

void main()
{
	vec4 norm = normal_matrix*vec4(aVertexNormal, 1.f);
	new_normal_matrix = norm.xyz;
	fragPosition=aVertexPosition;

	//Changement lieu source lumineuse
	vec3 modelPos = (uMVPMatrix * vec4(aVertexPosition, 1.f)).xyz;
	vec3 lightPos=normalize(uMVPMatrix*vec4(450.f,300.f,-150.f,1.f)).xyz;
	lPos=normalize(lightPos-modelPos);

	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );
}
