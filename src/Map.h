#ifndef MAP_H
#define MAP_H

#include "olcPixelGameEngine.h"
#include "Object.h"

#include <array>
#include <vector>

struct Camera
{
public:
	olc::vf2d position;
	olc::vi2d visibleTiles;
	olc::vf2d offset;
	olc::vf2d tileOffset;
public:
	Camera();

	void Update();
};

class Map
{
public:
	int width;
	int height;

	int tileSize;

	Camera* camera = nullptr;

	int level;
private:
	std::string m_sData;
	std::string m_sInitData;

	std::array<olc::Renderable, 4> mBkgLayers;
	olc::Sprite* m_sprTiles;
	olc::Decal* m_decTiles;

public:
	Map();
	~Map();

	char GetTile(int x, int y);
	char GetTile(const olc::vi2d& id);
		
	void SetTile(int x, int y, char c);
	void SetTile(const olc::vi2d& id, char c);

	bool IsSolid(int x, int y);
	bool IsSolid(const olc::vi2d& id);

	void Draw();

	void Reset();

	void LoadLevel();
private:
	void ReadFile(const std::string& filepath);
};



#endif // !MAP_H