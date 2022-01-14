#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitangent;

uniform mat4 uMVPMatrix; // Projection * View * Model
uniform mat4 normal_matrix;
uniform mat4 ModelView;

out vec3 new_normal_matrix;
out vec3 fragPosition;
out vec3 lPos;

out vec3 tangentPos;
out vec3 tangentLoc;
out vec2 VertexTexCoords;

vec3 t;
vec3 b;
vec3 n;
mat3 inv_TBN;

void main()
{
	VertexTexCoords = aVertexTexCoords;

	//view space
	vec4 norm		  = normal_matrix * vec4( aVertexNormal, 1.f );
	new_normal_matrix = norm.xyz;
	fragPosition	  = aVertexPosition;

	vec3 modelPos = ( uMVPMatrix * vec4( aVertexPosition, 1.f ) ).xyz;
	vec3 lightPos = normalize( uMVPMatrix * vec4( 0.f, 0.f, 0.f, 0.f ) ).xyz;
	lPos		  = normalize( lightPos - modelPos );

	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );


	n = normalize ( ModelView * vec4( aVertexNormal, 0.0 ) ).xyz ;
	t = normalize ( ModelView * vec4( aVertexTangent, 0.0 ) ).xyz ;
	b = normalize ( ModelView * vec4( aVertexBitangent, 0.0 ) ).xyz ;

	inv_TBN = transpose ( mat3 ( t, b, n ) ); //inversion de la matrice

	//tangent space
	tangentPos = inv_TBN * fragPosition;
	tangentLoc = inv_TBN * vec3(0,0,0);
}
