#include "config.h"
#include "Graphics.h"
#include <cstring>
#include <chrono>
#include "MeshResource.h"
#include "GraphicsNode.h"
#include "JointsStructure.h"
#define PI 3.14159265

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


        camera.setValues(0, 0, 4);
        origin.setValues(0, 0, 0);
        headUp.setValues(0, 1, 0);
        camFront.setValues(0.0f, 0.0f, 1.0f);
		yaw    = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
		pitch  =  0.0f;

		oldPosX = 300;
		oldPosY = 300;



		window->SetKeyPressFunction([this](int32 key, int32, int32, int32)
		{
            float camSpeed = 0.05f;
			if (GLFW_KEY_E == key)
			{
				if (mouseLocked == false)
				{
					glfwSetInputMode(this->window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					mouseLocked = true;
				}
			}
			if (GLFW_KEY_R == key)
			{
				glfwSetInputMode(this->window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				mouseLocked = false;
			}

			if (GLFW_KEY_1 == key)
            {
				this->clipIndex = 0;
            }
            if (GLFW_KEY_2 == key)
            {
                this->clipIndex = 1;
            }
            if (GLFW_KEY_3 == key)
            {
                this->clipIndex = 2;
            }
            if (GLFW_KEY_4 == key)
            {
                this->clipIndex = 3;
            }
            if (GLFW_KEY_5 == key)
            {
                this->clipIndex = 4;
            }
            if (GLFW_KEY_6 == key)
            {
                this->clipIndex = 5;
            }
            if (GLFW_KEY_7 == key)
            {
                this->clipIndex = 6;
            }
            if (GLFW_KEY_8 == key)
            {
                this->clipIndex = 7;
            }
            if (GLFW_KEY_ESCAPE == key)
			{
				this->window->Close();
			}
            if (GLFW_KEY_COMMA == key)
            {
                this->skeleIndex++;
            }
            if (GLFW_KEY_PERIOD == key)
            {
                this->skeleIndex--;
            }
			if (GLFW_KEY_W == key)
			{
				if (!mouseLocked)
				{
					this->y += 0.1f;
				} else
				{
					camera[0] -= (camFront * camSpeed)[0];
					camera[1] -= (camFront * camSpeed)[1];
					camera[2] -= (camFront * camSpeed)[2];
				}
				//this->y += 0.1f;

                //camera[1] += 0.1f;
			}
			if (GLFW_KEY_A == key)
			{
				if (!mouseLocked)
				{
					this->x += 0.1f;
				} else
				{
					camera[0] -= (camFront.cross(headUp).normalizeRe() * camSpeed)[0];
					camera[1] -= (camFront.cross(headUp).normalizeRe() * camSpeed)[1];
					camera[2] -= ((camFront.cross(headUp).normalizeRe()) * camSpeed)[2];
				}

                //camera[0] -= 0.1f;
			}
			if (GLFW_KEY_S == key)
			{
				if (!mouseLocked)
				{
					this->y -= 0.1f;
				} else
				{
					camera[0] += (camFront * camSpeed)[0];
					camera[1] += (camFront * camSpeed)[1];
					camera[2] += (camFront * camSpeed)[2];
				}

                //camera[1] -= 0.1f;
            }
			if (GLFW_KEY_D == key)
			{
				if (!mouseLocked)
				{
					this->x -= 0.1f;


				} else
				{
					camera[0] += (camFront.cross(headUp).normalizeRe() * camSpeed)[0];
					camera[1] += (camFront.cross(headUp).normalizeRe() * camSpeed)[1];
					camera[2] += ((camFront.cross(headUp).normalizeRe()) * camSpeed)[2];
				}

                //camera[0] += 0.1f;
            }
		});

		window->SetMousePressFunction([this](int32 key, int32 state , int32)
		{
			if (key == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS)
			{
				mousePressedLeft = true;
			}
			if (key == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_RELEASE)
			{
				mousePressedLeft = false;

			}
			if (key == GLFW_MOUSE_BUTTON_RIGHT && state == GLFW_PRESS)
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
			if (key == GLFW_MOUSE_BUTTON_RIGHT && state == GLFW_RELEASE)
			{
				mousePressed = false;
			}

		});

		window->SetMouseScrollFunction([this](float64 x, float64 y)
		{
			scrollX += 0.1f * y;
		});

		window->SetMouseMoveFunction([this](float64 xpos, float64 ypos)
		{


			if (mousePressedLeft)
			{
				posX = (xpos - oldPosX);
				posY = (ypos - oldPosY);

			}

			if (mouseLocked == false || mousePressed != true )
			{
				oldPosX = xpos;
				oldPosY = ypos;
				return;
			}

			if(firstM)
			{
				oldPosX = xpos;
				oldPosY = ypos;
				firstM = false;
			}

			GLfloat xoffset = xpos - oldPosX;
			GLfloat yoffset = oldPosY - ypos;
			oldPosX = xpos;
			oldPosY = ypos;

			float sensitivity = 0.05;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw   += xoffset;
			pitch -= yoffset;

			if(pitch > 89.0f)
				pitch = 89.0f;
			if(pitch < -89.0f)
				pitch = -89.0f;

			vector3D front;
			front[0] = (cos(rad(yaw)) * cos(rad(pitch)));
			front[1] = (sin(rad(pitch)));
			front[2] = -(sin(rad(yaw)) * cos(rad(pitch)));
			camFront = front.normalizeRe();


			mX = xpos;
			mY = 300 - ypos;

			oldPosX = xpos;
			oldPosY = ypos;

			
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
        JointsStructure a;
        GraphicsNode graph;


        matrix4D mvp;
		matrix4D projection;
		matrix4D view;

		glEnable(GL_DEPTH_TEST);

        //a.createSkeleton();
        projection = (projection.setPerspective(45.0f, (600.0f / 600.0f), 0.1f, 100.0f));
        //view = view.LookAtRH(camera, origin, headUp);
        a.createInverseOriginal();
        typedef std::chrono::system_clock Clock;
        auto start = Clock::now();
        static int keyclip = 0;


        while (this->window->IsOpen())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			this->window->Update();

            //Rotate camera:
            //float radius = 5.0f;
            //float camX = sin(glfwGetTime()) * radius;
            //float camZ = cos(glfwGetTime()) * radius;

			model = model.transform(x, y, 0) * model.rot_x(-posY ) * model.rot_y(-posX );
			//model = model.transform(x * 100, y * 100, (scrollX + 5) * 100) * model.rot_x(posY / 10.0f) * model.rot_y(-posX / 10.0f);
            //camera[2] = (float)(scrollX - 2);
			view = view.LookAtRH(camera, camera+camFront, headUp);

            //Rotate camera:
            //camera.setValues(camX, 0.0, camZ);
            //view = view.LookAtRH(camera, origin, headUp);
            posY = 0.0f;
            posX = 0.0f;

            //matrix4D rotmaty = model.rot_x(-posY * 100  ) * model.rot_y(-posX *100 ) ;// rotmaty.rot_y(2);
			//matrix4D rotmatx = rotmaty.rot_z(5);

            static int r = 0;
            if (a.an->animClips[clipIndex].keyDuration <= (ushort)std::chrono::duration_cast<std::chrono::milliseconds> (Clock::now()-start).count())
            {
                if ( r >= a.an->animClips[clipIndex].numKeys)
                {
                    r = 0;
                }
                a.PlayAnimation(r++, clipIndex);
                start = Clock::now();
            }

            //graph.draw(model);

            a.drawSkeleton();

            //mvp = projection * view * model;
            matrix4D pv = projection * view;
			graph.drawAnim(model, pv, a.bones);

			this->window->SwapBuffers();
		}


	}
	float Graphics::rad(float d)
	{
		float rad = (PI/180) * d;
		return rad;
	}
} // namespace Example