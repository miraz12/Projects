#include "Object.h"
#include "RTSCamera.h"

Object::Object()
{
}

Object::Object(const glm::vec3 &color)
{
	this->color = color;
	this->physics = false;
}

Object::~Object()
{
}

void Object::setPosition(GLfloat posX, GLfloat posY)
{
	this->posX = posX;
	this->posY = posY;
	ModelMatrix = getModelMatrix();
	
}

void Object::UpdatePos()
{
	ModelMatrix = getModelMatrix();
}

void Object::setColor(glm::vec3 color)
{
	this->color = color;
}

glm::mat4 Object::getModelMatrix()
{
	return glm::translate(glm::vec3(posX, posY, 0.0f));
}

void Object::setModel(Model* model)
{
	this->model = model;
}

void Object::draw()
{
	if (!static_model.getLine())
	{
		if (model->getLine())
			model->draw(ModelMatrix, color, true);
		else
			model->draw(ModelMatrix, color);
	}
	else
	{
		static_model.draw(ModelMatrix, color, true);

	}
	
}


/********************************
*	LOGIC THAT AFFECTS ALL		*
*	GEOMETRY ON THE SCENE		*
*	GOES HERE!					*
*********************************/

void Object::update(float deltaTime)
{
}