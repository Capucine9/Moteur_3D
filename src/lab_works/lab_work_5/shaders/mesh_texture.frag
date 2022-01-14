#version 450

uniform vec3  ambiant_color;
uniform vec3  diffuse_color;
uniform vec3  specular_color;
uniform float shininess;

//textures
uniform bool uHasDiffuseMap;
uniform bool uHasAmbientMap;
uniform bool uHasSpecularMap;
uniform bool uHasShininessMap;
uniform bool uHasNormalMap;

vec4 diffus_couleur;
vec3 ambient_couleur;
vec3 speculaire_couleur;
float shininess_couleur;

layout(binding=1) uniform sampler2D uDiffuseMap;
layout(binding=2) uniform sampler2D uAmbientMap;
layout(binding=3) uniform sampler2D uSpecularMap;
layout(binding=4) uniform sampler2D uShininessMap;
layout(binding=5) uniform sampler2D uNormalMap;

in vec3 new_normal_matrix;
in vec3 fragPosition;
in vec3 lPos;
in vec2 VertexTexCoords;

in vec3 Ptangent;
in vec3 Ltangent;


layout( location = 0 ) out vec4 fragColor;

vec3 newLight( vec3 normal, vec3 directLight )
{
	if ( dot( normal, -directLight ) < 0.0 )
		normal *= -1;
	return normal;
}


void main()
{
	

	if(uHasDiffuseMap){
		//diffus_couleur
		vec4 texel =texture(uDiffuseMap,VertexTexCoords);
		diffus_couleur = vec4(texel.rgb,1.f);
		if( texel.a < 0.5 )
			discard;
	}
	else{
		diffus_couleur= vec4 ( diffuse_color, 1.0 );
	}

	if(uHasAmbientMap){
		ambient_couleur=ambiant_color*(texture(uAmbientMap,VertexTexCoords)).xyz;
	}
	else{
		ambient_couleur=ambiant_color;
	}

	if(uHasSpecularMap){
		speculaire_couleur=specular_color*(texture(uSpecularMap,VertexTexCoords)).xxx;
	}
	else{
		speculaire_couleur=specular_color;
	}

	if(uHasShininessMap){
		shininess_couleur=(texture(uShininessMap,VertexTexCoords)).x;
	}
	else{
		shininess_couleur=shininess;
	}

	if(uHasNormalMap){//tangent space
	
		vec3  directLight = normalize( Ltangent - Ptangent );
		vec3  N			  = texture2D( uNormalMap, VertexTexCoords ).xyz;
		N = N*2 - 1;
		float cosinus	  = max( dot( N, -directLight ), 0.f );
		vec3  diffuse	  = diffuse_color * cosinus;

		float cos	   = pow( dot( N, normalize( -directLight - Ptangent ) ), shininess );
		vec3  specular = specular_color * cos;

		fragColor = vec4( ( ambient_couleur + diffus_couleur.xyz + speculaire_couleur ), 1.f );
	}

	else //view space
	{
		vec3  directLight = normalize( fragPosition - vec3( 0.f, 0.f, 0.f ) );
		vec3  N			  = newLight( normalize( new_normal_matrix ), directLight );
		float cosinus	  = max( dot( N, -directLight ), 0.f );
		vec3  diffuse	  = diffuse_color * cosinus;

		float cos	   = pow( dot( N, normalize( -directLight - fragPosition ) ), shininess );
		vec3  specular = specular_color * cos;

		fragColor = vec4( ( ambient_couleur + diffus_couleur.xyz + speculaire_couleur ), 1.f );
	}

}