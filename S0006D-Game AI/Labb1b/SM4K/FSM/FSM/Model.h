#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>
class Model
{
private:
	//Original dimensions
	float width;
	float height;

	//If the model is a line or not
	bool line = false;

	//openGL variables
	GLuint vbo;
	GLuint vao;
	GLuint vertexSize;
	GLuint MVPlocation;
	GLuint ColorLocation;
	GLuint shaderID;
public:
	Model(float width, float height, GLuint vao, GLuint vbo, GLuint vertexSize, GLuint shaderID);
	Model(float width, float height, GLuint vao, GLuint vbo, GLuint vertexSize, GLuint shaderID, bool line);
	~Model();
	void update();
	void draw(const glm::mat4 &MVP, const glm::vec3 &color);
	void draw(const glm::mat4 &MVP, const glm::vec3 &color, bool line);
	
	//
	bool getLine(){ return line; }

	//Dimension getters
	float getWidth() { return width; }
	float getHeight() { return height; }
};

