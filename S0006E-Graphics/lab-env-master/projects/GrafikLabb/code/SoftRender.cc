#pragma once
#include "SoftRender.h"
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



SoftRender::SoftRender()
{
	Color p = {0, 0, 0};
	pixels.assign((height*width*2), p);
	

}


SoftRender::SoftRender(GLfloat vb[], GLfloat ib[])
{

	vBuffer = vb;
	iBuffer = ib;
	
}

void SoftRender::VertexBuffer()
{

	verticis = new Vertex[4];

	Vertex v1(0.0f,0.0f);
	Vertex v2(300.0f, 0.0f);
	Vertex v3(300.0f, 300.0f);
	Vertex v4(0.0f, 300.0f);

	v1.U = 0;
	v1.V = 0;

	v2.U = 1;
	v2.V = 0;

	v3.U = 1;
	v3.V = 1;

	v4.U = 0;
	v4.V = 1;


	verticis[0] = v1;
	verticis[1] = v2;
	verticis[2] = v3;
	verticis[3] = v4;

	indicies = new unsigned int[6];

	indicies[0] = 2;
	indicies[1] = 3;
	indicies[2] = 0;

	indicies[3] = 2;
	indicies[4] = 0;
	indicies[5] = 1;

}



SoftRender::~SoftRender()
{
}

SoftRender::Vertex SoftRender::interpolateUp(Vertex v1, Vertex v2, int y, int x)
{
	Vertex temp;
	float u, v, w;
	float pos = (v1.X - v2.X) / (v1.Y - v2.Y);

	temp.X = v1.X + y * pos;
	temp.Y = v1.Y + x * pos;

	//Barycentric(temp, verticis[indicies[0]], verticis[indicies[1]], verticis[indicies[2]], u, v, w);

	//temp.U = u;
	//temp.V = v;

	return temp;
}

SoftRender::Vertex SoftRender::interpolateDown(Vertex v1, Vertex v2, int y, int x)
{
	Vertex temp;
	float u, v, w;
	float pos = (v1.X - v2.X ) / (v1.Y - v2.Y);


	temp.X = v1.X - y * pos;
	temp.Y = v1.Y - x * pos;

	//Barycentric(temp, verticis[indicies[3]], verticis[indicies[4]], verticis[indicies[5]], u, v, w);

	//temp.U = u;
	//temp.V = v;

	return temp;



	
}

void SoftRender::Barycentric(Vertex p, Vertex v2, Vertex v3, Vertex v4, float &u, float &v, float &w)
{

	Vertex A, B, C;

	A.X = v3.X - v2.X;
	A.Y = v3.Y - v2.Y;

	B.X = v4.X - v2.X;
	B.Y = v4.Y - v2.Y;

	C.X = p.X - v2.X;
	C.Y = p.Y - v2.Y;

	float denomholder = (A.X * B.Y) - (B.X * A.Y);
	float divisionholder = (1 / denomholder);

	v = ((C.X * B.Y) - (B.X * C.Y)) * divisionholder;
	w = ((A.X * C.Y) - (C.X * A.Y)) * divisionholder;
	u = 1.0f - v - w;

// 	Vertex v0, v1, v2;
// 	v0.X = b.X - a.X;
// 	v0.Y = b.Y - a.Y;
// 
// 	v1.X = c.X - a.X;
// 	v1.Y = c.Y - a.Y;
// 
// 	v2.X = p.X - a.X;
// 	v2.Y = p.Y - a.Y;
// 
// 	float d00 = v0.X + v0.X + v0.Y + v0.Y;
// 	float d01 = v0.X + v1.X + v0.Y + v1.Y;
// 	float d11 = v1.X + v1.X + v1.Y + v1.Y;
// 	float d20 = v2.X + v0.X + v2.Y + v0.Y;
// 	float d21 = v2.X + v1.X + v2.Y + v1.Y;
// 	float denom = d00 * d11 - d01 * d01;
// 	v = (d11 * d20 - d01 * d21) / denom;
// 	w = (d00 * d21 - d01 * d20) / denom;
// 	u = 1.0f - v - w;

}

void SoftRender::setFrameBuffer(std::vector<Color> &c, int w, int h)
{
	pixels = c;
	width = w;
	height = h;

}

void SoftRender::GetNormal(Vertex &v1, Vertex &v2, Vertex &v3, Vertex &p)
{

	vector3D a(v1.X, v1.Y, 0.0f);
	vector3D b(v1.X, v1.Y, 0.0f);
	vector3D c(v1.X, v1.Y, 0.0f);

	vector3D u = a - b;
	vector3D v = a - c;

	vector3D Normal = u.cross(v);

	Normal.normalize();

	p.nx = Normal[0];
	p.ny = Normal[1];
	p.nz = Normal[2];
	
	
}

void SoftRender::setPixels(Vertex v1)
{
	float u, v, w;

	Vertex v2 = verticis[indicies[1]];
	Vertex v3 = verticis[indicies[0]];
	Vertex v4 = verticis[indicies[2]];

	Barycentric(v1, v2, v3, v4, u, v, w);

	v1.U = u * v2.U + v * v3.U + w * v4.U;
	v1.V = 1 - (u * v2.V + v * v3.V + w * v4.V);

	pixels[v1.Y * width + v1.X] = lambdaPixels(v1, image);
}

void SoftRender::draw(int w, int h)
{
	glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);
}

void SoftRender::fill()
{

}

void SoftRender::loadTexture(const char* filename)
{
	int width, height, numComponents;

	image = stbi_load(filename, &width, &height, &numComponents, 4);

	if (image == NULL){

		std::cerr << "Unable to load texture: " << filename << std::endl;
		return;
	}
	else
	{
		std::cout << "Loaded file";
	}

}



void SoftRender::fillBottomFlatTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	Vertex out_vert1;
	Vertex out_vert2;


	float invslope1 = (v1.X - v3.X) / (v1.Y - v3.Y);
	float invslope2 = (v1.X - v2.X) / (v1.Y - v2.Y);

	float curx1 = v1.X;
	float curx2 = v1.X;

	int numline = -0;

	GetNormal(v1, v2, v3, out_vert1);


	for (int scanlineY = v1.Y; scanlineY > v3.Y; scanlineY--)
	{
		out_vert1 = interpolateDown(v1, v2, numline, scanlineY);
		out_vert2 = interpolateDown(v1, v3, numline, scanlineY);


		
		curx1 -= invslope1;
		curx2 -= invslope2;
		bhm_line(out_vert1, out_vert2, scanlineY, numline);
		numline++;
	}
}

void SoftRender::fillTopFlatTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	Vertex out_vert1;
	Vertex out_vert2;

	float invslope1 = (v3.X - v1.X) / (v3.Y - v1.Y);
	float invslope2 = (v3.X - v2.X) / (v3.Y - v2.Y);

	float curx1 = v3.X;
	float curx2 = v3.X;

	GetNormal(v1, v2, v3, out_vert1);

	int numline = 0;

	for (int scanlineY = v3.Y; scanlineY <= v1.Y; scanlineY++)
	{
		out_vert1 = interpolateUp(v3, v1, numline, scanlineY);
		out_vert2 = interpolateUp(v3, v2, numline, scanlineY);


		bhm_line(out_vert1, out_vert2, scanlineY, numline);
		curx1 += invslope1;
		curx2 += invslope2;

		numline++;


		
	}
}

void SoftRender::bhm_line(Vertex v1, Vertex v2, int scanlineY, int numline)
{
	Vertex out_vert;

	float u, v, w;
	float x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = v2.X - v1.X;
	dy = 0/*v2.Y - v1.Y*/;
	dx1 = abs(dx);
	dy1 = abs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	int y1 = scanlineY;
	int y2 = scanlineY;


	//v1 is over v2
	if (dy1 <= dx1)
	{
		//Differansen i x led är mer än no
		if (dx >= 0)
		{
			x = v1.X;
			y = scanlineY;
			out_vert.X = v1.X;
			out_vert.Y = v1.Y;
			xe = v2.X;
			out_vert.U = v1.U;
			out_vert.V = v1.V;
			out_vert.nx = v1.nx;
			out_vert.ny = v1.ny;
			out_vert.nz = v1.nz;

		}
		else
		{
			x = v2.X;
			y = scanlineY;
			out_vert.X = v2.X;
			out_vert.Y = v2.Y;
			xe = v1.X;
			out_vert.U = v2.U;
			out_vert.V = v2.V;
			out_vert.nx = v1.nx;
			out_vert.ny = v1.ny;
			out_vert.nz = v1.nz;



		}



		//intrepolate
		out_vert.X = x;
		out_vert.Y = y;

		setPixels(out_vert);
		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
			{
				px = px + 2 * dy1;
			}
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0))
				{	
					
					
					y = y + 1;
				}
				else
				{	
					
					y = y - 1;
				}
				px = px + 2 * (dy1 - dx1);
			}

			//intrepolate
			out_vert.X = x;
			out_vert.Y = y;

			setPixels(out_vert);

		}
	}
	//V2 is over v1
	else
	{
		if (dy >= 0)
		{
			x = v1.X;
			y = v1.Y;
			ye = v2.Y;
		}
		else
		{
			x = v2.X;
			y = v2.Y;
			ye = v1.Y;
		}

		//intrepolate
		out_vert.X = x;
		out_vert.Y = y;

		setPixels(out_vert);
		for (i = 0; y < ye; i++)
		{
			//Y ändras
			y = y + 1;
			if (py <= 0)
			{
				py = py + 2 * dx1;
			}
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0))
				{
					x = x + 1;
				}
				else
				{
					x = x - 1;
				}
				py = py + 2 * (dx1 - dy1);
			}
			//intrepolate
			out_vert.X = x;
			out_vert.Y = y;

			setPixels(out_vert);
		}
	}

}



void SoftRender::sortVertices(Vertex& v1, Vertex& v2, Vertex& v3)
{
	if (v3.Y >= v2.Y && v3.Y >= v1.Y)
	{
		if (v1.Y > v2.Y)
		{
			Vertex temp;
			temp = v1;
			v1 = v2;
			v2 = temp;

			/* temp.X = v3.X;
			temp.Y = v3.Y;
			v3.X = v2.X;
			v3.Y = v2.Y;
			v2.X = temp.X;
			v2.Y = temp.Y;*/
		}
		Vertex temp;

		temp = v1;
		v1 = v3;
		v3 = temp;

		/* temp.X = v3.X;
		temp.Y = v3.Y;
		v3.X = v1.X;
		v3.Y = v1.Y;
		v1.X = temp.X;
		v1.Y = temp.Y;*/

	}
	else if (v2.Y >= v1.Y && v2.Y >= v3.Y)
	{
		if (v1.Y > v3.Y)
		{
			Vertex temp;

			temp = v3;
			v3 = v1;
			v1 = temp;

			/* temp.X = v3.X;
			temp.Y = v3.Y;
			v3.X = v1.X;
			v3.Y = v1.Y;
			v1.X = temp.X;
			v1.Y = temp.Y;*/
		}
		Vertex temp;

		temp = v1;
		v1 = v2;
		v2 = temp;

		/*  temp.X = v3.X;
		temp.Y = v3.Y;
		v3.X = v2.X;
		v3.Y = v2.Y;
		v2.X = temp.X;
		v2.Y = temp.Y;*/
	}
	else if (v1.Y >= v2.Y && v1.Y >= v3.Y)
	{
		if (v3.Y > v2.Y)
		{
			Vertex temp;

			temp = v3;
			v3 = v2;
			v2 = temp;

			/* temp.x = v1.x;
			temp.y = v1.y;
			v1.x = v2.x;
			v1.y = v2.y;
			v2.x = temp.x;
			v2.y = temp.y;*/
		}
	}
	
}


void SoftRender::drawQuad()
{

	VertexBuffer();
	drawTriangle(verticis[indicies[0]], verticis[indicies[1]], verticis[indicies[2]]);
	drawTriangle(verticis[indicies[3]], verticis[indicies[4]], verticis[indicies[5]]);




}

void SoftRender::drawTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	//sortVertices(v1, v2, v3);	

	if (v1.Y == v2.Y)
	{
		fillTopFlatTriangle(v1, v2, v3);

	}

	else if (v2.Y == v3.Y)
	{
		fillBottomFlatTriangle(v1, v2, v3);
	}
	else
	{
		Vertex v4((v1.X + ((float)(v2.Y - v1.Y) / (float)(v3.Y - v1.Y)) * (v3.X - v1.X)), v2.Y);
		fillBottomFlatTriangle(v1, v2, v4);
		fillTopFlatTriangle(v2, v4, v3);
	}


}


