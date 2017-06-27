//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include "ray.h"
#include <cstring>
#include <imgui.h>



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

                                         moX = xpos / 1.0f;
                                         moY = ypos / 1.0f;

                                         if (mousePressedLeft)
                                         {
                                             posX = (xpos - oldPosX);
                                             posY = (ypos - oldPosY);
                                         }

                                     if (!mousePressed)
                                         {
                                             oldPosX = xpos;
                                             oldPosY = ypos;
                                             return;
                                         }

                                         if (firstM)
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

                                         mX = xpos;
                                         mY = 300 - ypos;

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

        std::shared_ptr<LightNode> lNode = std::make_shared<LightNode>();

        //Particle system
        ParticleSystem* pSystem = new ParticleSystem("computeShader.cs");
        pSystem->initGraphics();
        pSystem->updateParticles();


        //Create graphics nodes and give them correct physical properties.
        box1 = new GraphicsNode();
        box1->getMesh()->loadOBJ("cube.obj");
        box1->setTexture(std::make_shared<TextureResource>("stone.jpg"));
        box1->setPhysicsNode(std::make_shared<PhysicsNode>());
        box1->getPhysicsNode()->createInerta(2, 2, 2);
        box1->setLight(lNode);



        box2 = new GraphicsNode();
        box2->getMesh()->loadOBJ("cube.obj");
        box2->setTexture(std::make_shared<TextureResource>("stone.jpg"));
        box2->setPhysicsNode(std::make_shared<PhysicsNode>());
        box2->getPhysicsNode()->createInerta(2, 2, 2);
        box2->getPhysicsNode()->position = vector3D(-3.1f, 0.0f, -0.0f);
        box2->setLight(lNode);

        box3 = new GraphicsNode();
        box3->getMesh()->loadOBJ("planeBox.obj");
        box3->setTexture(std::make_shared<TextureResource>("wood.jpg"));
        box3->setPhysicsNode(std::make_shared<PhysicsNode>());
        box3->getPhysicsNode()->inverseMass = 0.0f;
        box3->getPhysicsNode()->invInertiaTensorShape = matrix4D();
        box3->getPhysicsNode()->invInertiaTensorShape[0][0] = 0.f;
        box3->getPhysicsNode()->invInertiaTensorShape[1][1] = 0.f;
        box3->getPhysicsNode()->invInertiaTensorShape[2][2] = 0.f;
        box3->getPhysicsNode()->invInertiaTensorShape[3][3] = 0.f;
        box3->getPhysicsNode()->acceleration = vector3D();
        box3->getPhysicsNode()->position = vector3D(0.f, -3.1f, -0.0f);
        box3->setLight(lNode);

        gNodesPhysics.push_back(box1);
        gNodesPhysics.push_back(box2);
        gNodesPhysics.push_back(box3);

        //box1->getPhysicsNode()->addForceAtPoint(0.5f, vector3D(-1.f, 0.f, 0.f), box1->getPhysicsNode()->position);

        cHandler = new CollisionHandler(gNodesPhysics);     //initialize collision handler

        gNodesPoints = new GraphicsNode();      //Graphics node for drawing points
        projection = (projection.setPerspective(45.0f, (wWidht / wHeight), 0.01f, 100.0f));

        matrix4D sphereMat;
        matrix4D pv;

        //Time variables for calculationg alg time
        double lastTime = glfwGetTime();

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
            posY = 0.0f;
            posX = 0.0f;

            pv = view * projection;


            if (box1->getPhysicsNode()->posLocked)            //Lock box0 to (0,0,0)
            {
                box1->getPhysicsNode()->position.setValues(0);
                box1->getPhysicsNode()->velocity.setValues(0);
            }
            if (!box1->getPhysicsNode()->gravity)
            {
                for (int i = 0; i < gNodesPhysics.size()-1; ++i)
                {
                    gNodesPhysics[i]->getPhysicsNode()->acceleration = vector3D();
                }
            } else
            {
                for (int i = 0; i < gNodesPhysics.size()-1; ++i)
                {
                    gNodesPhysics[i]->getPhysicsNode()->acceleration = vector3D(0.f, -0.1f, 0.f);
                }
            }

            for (int i = 0; i < gNodesPhysics.size(); ++i)        //Integrate physics and draw boxes
            {

                gNodesPhysics[i]->getPhysicsNode()->integrate(timeStep);
                gNodesPhysics[i]->drawOBJPhysics(pv, rayForce);
            }
            lastTime = currentTime;

            cHandler->SortAndSweep();        //Check for collision
            cHandler->ResolveCollision(gNodesPhysics);

            if (mousePressedLeft)   //Cast ray from mouse pos
            {
                RayFromMouse();
            }

            if (gNodesRay != nullptr)
            {
                //Draw ray
                //gNodesRay->drawLines(pv, sphereMat);
                //gNodesRay->getMesh()->DrawLine();
                //Draw points
                vector3D poi = gNodesRay->getMesh()->ra->pHit;
                if (!(poi == vector3D(-13, -3, -7)))
                {
                    //      gNodesPoints->getMesh()->DrawPoint(gNodesRay->getMesh()->ra->pHit);
                }
            }

            //gNodesPoints->getMesh()->DrawPoint(cHandler->point);        //Draw coll point

            //gNodesPhysics[0]->drawLines(pv, sphereMat);
            for (int i = 0; i < gNodesPhysics.size(); ++i)        //Draw AABB
            {
               // gNodesPhysics[i]->getMesh()->DrawBox();
            }

            pSystem->animate();
            pSystem->display(pv * sphereMat);
            pSystem->render(pv);

            this->window->SwapBuffers();
        }
    }

    float ExampleApp::rad(float d)  //Degrees to rad
    {
        float rad = (PI / 180) * d;
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

            if (!box1->getPhysicsNode()->posLocked)
            {
                if (ImGui::Button("Lock Position.0"))
                {
                    box1->getPhysicsNode()->posLocked = true;
                }
            } else
            {
                if (ImGui::Button("Unlock Position.0"))
                {
                    box1->getPhysicsNode()->posLocked = false;
                }
            }
            ImGui::Checkbox("Gravity", &box1->getPhysicsNode()->gravity);

            for (int i = 0; i < gNodesPhysics.size(); ++i)
            {
                ImGui::Text("Object.%i Position: x: %.2f, y: %.2f, z: %.2f", i,
                            gNodesPhysics[i]->getPhysicsNode()->position.x(),
                            gNodesPhysics[i]->getPhysicsNode()->position.y(),
                            gNodesPhysics[i]->getPhysicsNode()->position.z());
                ImGui::Text("Object.%i Velocity: x: %.2f, y: %.2f, z: %.2f", i,
                            gNodesPhysics[i]->getPhysicsNode()->velocity.x(),
                            gNodesPhysics[i]->getPhysicsNode()->velocity.y(),
                            gNodesPhysics[i]->getPhysicsNode()->velocity.z());
                ImGui::Text("Object.%i Rotation: x: %.2f, y: %.2f, z: %.2f", i,
                            gNodesPhysics[i]->getPhysicsNode()->rotation.x(),
                            gNodesPhysics[i]->getPhysicsNode()->rotation.y(),
                            gNodesPhysics[i]->getPhysicsNode()->rotation.z());
                ImGui::Text("Object.%i AABB Center: x: %.2f, y: %.2f, z: %.2f", i,
                            gNodesPhysics[i]->getMesh()->box->center.x(), gNodesPhysics[i]->getMesh()->box->center.y(),
                            gNodesPhysics[i]->getMesh()->box->center.z());
                ImGui::Text("Object.%i Quat Lenght: %.1f", i, gNodesPhysics[i]->getPhysicsNode()->orientation.length());
                if (i != gNodesPhysics.size() - 1)
                    ImGui::Text("\n");
            }
            ImGui::SliderFloat("Force", &this->rayForce, 0.0f, 10.0f);
            if (cHandler->collisionBool)
            {
                ImGui::Text("Collision data: Point: x: %.2f, y: %.2f, z: %.2f", cHandler->point.x(),
                            cHandler->point.y(), cHandler->point.z());
                ImGui::Text("Normal: x: %.2f, y: %.2f, z: %.2f", cHandler->norm.x(), cHandler->norm.y(),
                            cHandler->norm.z());
                ImGui::Text("Depth: x: %.2f", cHandler->penDepth);
            }
            ImGui::End();
        }
    }

} // namespace Example