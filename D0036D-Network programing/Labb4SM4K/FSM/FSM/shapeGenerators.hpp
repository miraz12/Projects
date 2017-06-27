#pragma once
#include "Model.h"

namespace shapeGenerator {

	Model square(float width, float height, GLuint shaderID);

	Model triangle(float base, float height, GLuint shaderID);


	Model line(glm::vec2 startPos, glm::vec2 endPos, GLuint shaderID);
	//	return Model()
		
	//}
}

