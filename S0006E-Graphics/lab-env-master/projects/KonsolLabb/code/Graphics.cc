#include "config.h"
#include "Graphics.h"
#include "GraphicsNode.h"

//#define true false
using namespace Display;

namespace Labb2
{
    Graphics::Graphics()
	{
	}

	Graphics::~Graphics()
	{
	}

	bool Graphics::Open()
	{
		App::Open();
		this->window = new Display::Window;
		this->window->SetSize(600, 600);

		window->SetKeyPressFunction([this](int32 key, int32, int32, int32)
		{
			if (GLFW_KEY_ESCAPE == key)
			{
				this->window->Close();
			}
			if (GLFW_KEY_W == key)
			{
				this->y += 0.1f;
                //camera[1] += 0.1f;
			}
			if (GLFW_KEY_A == key)
			{
				this->x -= 0.1f;
                //camera[0] -= 0.1f;
			}
			if (GLFW_KEY_S == key)
			{
				this->y -= 0.1f;
                //camera[1] -= 0.1f;
            }
			if (GLFW_KEY_D == key)
			{
				this->x += 0.1f;
                //camera[0] += 0.1f;
            }
		});

		window->SetMousePressFunction([this](int32 key, int32 state , int32)
		{
			if (key == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS)
			{
				mousePressed = true;
				if (vertexINT == 0)
				{
					v1.setXY(mX, mY);
					vertexINT++;
				}
				else if (vertexINT == 1)
				{
					v2.setXY(mX, mY);
					vertexINT++;
				}
				else if (vertexINT == 2)
				{
					v3.setXY(mX, mY);
					vertexINT++;
				}
				
			}
			if (key == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_RELEASE)
			{
				mousePressed = false;
			}

		});

		window->SetMouseScrollFunction([this](float64 x, float64 y)
		{
			scrollX += 0.1f * y;
		});

		window->SetMouseMoveFunction([this](float64 x, float64 y)
		{
			mX = x;
			mY = 300 - y;

			if (mousePressed)
			{
				posX = (x - oldPosX);
				posY = (y - oldPosY);

			}
				oldPosX = x;
				oldPosY = y;
			
		});

		if (this->window->Open())
		{
			
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	
			return true;
		}
		return false;
	}

	void Graphics::Run()
	{
        /*
			SoftRender s;

			s.lambdaPixels = [](SoftRender::Vertex v, unsigned char *image)
			{
				SoftRender::Color c;

				vector3D PL(50.0f, 2.0f, 1.0f);
				vector3D vec(v.nx, v.ny, v.nz);

				PL.normalize();

				float intens = vec.dott(PL)-4;

				if (intens < 0)
				{
					intens = 0;
				}
				if (intens > 1)
				{
					intens = 1;
				}

				int teX = (v.U * 300);
				int teY = (v.V * 300);
				int index = (teY * 300 + teX) * 4;

				c.R = image[index] * intens;
				c.G = image[index + 1] * intens;
				c.B = image[index + 2] * intens;

				return c;
			};

		TextureResource tex;

		s.loadTexture("frooog.jpg");
		s.draw(300, 300);
		s.drawQuad();

		GraphicsNode graph;
		texture = std::make_shared<TextureResource>();
		texture->loadFromRaz(s);
		texture->bind();
		graph.setTexture(texture);
        */
        GraphicsNode graph;

		camera.setValues(0, 0, 3);
		origin.setValues(0, 0, 0);
		headUp.setValues(0, 1, 0);

		matrix4D projection = (projection.setPerspective(80.0f, (600.0f / 600.0f), 0.01f, 1000.0f));
		matrix4D view;
		matrix4D mvp;
		
		glEnable(GL_DEPTH_TEST);

        while (this->window->IsOpen())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			this->window->Update();

			model = model.transform(x, y, (scrollX )) * model.rot_x(-posY / 10.0f) * model.rot_y(-posX / 10.0f);
			//model = model.transform(x * 100, y * 100, (scrollX + 5) * 100) * model.rot_x(posY / 10.0f) * model.rot_y(-posX / 10.0f);
			view = view.LookAtRH(camera, origin, headUp);

            //camera[2] = (float)(scrollX - 2);


            mvp = projection * view * model;
			graph.draw(mvp);

			this->window->SwapBuffers();
		}
	}

} // namespace Example