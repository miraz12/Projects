#pragma once
#include <GL\glew.h>
#include <glm.hpp>
#include <gtx\transform.hpp>
#include <iostream>
#include "Model.h"
#include "GameHandler.h"
class Object
{
private:
	bool physics;
	Model* model;
	glm::mat4 ModelMatrix;

	

protected:
	GLfloat posX;
	GLfloat posY;
	glm::vec3 color;
public:
	Model static_model;

	Object();
	Object(const glm::vec3 &color);
	~Object();
	void setModel(Model* model);

	glm::mat4 getModelMatrix();
	//Using models draw function
	void draw();
	//Update function, all logic goes here!
	virtual void update(float deltaTime);

	//Dimension functions
	float getWidth() { return model->getWidth(); }
	float getHeight() { return model->getHeight(); }

	void setColor(glm::vec3 color);

	//Position functions
	void setPosition(GLfloat posX, GLfloat posY);
	void UpdatePos();
	//Increment position
	void incrementX(GLfloat x) { posX += x; }
	void incrementY(GLfloat y) { posY += y; }
	//Setters and getters
	void setX(GLfloat x) { posX = x; }
	void setY(GLfloat y) { posY = y; }
	GLfloat getX() { return posX; }
	GLfloat getY() { return posY; }

	//Physics functions
	void enablePhysics(bool boolean) { physics = boolean; }
};

