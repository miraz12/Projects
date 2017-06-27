#pragma once
#include "GraphNode.h"
#include "tileType.h"
#include <glm.hpp>
#include <iostream>
#include <vector>
#include <random>

///Node type, contains pos and node falg. ex: walkable, unwalkable
class NavNode : public GraphNode
{
private:
	///Postition coords
	glm::vec2 pos;

	///Node flag
	int flag;
	int type;
	int wood;
	bool occupied = false;


	std::vector<int> nodeTiles;




public:
	NavNode(){ };
	NavNode(int i, glm::vec2 p) : GraphNode(i), pos(p) {  };
	virtual ~NavNode(){};

	glm::vec2& getPos() { return pos; }
	void setPos(glm::vec2 val) { pos = val; }

	int& getFlag() { return flag; }
	void setFlag(int val) { flag = val; }

	int getType() const { return type; }
	void setType(int val) { type = val; }

	int& getWood() { return wood; }
	void setWood(int val) { wood = val; }
	bool chopWood(){
		if (wood <= 0)
		{
			type = normal;
			return false;
		}
		else
		{
			std::cout << "\n" << wood << "\n";
			wood--;
			return true;
		}
	};

	void chop()
	{
		if (wood <= 0)
		{
			type = normal;

		}
		else
		{
			std::cout << "\n" << wood << "\n";
			wood--;

		}
	}
	bool getOccupied() const { return occupied; }
	void setOccupied(bool val) { occupied = val; }

	void addTiles()
	{
		for (int i = 0; i < 9; i++)
		{
			nodeTiles.push_back(0);
		}
	}
	void addTilesTree()
	{
		for (int i = 0; i < 5; i++)
		{
			int ranI;

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 8);
			ranI = dis(gen);
			while (true)
			{
				if (nodeTiles[ranI] == 1)
				{
					std::random_device rd;
					std::mt19937 gen(rd());
					std::uniform_int_distribution<> dis(0, 8);
					ranI = dis(gen);

				}
				else
				{
					nodeTiles[ranI] = 1;
					break;
				}
			}




		}


	}


	std::vector<int>& getNodeTiles() { return nodeTiles; }
};



