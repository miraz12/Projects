#pragma once
#include "GraphicsNode.h"
#include "config.h"
#include "../../GrafikLabb/code/GraphicsNode.h"


GraphicsNode::GraphicsNode()
{
	mesh = std::make_shared<MeshResource>();
	texture = std::make_shared<TextureResource>("kkdht.png.jpg");
	shader = std::make_shared<ShaderObject>("VertexShader.vs", "FragmentShader.fs");
	light = std::make_shared<LightNode>();
	
	mesh->loadOBJ("KingdomKeyDHT.obj");
	mesh->Setup();
}


GraphicsNode::~GraphicsNode()
{
	
}

std::shared_ptr<MeshResource> GraphicsNode::getMesh()
{
	return mesh;
}

void GraphicsNode::setMesh(std::shared_ptr<MeshResource> m)
{
	mesh = m;
}

std::shared_ptr<TextureResource> GraphicsNode::getTexture()
{
	return texture;
}

void GraphicsNode::setTexture(std::shared_ptr<TextureResource> t)
{
	texture = t;
}

std::shared_ptr<ShaderObject> GraphicsNode::getShader()
{
	return shader;
}

void GraphicsNode::setShader(std::shared_ptr<ShaderObject> s)
{
	shader = s;
}

void GraphicsNode::draw(matrix4D projection, matrix4D model)
{


	
	texture->bind();
	shader->bind();
	//matrix4D view = view.setFrustum(80.0f, (800.0f/600.0f), 0.01f, 1000.0f);

	

	shader->modVector3f("in_lightPos", light->getPos());
	shader->modVector3f("in_color", (light->getColor() * light->getIntensity()));
	
	shader->modMatrix4fv("projection", projection);
	shader->modMatrix4fv("model", model);

	//mesh->DrawQuad();
	//mesh->DrawOBJ();
    mesh->DrawCube();

}





