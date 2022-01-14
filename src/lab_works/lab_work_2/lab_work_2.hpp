#ifndef __LAB_WORK_2_HPP__
#define __LAB_WORK_2_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork2 : public BaseLabWork
	{
	  public:
		LabWork2() : BaseLabWork() {}
		~LabWork2();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

	  private:
		GLuint			   _program = GL_INVALID_INDEX;
		std::vector<Vec2f> _vertices;
		std::vector<int>   _indices_vertices;
		std::vector<Vec4f> _couleurs;
		GLuint			   _ebo	 = GL_INVALID_INDEX;
		GLuint			   _vbo	 = GL_INVALID_INDEX;
		GLuint			   _vboc = GL_INVALID_INDEX;
		GLuint			   _vao	 = GL_INVALID_INDEX;
		GLint			   _loc;
		GLint			   _locLum;
		float			   _time = 0;
		float			   _lum	 = 1.f;
		// ================ Scene data.
		// ================

		// ================ GL data.
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_HPP__
