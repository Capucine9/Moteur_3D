#include "lab_work_3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/random.hpp"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork3::_shaderFolder = "src/lab_works/lab_work_3/shaders/";

	LabWork3::~LabWork3() { glDeleteProgram( _program ); } 

	bool LabWork3::init()
	{
		std::cout << "Initializing lab work 3..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		if ( !_initProgram() )
			return false;

		_cube			= _createCube();
		_cube._vertices = {
			Vec3f( -0.5f, -0.5f, -0.5f ), 
			Vec3f( 0.5f, -0.5f, -0.5f ),  
			Vec3f( 0.5f, -0.5f, 0.5f ),	  
			Vec3f( -0.5f, -0.5f, 0.5f ),  
			Vec3f( -0.5f, 0.5f, -0.5f ),  
			Vec3f( 0.5f, 0.5f, -0.5f ),	  
			Vec3f( 0.5f, 0.5f, 0.5f ),	  
			Vec3f( -0.5f, 0.5f, 0.5f ),	  
		};
		_cube._vertexColors = { 
								getRandomVec3f(), getRandomVec3f(), getRandomVec3f(), getRandomVec3f(),
								getRandomVec3f(), getRandomVec3f(), getRandomVec3f(), getRandomVec3f()
		};
		// groupes de trois triangles
		_cube._indices = {0,1,4, 
			0,3,4, 
			0,1,3, 
			5,1,4, 
			5,1,6, 
			5,4,6,
			2,1,3, 
			2,1,6,
			2,3,6,
			7,6,4,
			7,6,3,
			7,4,3 
		};
		glEnable( GL_DEPTH_TEST );

		_initCamera();
		_initBuffers();

		glUseProgram( _program );

		_cube._transformation = glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.8 ) );
		// glUniformMatrix4fv( _uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr( transfo ) );
		glProgramUniformMatrix4fv( _program, _uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr( _cube._transformation ) );

		_updateViewMatrix();
		_updateProjectionMatrix();

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork3::animate( const float p_deltaTime )
	{
		_cube._transformation
			= glm::rotate( _cube._transformation, glm::radians( p_deltaTime * 50 ), glm::vec3( 0, 1, 0.5 ) );
		glProgramUniformMatrix4fv( _program, _uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr( _cube._transformation ) );

		// glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	}

	void LabWork3::render()
	{
		// Clear the color buffer.
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Lier le VAO au programme
		glBindVertexArray( _cube._vao );

		// Utilisation de l'EBO pour l'affichage
		glDrawElements( GL_TRIANGLES, _cube._indices.size(), GL_UNSIGNED_INT, 0 );

		glBindVertexArray( 0 );
	}

	void LabWork3::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				_updateViewMatrix();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateViewMatrix();
		}
	}

	void LabWork3::displayUI()
	{
		ImGui::Begin( "Settings lab work 3" );

		// Background.
		if ( ImGui::ColorEdit3( "Background", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}

		// Camera.
		if ( ImGui::SliderFloat( "fovy", &_fovy, 10.f, 160.f, "%01.f" ) )
		{
			_camera.setFovy( _fovy );
			_updateProjectionMatrix();
		}
		if ( ImGui::SliderFloat( "Speed", &_cameraSpeed, 0.1f, 10.f, "%01.1f" ) )
		{
			_camera.setFovy( _fovy );
			_updateProjectionMatrix();
		}

		ImGui::End();
	}

	void LabWork3::resize( const int p_width, const int p_height )
	{
		BaseLabWork::resize( p_width, p_height );
		_camera.setScreenSize( p_width, p_height );
	}

	bool LabWork3::_initProgram()
	{
		// ====================================================================
		// Shaders.
		// ====================================================================
		// Create shaders.
		const GLuint vertexShader	= glCreateShader( GL_VERTEX_SHADER );
		const GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		// Get sources from files.
		const std::string vertexShaderSrc	= readFile( _shaderFolder + "triangle.vert" );
		const std::string fragmentShaderSrc = readFile( _shaderFolder + "triangle.frag" );

		// Convert to GLchar *
		const GLchar * vSrc = vertexShaderSrc.c_str();
		const GLchar * fSrc = fragmentShaderSrc.c_str();

		// Compile vertex shader.
		glShaderSource( vertexShader, 1, &vSrc, NULL );
		glCompileShader( vertexShader );
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

		// Compile vertex shader.
		glShaderSource( fragmentShader, 1, &fSrc, NULL );
		glCompileShader( fragmentShader );
		// Check if compilation is ok.
		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compiled );
		if ( compiled != GL_TRUE )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fragmentShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std::cerr << "Error compiling fragment shader: " << log << std::endl;
			return false;
		}
		// ====================================================================

		// ====================================================================
		// Program.
		// ====================================================================
		// Create program.
		_program = glCreateProgram();

		// Attach shaders.
		glAttachShader( _program, vertexShader );
		glAttachShader( _program, fragmentShader );

		// Link program.
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

		// Shaders are now useless.
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
		// ====================================================================

		// ====================================================================
		// Get uniform locations.
		// ====================================================================

		_uModelMatrixLoc	  = glGetUniformLocation( _program, "uModelMatrix" );
		_uViewMatrixLoc		  = glGetUniformLocation( _program, "uViewMatrix" );
		_uProjectionMatrixLoc = glGetUniformLocation( _program, "uProjectionMatrix" );
		// ====================================================================

		return true;
	}

	void LabWork3::_initCamera()
	{
		_camera.setScreenSize( _windowWidth, _windowHeight );
		_camera.setPosition( Vec3f( 0, 1, 3 ) );
	}

	void LabWork3::_initBuffers()
	{
		// Creation du VBO
		glCreateBuffers( 1, &_cube._vboPositions ); 
		glCreateBuffers( 1, &_cube._vboColors );	
		glNamedBufferData(							
			_cube._vboPositions,
			_cube._vertices.size()
				* sizeof( Vec3f ),	
			_cube._vertices.data(), 
			GL_STATIC_DRAW );		
		glNamedBufferData(			
			_cube._vboColors,
			_cube._vertexColors.size() * sizeof( Vec3f ), 
			_cube._vertexColors.data(),					  
			GL_STATIC_DRAW );							  

		// Creation du VAO
		glCreateVertexArrays( 1, &_cube._vao );
		glEnableVertexArrayAttrib( _cube._vao, 0 );
		glEnableVertexArrayAttrib( _cube._vao, 1 );
		glVertexArrayAttribFormat(_cube._vao,0, 3, GL_FLOAT,GL_FALSE, 0 ); 
		glVertexArrayAttribFormat(_cube._vao,1,	3,GL_FLOAT, GL_FALSE, 0 );

		glVertexArrayVertexBuffer(_cube._vao,0,_cube._vboPositions,0,sizeof( Vec3f ) );	
		glVertexArrayVertexBuffer(_cube._vao,1,_cube._vboColors,0,sizeof( Vec3f ) ); 

		// Creation de l'EBO
		glCreateBuffers( 1, &_cube._ebo );
		glNamedBufferData( _cube._ebo, _cube._indices.size() * sizeof( int ), _cube._indices.data(), GL_STATIC_DRAW );
		// liaison de l'EBO avec le VAO
		glVertexArrayElementBuffer( _cube._vao, _cube._ebo );
	}

	void LabWork3::_updateViewMatrix()
	{
		glProgramUniformMatrix4fv( _program, _uViewMatrixLoc, 1, GL_FALSE, glm::value_ptr( _camera.getViewMatrix() ) );
	}

	void LabWork3::_updateProjectionMatrix()
	{
		glProgramUniformMatrix4fv(
			_program, _uProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr( _camera.getProjectionMatrix() ) );
	}

	LabWork3::Mesh LabWork3::_createCube() { return Mesh(); }
} // namespace M3D_ISICG
