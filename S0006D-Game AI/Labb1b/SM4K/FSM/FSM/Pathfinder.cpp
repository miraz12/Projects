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
			node.setIndex(graph.getNextNodeIndex());
			node.setPos(glm::vec2(x , y));


			if (line[x] == 'S')
			{
				node.setFlag(start);
				setStart(node);
			}
			if (line[x] == 'G')
			{
				node.setFlag(goal);
				setGoalNode(node);
			}
			if (line[x] == 'X')
			{
				node.setFlag(unwalkable);
			}
			if (line[x] == '0')
			{
				node.setFlag(walkable);
			}
			graph.AddNode(node);

		}
		y++;

	}
	graphWidth = x;
	graphHeight = y;




	for (int i = 0; i < graph.getNumNodes(); i++)
	{
		
		int col = graph.getNode(i).getPos().x;
		int row = graph.getNode(i).getPos().y;

		for (int j = -1; j < 2; ++j)
		{
			for (int k = -1; k < 2; ++k)
			{
				int nodeX = col + k;
				int nodeY = row + j;

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

						//this neighbour is okay so it can be added
						NavEdge NewEdge(row*graphWidth + col, nodeY*graphWidth + nodeX, dist);
						NewEdge.setCost(dist);
 						graph.AddEdge(NewEdge);

						//Not directed add edges both ways.
						NavEdge NewEdgeBack(nodeY*graphWidth + nodeX, row*graphWidth + col, dist);
						NewEdge.setCost(dist);
						graph.AddEdge(NewEdgeBack);
					}



				}
			}
		}

	}


}
