//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include "ray.h"
#include "fluidsimulation.h"
#include <cstring>
#include <imgui.h>
#include <algorithm>



using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
    ExampleApp::ExampleApp()
    {
        this->gNodesRay = nullptr;
        // empty
    }

//------------------------------------------------------------------------------
/**
*/
    ExampleApp::~ExampleApp()
    {
        for (int i = 0; i < gNodesPlane.size(); ++i)
        {
            delete gNodesPlane[i];
        }
        for (int i = 0; i < gNodesPhysics.size(); ++i)
        {
            delete gNodesPhysics[i];
        }
        delete gNodesRay;
        delete gNodesPoints;
        // empty
    }

//------------------------------------------------------------------------------
/**
*/
    bool
    ExampleApp::Open()
    {
        App::Open();
        wWidht = 1024;
        wHeight = 768;

        this->window = new Display::Window;
        this->window->SetSize(wWidht, wHeight);

        camera.setValues(0, 0, 4);
        origin.setValues(0, 0, 0);
        headUp.setValues(0, 1, 0);
        camFront.setValues(0.0f, 0.0f, 1.0f);
        yaw = -90.0f;    // Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
        pitch = 0.0f;

        oldPosX = wWidht / 2.0f;
        oldPosY = wHeight / 2.0f;


        window->SetKeyPressFunction([this](int32 key, int32, int32 action, int32)
            {
                if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
                {
                    camSpeed = 10.f;
                }
                if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
                {
                    camSpeed = 2.5f;
                }
                if (GLFW_KEY_G == key)
                {
                    if (uiIt > 1)
                    {
                        if (showUI)
                        {
                            showUI = false;
                            uiIt = 0;
                        } else
                        {
                            showUI = true;
                            uiIt = 0;
                        }
                    }
                    uiIt++;
                }
                if (GLFW_KEY_R == key)
                {
                    //glfwSetInputMode(this->window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    //mouseLocked = false;
                }
                if (GLFW_KEY_W == key)
                {
                        camera[0] -= (camFront * camSpeed * deltaTime)[0];
                        camera[1] -= (camFront * camSpeed * deltaTime)[1];
                        camera[2] -= (camFront * camSpeed * deltaTime)[2];
                }
                if (GLFW_KEY_A == key)
                {
                        camera[0] -= (camFront.cross(headUp).normalizeRe() * camSpeed * deltaTime)[0];
                        camera[1] -= (camFront.cross(headUp).normalizeRe() * camSpeed * deltaTime)[1];
                        camera[2] -= ((camFront.cross(headUp).normalizeRe()) * camSpeed * deltaTime)[2];
                }
                if (GLFW_KEY_S == key)
                {
                        camera[0] += (camFront * camSpeed * deltaTime)[0];
                        camera[1] += (camFront * camSpeed * deltaTime)[1];
                        camera[2] += (camFront * camSpeed * deltaTime)[2];
                }
                if (GLFW_KEY_D == key)
                {
                        camera[0] += (camFront.cross(headUp).normalizeRe() * camSpeed * deltaTime)[0];
                        camera[1] += (camFront.cross(headUp).normalizeRe() * camSpeed * deltaTime)[1];
                        camera[2] += ((camFront.cross(headUp).normalizeRe()) * camSpeed * deltaTime)[2];
                }


            });
        window->SetMousePressFunction([this](int32 key, int32 state, int32)
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
                                         if (!mousePressed)
                                         {
                                             oldPosX = xpos;
                                             oldPosY = ypos;
                                             return;
                                         }

                                         GLfloat xoffset = xpos - oldPosX;
                                         GLfloat yoffset = oldPosY - ypos;

                                         float sensitivity = 0.05;
                                         xoffset *= sensitivity;
                                         yoffset *= sensitivity;

                                         yaw += xoffset;
                                         pitch -= yoffset;

                                         if (pitch > 89.0f)
                                             pitch = 89.0f;
                                         if (pitch < -89.0f)
                                             pitch = -89.0f;

                                         vector3D front;
                                         front[0] = (cos(rad(yaw)) * cos(rad(pitch)));
                                         front[1] = (sin(rad(pitch)));
                                         front[2] = -(sin(rad(yaw)) * cos(rad(pitch)));
                                         camFront = front.normalizeRe();

                                         oldPosX = xpos;
                                         oldPosY = ypos;

                                     });

        if (this->window->Open())
        {

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

            // set ui rendering function
            this->window->SetUiRender([this]()
                                      {
                                          this->RenderUI();
                                      });

            return true;
        }
        return false;
    }

//------------------------------------------------------------------------------
/**
*/
    void
    ExampleApp::Run()
    {

        sim = new FluidSimulation(2.f, 2.f, 2.f, vector3D(0,0,0));
        //sim->initParticles();
        //sim->grid.addCell(1,1,1, new Cell(Cell::cellType::AIR));


        sim->initParticles();
        sim->CalculateTimeStep();
        sim->UpdateGrid();
        sim->AdvanceVelocityField();

        //Particle system
        ParticleSystem* pSystem = new ParticleSystem("computeShader.cs");
        pSystem->initGraphics();
        pSystem->updateParticles();
        //int test = 0.9f * 10;


        GraphicsNode* waterBoxes = new GraphicsNode();
        waterBoxes->setTexture(std::make_shared<TextureResource>("blue.jpg"));
        GraphicsNode* airBoxes = new GraphicsNode();
        airBoxes->setTexture(std::make_shared<TextureResource>("bricks.jpg"));
        GraphicsNode* solidBoxes = new GraphicsNode();
        solidBoxes->setTexture(std::make_shared<TextureResource>("redtex.jpg"));



        GraphicsNode* markerG = new GraphicsNode();
        markerG->getMesh()->box = new AABB(sim->pos, vector3D(sim->width/2.f, sim->height/2.f, sim->depth/2.f));

        projection = projection.setPerspective(45.0f, (wWidht / wHeight), 0.01f, 100.0f);

        matrix4D sphereMat;
        matrix4D pv;

        //Time variables for calculationg alg time
        double lastTime = glfwGetTime();

        int iter = 0;

        glEnable(GL_DEPTH_TEST);
        //Wireframe
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        while (this->window->IsOpen())
        {
			//Calculate deltaTime
			double currentTime = glfwGetTime();
			deltaTime = fabs(currentTime - lastTime);

			FPS = 1 / deltaTime;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            this->window->Update();

            view = view.LookAtRH(camera, camera + camFront, headUp);

            pv = view * projection;

            lastTime = currentTime;


            sim->UpdateGrid();
            sim->AdvanceVelocityField();


            //Draw cells
            markerG->drawLines(pv, sphereMat);
            markerG->getMesh()->DrawBox();
            std::tr1::unordered_map<float, Cell*>::iterator it = sim->grid.macCells.begin();
            for (it; it != sim->grid.macCells.end(); ++it)
            {
               /* if(it->second->type == Cell::FLUID)
                {
                    if (it->second->rightN != nullptr)
                        markerG->getMesh()->DrawPoint(it->second->rightN->position);
                    if (it->second->leftN != nullptr)
                        markerG->getMesh()->DrawPoint(it->second->leftN->position);
                    if (it->second->upN != nullptr)
                        markerG->getMesh()->DrawPoint(it->second->upN->position);
                    if (it->second->downN != nullptr)
                        markerG->getMesh()->DrawPoint(it->second->downN->position);
                    if (it->second->inN != nullptr)
                        markerG->getMesh()->DrawPoint(it->second->inN->position);
                    if (it->second->outN != nullptr)
                        markerG->getMesh()->DrawPoint(it->second->outN->position);
                }*/
                if(it->second->type == Cell::SOLID)
                {
                    solidBoxes->drawLines(pv, sphereMat);
                    glLineWidth(.1f);
                    //solidBoxes->getMesh()->DrawBox(AABB(vector3D(it->second->position.x(), it->second->position.y(), it->second->position.z()), vector3D(sim->grid.hCellWidth, sim->grid.hCellWidth, sim->grid.hCellDepth)));
                    vector3D a = it->second->stagedVel;
                }
                if(it->second->type == Cell::AIR)
                {
                    airBoxes->drawLines(pv, sphereMat);
                    glLineWidth(.1f);
                    //airBoxes->getMesh()->DrawBox(AABB(vector3D(it->second->position.x(), it->second->position.y(), it->second->position.z()), vector3D(sim->grid.hCellWidth, sim->grid.hCellWidth, sim->grid.hCellDepth)));
                    vector3D a = it->second->stagedVel;
                }
                if(it->second->type == Cell::FLUID)
                {
                    glLineWidth(6.f);
                    waterBoxes->drawLines(pv, sphereMat);
                    waterBoxes->getMesh()->DrawBox(AABB(vector3D(it->second->position.x(), it->second->position.y(), it->second->position.z()), vector3D(sim->grid.hCellWidth, sim->grid.hCellWidth, sim->grid.hCellDepth)));
                    vector3D a = it->second->stagedVel;
                }

                {
                    //markerG->getMesh()->DrawLine(it->second->stagedVel, vector3D(it->second->position.x(), it->second->position.y(), it->second->position.z()));

                }
            }

            for (int i = 0; i < sim->grid.test.size(); ++i)
            {
                //markerG->getMesh()->DrawPoint(sim->grid.test[i]);
                //markerG->getMesh()->DrawPoint(sim->grid.indexVector[i]);
            }



            pSystem->animate();
            pSystem->display();
            //pSystem->render(pv);

            this->window->SwapBuffers();
        }
    }

    float ExampleApp::rad(float d)  //Degrees to rad
    {
        float rad = (PI / 180.f) * d;
        return rad;
    }

    void ExampleApp::CreatePlane(vector3D a, vector3D b, vector3D c, vector3D d)        //Create plane from dimensions
    {
        GraphicsNode *graph = new GraphicsNode();
        Plane p = *graph->getMesh()->CreateQuadPlane(a, b, c, d);
        gNodesPlane.push_back(graph);
    }

    void ExampleApp::CreateRay(vector3D a, vector3D b)
    {
        if (this->gNodesRay == nullptr)
        {
            this->gNodesRay = new GraphicsNode();
        } else
        {
            delete this->gNodesRay->getMesh()->ra;
        }
        Ray *rayj = new Ray(a, b);
        gNodesRay->getMesh()->ra = rayj;
    }

    void ExampleApp::RayFromMouse()     //Calculate ray from mouse
    {
        float nearPlane = 0.01f;
        matrix4D tempInvProj = this->projection.inv();
        matrix4D invView = this->view.inv();

        vector4D p((moX / wWidht - 0.5f) * 2.0f, (moY / wHeight - 0.5f) * 2.0f, 1.0f, 1.0f);

        vector4D pointView = tempInvProj.transform(p);
        vector4D localMousePos = pointView * nearPlane * 1.1f;
        localMousePos[1] = -1 * localMousePos[1];

        vector4D worldMousePos = invView.transform(localMousePos);
        vector4D worldMouseDir;
        worldMouseDir[0] = worldMousePos[0] - invView[3][0];
        worldMouseDir[1] = worldMousePos[1] - invView[3][1];
        worldMouseDir[2] = worldMousePos[2] - invView[3][2];
        worldMouseDir[3] = 0;
        worldMouseDir.normalize();

        vector4D endPoint = worldMousePos + worldMouseDir * 1000;

        vector3D p0(worldMousePos[0], worldMousePos[1], worldMousePos[2]);
        vector3D p1(endPoint[0], endPoint[1], endPoint[2]);
        this->CreateRay(p0, p1);

        for (int i = 0; i < gNodesPhysics.size(); ++i)
        {
            gNodesPhysics[i]->getMesh()->CollisionBox(gNodesRay->getMesh()->ra);
        }
        for (int i = 0; i < gNodesPhysics.size(); ++i)
        {
            gNodesPhysics[i]->getMesh()->PointOnMesh(gNodesRay->getMesh()->ra);
            if (gNodesPhysics[i]->getMesh()->hitByRay == true)
            {
                break;
            }
        }
    }

    void ExampleApp::RenderUI()
    {
        if (this->window->IsOpen() && showUI)
        {
            bool show = true;
            ImGui::Begin("Debug", &show, ImGuiWindowFlags_NoSavedSettings);
			ImGui::Text("FPS: %i", FPS);

            int cx, cy, cz;
            glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &cx );
            glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &cy );
            glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &cz );
            ImGui::Text("Max Compute Work Group Count = %5d, %5d, %5d", cx, cy, cz );

            int sx, sy, sz;
            glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &sx );
            glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &sy );
            glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &sz );
            ImGui::Text("Max Compute Work Group Size  = %5d, %5d, %5d", sx, sy, sz );

            ImGui::Text("My Compute Work Group Count  = %5d, %5d, %5d", NUM_PARTICLES/WORK_GROUP_SIZE, 1, 1 );
            ImGui::Text("My Compute Work Group Size   = %5d, %5d, %5d", WORK_GROUP_SIZE, 1, 1 );
            ImGui::Text("Number of particles = %5d", NUM_PARTICLES);
            ImGui::End();
        }
    }

} // namespace Example