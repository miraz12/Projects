#include "GraphicsNode.h"
#include "config.h"
#include <ctime>




GraphicsNode::GraphicsNode()
{
	mesh = std::make_shared<MeshResource>();
	texture = std::make_shared<TextureResource>("frooog.jpg");
	shader = std::make_shared<ShaderObject>("VertexShader.vs", "FragmentShader.fs");
	light = std::make_shared<LightNode>();
	
	mesh->loadOBJ("cubeL.obj");
	mesh->Setup();

    camera.setValues(0, -5, 5);
    origin.setValues(0.0f, 0.0f, 0.0f);
    headUp.setValues(0.0f, 1, 0.0f);

	mesh->convertToHalfEdges(10);

    std::clock_t start;
    double duration;
    start = std::clock();
        mesh->catmullClark();
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    printf("%f seconds\n", duration);

    mesh->DrawHalfEdge();
}

GraphicsNode::GraphicsNode(std::shared_ptr<MeshResource> me, std::shared_ptr<TextureResource> tex, std::shared_ptr<ShaderObject> sha)
{
    mesh = me;
    texture = tex;
    shader = sha;
    light = std::make_shared<LightNode>();

    camera.setValues(0, -5, 5);
    origin.setValues(0.0f, 0.0f, 0.0f);
    headUp.setValues(0.0f, 1, 0.0f);
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
	mesh->DrawOBJ();
    //mesh->DrawCube();

}

//Takes a MVP matrix
void GraphicsNode::draw(matrix4D model)
{

    texture->bind();
    shader->bind();

    this->mod = model;


    shader->modVector3f("in_lightPos", light->getPos());
    shader->modVector3f("in_color", (light->getColor() * light->getIntensity()));

    shader->modMatrix4fv("mvp", model);
    shader->modMatrix4fv("model", model);

    //mesh->DrawQuad();
    mesh->DrawOBJ();
    //mesh->DrawCube();

}

void GraphicsNode::draw()
{
	texture->bind();
	shader->bind();

    matrix4D projection = (projection.setPerspective(45.0f, (600.0f / 600.0f), 0.001f, 10000.0f));
    matrix4D view = view.LookAtRH(this->camera, this->origin, this->headUp);

    matrix4D mvp = projection * view * this->mod;

	shader->modVector3f("in_lightPos", light->getPos());
	shader->modVector3f("in_color", (light->getColor() * light->getIntensity()));

	shader->modMatrix4fv("mvp", mvp);
	shader->modMatrix4fv("model", this->mod);

	mesh->DrawOBJ();

}

void GraphicsNode::setProjec(const matrix4D &projec) {
	GraphicsNode::projec = projec;
}

void GraphicsNode::setMod(const matrix4D &mod) {
	GraphicsNode::mod = mod;
}
const matrix4D &GraphicsNode::getProjec() const {
	return projec;
}

matrix4D &GraphicsNode::getMod() {
	return mod;
}



