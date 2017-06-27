#pragma once
#include "GL/glew.h"
#include "ShaderObject.h"
#include "TextureResource.h"
#include "MeshResource.h"
#include "LightNode.h"
#include <memory>



class GraphicsNode
	{
	public:
		/// constructor
		GraphicsNode();
		GraphicsNode(std::shared_ptr<MeshResource> me, std::shared_ptr<TextureResource> tex, std::shared_ptr<ShaderObject> sha);
		/// destructor
		~GraphicsNode();

		std::shared_ptr<MeshResource> getMesh();
		void setMesh(std::shared_ptr<MeshResource>);

		std::shared_ptr<TextureResource> getTexture();
		void setTexture(std::shared_ptr<TextureResource>);

		std::shared_ptr<ShaderObject> getShader();
		void setShader(std::shared_ptr<ShaderObject>);

		void draw(matrix4D, matrix4D);
		void drawAnim(matrix4D model, matrix4D PV, matrix4D* bone);
		void draw();
		void draw(matrix4D);



private:
		std::shared_ptr<MeshResource> mesh;
		std::shared_ptr<TextureResource> texture;
		std::shared_ptr<ShaderObject> shader;
		std::shared_ptr<LightNode> light;

		matrix4D projec, mod;

public:
	const matrix4D &getProjec() const;

	matrix4D& getMod();
	vector3D camera;
	vector3D origin;
	vector3D headUp;

	void setProjec(const matrix4D &projec);

	void setMod(const matrix4D &mod);


};
