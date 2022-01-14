#include "lab_work_2.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";

	LabWork2::~LabWork2()
	{
		glDeleteProgram( _program );
		glDeleteBuffers( 1, &_vbo );
		glDeleteBuffers( 1, &_vboc );
		glDisableVertexArrayAttrib( _vao, 0 );
		glDeleteVertexArrays( 1, &_vao );
	}

	bool LabWork2::init()
	{
		std::cout << "Initializing lab work 2..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw2.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw2.frag" );
		const GLuint	  vertexShader		= glCreateShader( GL_VERTEX_SHADER );
		const GLuint	  fragmentShader	= glCreateShader( GL_FRAGMENT_SHADER );
		const GLchar *	  vSrc				= vertexShaderStr.c_str();
		const GLchar *	  fSrc				= fragmentShaderStr.c_str();
		glShaderSource( vertexShader, 1, &vSrc, NULL );
		glShaderSource( fragmentShader, 1, &fSrc, NULL );
		glCompileShader( vertexShader );
		glCompileShader( fragmentShader );
		// Check if compilation is ok.
		GLint compiled;
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertexShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std::cerr << "Error compiling vertex shader: " << log << std::endl;
			return false;
		}

		_program = glCreateProgram();
		glAttachShader( _program, vertexShader );
		glAttachShader( _program, fragmentShader );
		glLinkProgram( _program );
		// Check if link is ok.
		GLint linked;
		glGetProgramiv( _program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _program, sizeof( log ), NULL, log );
			std::cerr << "Error linking program: " << log << std::endl;
			return false;
		}
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );

		_vertices		  = { Vec2f( -0.5f, 0.5f ), Vec2f( 0.5f, 0.5f ), Vec2f( 0.5f, -0.5f ), Vec2f( -0.5f, -0.5f ) };
		_indices_vertices = { 0, 1, 2, 0, 2, 3 };
		_couleurs		  = { Vec4f( 1.f, 0.f, 0.f, 1.f ),
					  Vec4f( 0.f, 1.f, 0.f, 1.f ),
					  Vec4f( 0.f, 0.f, 1.f, 1.f ),
					  Vec4f( 1.f, 0.f, 1.f, 1.f ) };
		glCreateBuffers( 1, &_ebo );
		glNamedBufferData( _ebo, _indices_vertices.size() * sizeof( int ), _indices_vertices.data(), GL_STATIC_DRAW );

		glCreateBuffers( 1, &_vbo );
		glNamedBufferData( _vbo, _vertices.size() * sizeof( Vec2f ), _vertices.data(), GL_STATIC_DRAW );
		glCreateBuffers( 1, &_vboc );
		glNamedBufferData( _vboc, _couleurs.size() * sizeof( Vec4f ), _couleurs.data(), GL_STATIC_DRAW );

		glCreateVertexArrays( 1, &_vao );
		glEnableVertexArrayAttrib( _vao, 0 );
		glVertexArrayAttribFormat( _vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _vao, 0, _vbo, 0, sizeof( Vec2f ) );
		glVertexArrayAttribBinding( _vao, 0, 0 );

		glEnableVertexArrayAttrib( _vao, 1 );
		glVertexArrayAttribFormat( _vao, 1, 4, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _vao, 1, _vboc, 0, sizeof( Vec4f ) );
		glVertexArrayAttribBinding( _vao, 1, 1 );

		glVertexArrayElementBuffer( _vao, _ebo );
		glUseProgram( _program );
		glDisable( GL_DEPTH_TEST );

		_loc	= glGetUniformLocation( _program, "uTranslationX" );
		_locLum = glGetUniformLocation( _program, "lum" ); // recupere la localisation
		glProgramUniform1f( _program, _locLum, _lum );	   // initialisation de glProgramUniform1f

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork2::animate( const float p_deltaTime )
	{
		_time += p_deltaTime;
		glProgramUniform1f( _program, _loc, glm::sin( _time ) * 0.5 );
	}

	void LabWork2::render()
	{
		glClear( GL_COLOR_BUFFER_BIT );
		glBindVertexArray( _vao );
		glDrawElements( GL_TRIANGLES, _indices_vertices.size(), GL_UNSIGNED_INT, 0 );
		// glDrawElements( GL_LINES, _indices_vertices.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
	}

	void LabWork2::handleEvents( const SDL_Event & p_event ) {}

	void LabWork2::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );

		if ( ImGui::SliderFloat( "Luminosité", &_lum, 0.f, 1.f ) )
		{
			glProgramUniform1f( _program, _locLum, _lum );
		}
		if ( ImGui::ColorEdit3( "Couleur de fond", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}

		ImGui::End();
	}

} // namespace M3D_ISICG
