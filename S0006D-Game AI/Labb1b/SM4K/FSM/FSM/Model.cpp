#include "Model.h"


Model::Model(float width, float height, GLuint vao, GLuint vbo, GLuint vertexSize, GLuint shaderID)
{
	this->width = width;
	this->height = height;
	this->vao = vao;
	this->vbo = vbo;
	this->vertexSize = vertexSize;
	this->shaderID = shaderID;
	this->MVPlocation = glGetUniformLocation(shaderID, "MVP");
	this->ColorLocation = glGetUniformLocation(shaderID, "uniformColor");
}

Model::Model(float width, float height, GLuint vao, GLuint vbo, GLuint vertexSize, GLuint shaderID, bool line)
{
	this->width = width;
	this->height = height;
	this->vao = vao;
	this->vbo = vbo;
	this->vertexSize = vertexSize;
	this->shaderID = shaderID;
	this->MVPlocation = glGetUniformLocation(shaderID, "MVP");
	this->ColorLocation = glGetUniformLocation(shaderID, "uniformColor");
	this->line = line;
}

Model::~Model()
{
}

void Model::update()
{
}

void Model::draw(const glm::mat4 &MVP, const glm::vec3 &color)
{
	glm::vec3 apa = color;
	//Send Matrix to Shader
	glUniformMatrix4fv(MVPlocation, 1, GL_FALSE, &MVP[0][0]);
	glUniform3fv(ColorLocation, 1, &color[0]);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, vertexSize); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);
}

void Model::draw(const glm::mat4 &MVP, const glm::vec3 &color, bool line)
{
	glm::vec3 apa = color;
	//Send Matrix to Shader
	glUniformMatrix4fv(MVPlocation, 1, GL_FALSE, &MVP[0][0]);
	glUniform3fv(ColorLocation, 1, &color[0]);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Draw the line!
	glDrawArrays(GL_LINE_STRIP, 0, vertexSize); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);
}

