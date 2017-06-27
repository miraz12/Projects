#pragma once
#include <GL\glew.h>
#include <glm.hpp>
#include <gtx\transform.hpp>
#include "GLFW\glfw3.h"

/************************************************************************/
/* SINGLETON

Holds View and Projection Matrices

*/
/************************************************************************/
class RTSCamera
{
private:
	RTSCamera();

public:
	static RTSCamera* getInstance()
	{
		static RTSCamera instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	RTSCamera(RTSCamera const&) = delete;
	void operator=(RTSCamera const&) = delete;

	const glm::mat4& getViewMatrix() const { return viewMatrix; }
	const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }

	void Move(const int& x, const int& y);
	void Zoom(const float& amount);

	//This takes callback from GLFW scroll and calls zoomfunction.
	static void ScrollZoom(GLFWwindow* window, double xamount, double yamount);

private:
	float zoomAmount;
	float zoomSpeed;
	float zoomMin;
	float zoomMax;

	glm::vec2 pos;

	float movespeed;

	float windowWidth;
	float windowHeight;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
};

