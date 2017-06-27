#pragma once
#include "core/app.h"
#include "render/window.h"
//#include "TextureResource.h"
//#include "ShaderObject.h"
#include "matrix4D.h"
#include "vector3D.h"
#include "SoftRender.h"


namespace Labb2
{
	class Graphics : public Core::App
	{
	public:
		/// constructor
		Graphics();
		/// destructor
		~Graphics();

		/// open app
		bool Open();
		/// run app
		void Run();
	private:

		bool mousePressed = false;
		matrix4D mat;
		matrix4D model;

		GLuint program;
		GLuint vertexShader;
		GLuint pixelShader;
		GLuint triangle;
		Display::Window* window;
		float64 posX = 0.0f;
		float64 posY = 0.0f;
		float64 oldPosX = 0.0f;
		float64 oldPosY = 0.0f;
		float64 x = 0.0f;
		float64 y = 0.0f;
		float64 z = 0.0f;
		float64 scrollX = 0.0f;
		float64	scrollY = 0.0f;

        //Move cam
		vector3D camera;
        vector3D origin;
        vector3D headUp;
        //---
		SoftRender::Vertex v1;
		SoftRender::Vertex v2;
		SoftRender::Vertex v3;

		float mX, mY;

		int vertexINT = 0;
	};
} 