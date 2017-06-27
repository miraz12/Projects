#include "Pathfinder.h"
#include <iostream>     
#include <fstream>      
#include <assert.h>
#include <string>



bool Pathfinder::ValidNeighbour(int x, int y, int NumCellsX, int NumCellsY)
{
	return !((x < 0) || (x >= NumCellsX) || (y < 0) || (y >= NumCellsY));
}

Pathfinder::Pathfinder()
{
}


Pathfinder::~Pathfinder()
{
}

void Pathfinder::Load(char* filename)
{
	std::ifstream load(filename);

	assert(load && "Load ERROR");



	std::string line;

	int x = 0;
	int y = 0;


	while (std::getline(load, line))
	{
		for (x = 0; x < line.length(); x++)
		{
			
			NavNode node;
			glm::vec2 position = glm::vec2(x, y);
			std::vector<int> posVec(x, y);
			node.setIndex(graph.getNextNodeIndex());
			node.setPos(position);




			std::map<std::vector<int>, int> tempMap;

			tempMap[posVec] = 0;

			//Set if coord is occupied or not. 
			coordsVec.push_back(tempMap);

			node.addTiles();




			if (line[x] == 'O')
			{
				node.setFlag(goal);
				node.setType(normal);
				setGoalNode(node);
			}
			if (line[x] == 'B')
			{
				node.setFlag(unwalkable);
				node.setType(mountain);
			}
			if (line[x] == 'M')
			{
				node.setFlag(walkable);
				node.setType(normal);

			}
			if (line[x] == 'G')
			{
				node.setFlag(walkable);
				node.setType(marsh);
			}
			if (line[x] == 'T')
			{
				node.setFlag(walkable);
				node.setType(tree);		
				node.addTilesTree();
				node.setWood(5);
			}
			if (line[x] == 'V')
			{
				node.setFlag(unwalkable);
				node.setType(water);
			}

			graph.AddNode(node);

			if (node.getFlag() != start)
			{
				node.setFlag(unwalkable);
			}

			discoveredGraph.AddNode(node);
		}
		y++;
		
	}
	graphWidth = x;
	graphHeight = y;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 10000);
	int ranX = dis(gen);

	while (!(graph.getNode(ranX).getFlag() != unwalkable && graph.getNode(ranX).getType() == normal))
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 10000);
		int ranX = dis(gen);
	}

	graph.getNode(ranX).setFlag(start);
	setStart(graph.getNode(ranX));

	graph.getNode(ranX).setType(normal);


	for (int i = 0; i < graph.getNumNodes(); i++)
	{

		float col = graph.getNode(i).getPos().x;
		float row = graph.getNode(i).getPos().y;

		for (int j = -1; j < 2; ++j)
		{
			for (int k = -1; k < 2; ++k)
			{
				float nodeX = col + k;
				float nodeY = row + j;

				if ((j == 0) && (k == 0)) continue;

				if (ValidNeighbour(nodeX, nodeY, graphWidth, graphHeight) && graph.getNode(i).getFlag() == walkable)
				{
					//calculate the distance to this node
					glm::vec2 PosNode = graph.getNode(row*graphWidth + col).getPos();
					glm::vec2 PosNeighbour = graph.getNode(nodeY*graphWidth + nodeX).getPos();

					if (graph.getNode(nodeY*graphWidth + nodeX).getFlag() == unwalkable)
					{

					}
					else
					{
						double dist = glm::distance(PosNode, PosNeighbour);

						if (graph.getNode(nodeY*graphWidth + nodeX).getType() == marsh)
						{
							dist *= 2;
						}
						//this neighbour is okay so it can be added
						NavEdge NewEdge(row*graphWidth + col, nodeY*graphWidth + nodeX, dist);
						NewEdge.setCost(dist);
						graph.AddEdge(NewEdge);

						//Not directed add edges both ways.
						//NavEdge NewEdgeBack(nodeY*graphWidth + nodeX, row*graphWidth + col, dist);
						//NewEdgeBack.setCost(dist);
						//graph.AddEdge(NewEdgeBack);

					}



				}
			}
		}

	}


}
