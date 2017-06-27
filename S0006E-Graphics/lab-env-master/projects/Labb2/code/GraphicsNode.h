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
		/// destructor
		~GraphicsNode();

		std::shared_ptr<MeshResource> getMesh();
		void setMesh(std::shared_ptr<MeshResource>);

		std::shared_ptr<TextureResource> getTexture();
		void setTexture(std::shared_ptr<TextureResource>);

		std::shared_ptr<ShaderObject> getShader();
		void setShader(std::shared_ptr<ShaderObject>);

		void draw(matrix4D, matrix4D);

	private:
		std::shared_ptr<MeshResource> mesh;
		std::shared_ptr<TextureResource> texture;
		std::shared_ptr<ShaderObject> shader;
		std::shared_ptr<LightNode> light;

		

	};
