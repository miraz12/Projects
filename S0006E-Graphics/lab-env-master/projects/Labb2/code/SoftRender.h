#pragma once
#include "GL/glew.h"
#include <vector>
#include <functional>
#include <iostream>
#include "vector3D.h"

class SoftRender
{
public:
	
	struct Vertex
	{
		float X, Y;
		float U = 0;
		float V = 0;
		float nx = 1;
		float ny = 1;
		float nz = 1;
		Vertex()
		{

		}

		Vertex(float x, float y)
		{
			X = x;
			Y = y;
		}

		void setXY(int x, int y)
		{
			X = x;
			Y = y;
		}
	};

	struct Color
	{
		unsigned char R, G, B;

		Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0){
			R = r;
			G = g;
			B = b;

		};

	};
	
	SoftRender();
	SoftRender(GLfloat[], GLfloat[]);
	void VertexBuffer();
	~SoftRender();

	SoftRender::Vertex interpolateUp(Vertex v1, Vertex v2, int y, int x);
	SoftRender::Vertex interpolateDown(Vertex v1, Vertex v2, int y, int x);

	void Barycentric(Vertex p, Vertex a, Vertex b, Vertex c, float &u, float &v, float &w);
	void setFrameBuffer(std::vector<Color> &c, int w, int h);
	void GetNormal(Vertex &v1, Vertex &v2, Vertex &v3, Vertex &p);
	void setPixels(Vertex v);
	//void setPixelsTop(Vertex v);
	void draw(int, int);
	void fill();
	void loadTexture(const char* filename);
	void bhm_line(int x1, int y1, int x2, int y2, Color& c);
	void bhm_line(Vertex v1, Vertex v2, int scanlineY, int numline);
/*	void bhm_line(Vertex v1, Vertex v2, int scanlineY, int numline);*/

	void sortVertices(Vertex& v1, Vertex& v2, Vertex& v3);
	void drawQuad();
	void drawTriangle(Vertex v1, Vertex v2, Vertex v3);
	void fillBottomFlatTriangle(Vertex v1, Vertex v2, Vertex v3);

	void fillTopFlatTriangle(Vertex v1, Vertex v2, Vertex v3);

	//std::function<Color()> lambdaPixels;

	std::function <Color(Vertex v1, unsigned char *image)> lambdaPixels;
	int width = 300;
	int height = 300;
	std::vector<Color> pixels;

private:

	unsigned int *indicies;
	Vertex *verticis;
	unsigned char *image;


	GLfloat* vBuffer;
	GLfloat* iBuffer;
	GLfloat* tBuffer;

	//matrix4D model, view, projektion, MVP;

	


};

