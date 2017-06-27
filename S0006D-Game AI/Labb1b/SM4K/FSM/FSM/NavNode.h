#pragma once
#include "GraphNode.h"
#include <glm.hpp>

///Node type, contains pos and node falg. ex: walkable, unwalkable
class NavNode : public GraphNode
{
private:
	///Postition coords
	glm::vec2 pos;

	///Node flag
	int flag;


public:
	NavNode(){};
	NavNode(int i, glm::vec2 p) : GraphNode(i), pos(p) {};
	virtual ~NavNode(){};

	glm::vec2 getPos() const { return pos; }
	void setPos(glm::vec2 val) { pos = val; }

	int getFlag() const { return flag; }
	void setFlag(int val) { flag = val; }
};

