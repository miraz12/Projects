#include "RTSCamera.h"
#include "GameHandler.h"

RTSCamera::RTSCamera()
{
	//Setup variables
	zoomAmount = -1000.0f;
	zoomSpeed = 10.0f;
	movespeed = 10.0f;
	zoomMin = -500;
	zoomMax = -2000;

	windowWidth = GameHandler::getInstance()->getWindowWidth();
	windowHeight = GameHandler::getInstance()->getWindowHeight();

	pos = glm::vec2(-windowWidth*0.3f, -windowHeight*0.3f);
	viewMatrix = glm::translate(glm::vec3(-windowWidth*0.3f, -windowHeight*0.3f, 0.0f));
	projectionMatrix = glm::ortho(zoomAmount*0.5f, -zoomAmount*0.5f, -zoomAmount*0.5f, zoomAmount*0.5f);
}

void RTSCamera::Move(const int& x,const int& y)
{
	pos.x = pos.x + x * movespeed;
	pos.y = pos.y + (y * movespeed);
	viewMatrix = glm::translate(glm::vec3(pos.x,pos.y, 0.0f));
}

void RTSCamera::ScrollZoom(GLFWwindow* window, double xamount, double yamount)
{
	RTSCamera* cam = RTSCamera::getInstance();
		cam->Zoom((float)yamount);
}

void RTSCamera::Zoom(const float& amount)
{
	zoomAmount += amount * zoomSpeed;
	zoomAmount = glm::clamp(zoomAmount, zoomMax, zoomMin);
	projectionMatrix = glm::ortho(zoomAmount*0.5f, -zoomAmount*0.5f, -zoomAmount*0.5f, zoomAmount*0.5f);
}
