#pragma once
#include <vector>


struct Tile
{
	//TODO: Flags should be done with bitstream

	bool passable; // is this tile a wall or not?
	bool start = false;
	bool goal = false;
	unsigned int cost = 0; //Higher means slower movement
};

class Map
{
private:
	std::vector<std::vector<Tile>> tiles;

public:
	Map();
	~Map();

	char* filename;
	unsigned int width;
	unsigned int height;

	const Tile& getTile(unsigned int x, unsigned int y);
	void setTile(unsigned int x, unsigned int y, Tile tile);

	void mapTrim(); //Should always be called after setting up map!
};

inline Map::Map()
{
	filename = NULL;
	width = 0;
	height = 0;

	//HACK: MAX SIZE IS 1000x1000 FOR NOW
	tiles.resize(1000);
	for (int i = 0; i < 1000; i++)
		tiles[i].resize(1000);
}

inline Map::~Map()
{
}

inline const Tile& Map::getTile(unsigned int x, unsigned int y)
{
	return tiles[x][y];
}

inline void Map::setTile(unsigned int x, unsigned int y, Tile tile)
{
	tiles[x][y] = tile;
}

inline void Map::mapTrim()
{
	tiles.resize(width);
	for (unsigned int i = 0; i < width; i++)
		tiles[i].resize(height);
}
