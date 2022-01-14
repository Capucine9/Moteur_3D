#version 450

uniform vec3 ambiant_color; 
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform float shininess;

in vec3 new_normal_matrix;
in vec3 fragPosition;
in vec3 lPos;

layout( location = 0 ) out vec4 fragColor;

vec3 newLight(vec3 normal, vec3 directLight)
{
	if (dot (normal, -directLight)<0.0)
		normal *= -1;
	return normal;
}

void main()
{
	//Eclairage diffus
	vec3 directLight=normalize(fragPosition- vec3(0.f,0.f,3.f));
	vec3 n =newLight(normalize(new_normal_matrix),directLight);
	float cosinus =max(dot(n, -directLight),0.f);
	vec3 diffuse=diffuse_color*cosinus;

	//Eclairage speculaire
	float cos = pow(dot(n, normalize(-directLight+lPos)),shininess);
	vec3 specular=specular_color*cos;

	fragColor = vec4((ambiant_color+ diffuse+ specular), 1.f );
}
