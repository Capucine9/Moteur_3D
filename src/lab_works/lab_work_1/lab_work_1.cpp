#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";

	LabWork1::~LabWork1()
	{
		glDeleteProgram( _program );
		glDeleteBuffers( 1, &_vbo );
		glDisableVertexArrayAttrib( _vao, 0 );
		glDeleteVertexArrays( 1, &_vao );
	}

	bool LabWork1::init()
	{
		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw1.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw1.frag" );
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

		_vertices = { Vec2f( -0.5, 0.5 ), Vec2f( 0.5, 0.5 ), Vec2f( 0.5, -0.5 ) };

		glCreateBuffers( 1, &_vbo );
		glNamedBufferData( _vbo, _vertices.size() * sizeof( Vec2f ), _vertices.data(), GL_STATIC_DRAW );

		glCreateVertexArrays( 1, &_vao );
		glEnableVertexArrayAttrib( _vao, 0 );
		glVertexArrayAttribFormat( _vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _vao, 0, _vbo, 0, sizeof( Vec2f ) );
		// glVertexArrayAttribBinding( _vao, 0, 0 );
		glUseProgram( _program );
		glDisable( GL_DEPTH_TEST );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork1::animate( const float p_deltaTime ) {}

	void LabWork1::render()
	{
		glClear( GL_COLOR_BUFFER_BIT );
		glBindVertexArray( _vao );
		glDrawArrays( GL_TRIANGLES, 0, 3 );
		glBindVertexArray( 0 );
	}

	void LabWork1::handleEvents( const SDL_Event & p_event ) {}

	void LabWork1::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
