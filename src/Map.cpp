#include "Map.h"
#include "Game.h"

Camera::Camera()
{
}

void Camera::Update()
{
	position = game->player->position;

	visibleTiles =
	{
		game->ScreenWidth() / 16 + 1,
		game->ScreenHeight() / 16
	};

	offset = position - visibleTiles / 2.0f;

	if (position.x < 0.0f) position.x = 0.0f;
	if (position.x > game->map->width - 1) position.x = game->map->width - 1;

	if (offset.x < 0) offset.x = 0;
	if (offset.x > game->map->width - visibleTiles.x) offset.x = game->map->width - visibleTiles.x;
	if (offset.y < 0) offset.y = 0;
	if (offset.y > game->map->height - visibleTiles.y) offset.y = game->map->height - visibleTiles.y;

	tileOffset = (offset - (olc::vi2d)offset) * game->map->tileSize;
}

Map::Map()
{
	tileSize = 16;

	camera = new Camera();

	level = 1;

	LoadLevel();

	m_sInitData = m_sData;

	m_sprTiles = new olc::Sprite("assets/images/WorldTiles.png");
	m_decTiles = new olc::Decal(m_sprTiles);
}

Map::~Map()
{
	delete camera;
}

char Map::GetTile(int x, int y)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
		return m_sData[y * width + x];
	else
		return ' ';
}

char Map::GetTile(const olc::vi2d& id)
{
	return GetTile(id.x, id.y);
}

void Map::SetTile(int x, int y, char c)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
		m_sData[y * width + x] = c;
}

void Map::SetTile(const olc::vi2d& id, char c)
{
	SetTile(id.x, id.y, c);
}

bool Map::IsSolid(int x, int y)
{
	return GetTile(x, y) == '_' ||
		GetTile(x, y) == 't' || GetTile(x, y) == 'T' ||
		GetTile(x, y) == '[' || GetTile(x, y) == ']' ||
		GetTile(x, y) == 'J' || GetTile(x, y) == 'L';
}

bool Map::IsSolid(const olc::vi2d& id)
{
	return IsSolid(id.x, id.y);
}

void Map::Draw()
{
	olc::vf2d bkgOffset;
	std::array<int, 3> bkgHeights;
	float bkgScrollSpeed = game->player->speed.x / 5;

	switch (level)
	{
	case 1: bkgOffset = { 0.0f, 25.0f }; bkgHeights = { 0, 136, 269 }; break;
	case 2: bkgOffset = { 0.0f, -35.0f }; bkgHeights = { 0, 171, 330 }; break;
	case 3: bkgOffset = { 0.0f, 25.0f }; bkgHeights = { 0, 90, 162}; break;
	case 4: bkgOffset = { 0.0f, 0.0f }; bkgHeights = { 0, 120, 201}; break;
	}

	for (int i = 0; i < 3; i++)
	{
		if (game->energyCounter >= 30)
		{
			if (level == 4)
				game->DrawDecal(olc::vf2d(i * 480 - camera->offset.x, bkgHeights[0]) + bkgOffset, mBkgLayers[0].Decal());
			else
				game->DrawDecal(olc::vf2d(i * 480 - camera->offset.x, bkgHeights[0]) + bkgOffset, mBkgLayers[1].Decal());
		}
		if (game->energyCounter >= 40)
		{
			if (level == 4)
				game->DrawDecal(olc::vf2d(i * 480 - (bkgScrollSpeed - 1) * camera->offset.x, bkgHeights[1]) + bkgOffset, mBkgLayers[1].Decal());
			else
				game->DrawDecal(olc::vf2d(i * 480 - (bkgScrollSpeed - 1) * camera->offset.x, bkgHeights[1]) + bkgOffset, mBkgLayers[2].Decal());
		}	
		if (game->energyCounter >= 50 && level != 3)
		{
			if (level == 4)
				game->DrawDecal(olc::vf2d(i * 480 - bkgScrollSpeed * camera->offset.x, bkgHeights[2]) + bkgOffset, mBkgLayers[2].Decal());
			else
				game->DrawDecal(olc::vf2d(i * 480 - bkgScrollSpeed * camera->offset.x, bkgHeights[2]) + bkgOffset, mBkgLayers[3].Decal());
		}

		if (level == 4)
		{
			if (game->energyCounter >= 60)
				game->DrawDecal(olc::vf2d(i * 480 - (bkgScrollSpeed + 1) * camera->offset.x, bkgHeights[2] + 81) + bkgOffset, mBkgLayers[3].Decal());
		}
	}
	
	for (int y = -1; y <= camera->visibleTiles.y; y++)
	{
		for (int x = -1; x < camera->visibleTiles.x; x++)
		{
			char tileID = GetTile(x + camera->offset.x, y + camera->offset.y);
			olc::vi2d sprSheetID;
			olc::vi2d sprSheetIDOffset =
			{
				((level - 1) % 2) * 14,
				(level / 3) * 14
			};

			olc::vi2d size = olc::vi2d(tileSize, tileSize);

			// Ground
			if (game->energyCounter < 20)
			{
				if (tileID == 't' || tileID == 'T' || tileID == '[' || tileID == '8' ||
					tileID == ']' || tileID == 'J' || tileID == 'L' || tileID == '_')
				{
					olc::Pixel color;

					switch (level)
					{
					case 1: color = olc::GREEN; break;
					case 2: color = olc::WHITE; break;
					case 3: color = olc::Pixel(252, 216, 158); break;
					case 4: color = olc::Pixel(228, 187, 206); break;
					}

					game->FillRectDecal(olc::vf2d(x, y) * tileSize - camera->tileOffset, olc::vi2d(tileSize, tileSize), color);
				}
			}
			else
			{
				switch (tileID)
				{					
				case 't': sprSheetID = (olc::vi2d(11, 7) + sprSheetIDOffset) * 16 + 1; break;
				case 'T': sprSheetID = (olc::vi2d(13, 7) + sprSheetIDOffset) * 16 + 1; break;
				case '[': sprSheetID = (olc::vi2d(11, 8) + sprSheetIDOffset) * 16 + 1; break;
				case '8': sprSheetID = (olc::vi2d(12, 8) + sprSheetIDOffset) * 16 + 1; break;
				case ']': sprSheetID = (olc::vi2d(13, 8) + sprSheetIDOffset) * 16 + 1; break;
				case 'J': sprSheetID = (olc::vi2d(11, 9) + sprSheetIDOffset) * 16 + 1; break;
				case 'L': sprSheetID = (olc::vi2d(13, 9) + sprSheetIDOffset) * 16 + 1; break;
				case '_': sprSheetID = (olc::vi2d(12, 7) + sprSheetIDOffset) * 16 + 1; break;
				}

				game->DrawPartialDecal(olc::vf2d(x, y) * tileSize - camera->tileOffset, size, m_decTiles, sprSheetID, olc::vi2d(tileSize, tileSize));
			}
			
			if (game->energyCounter >= 60)
			{				
				switch (tileID)
				{
				// Tree
				case '0': size = olc::vf2d(tileSize, tileSize) * 2; sprSheetID = (olc::vi2d(11, 4) + sprSheetIDOffset) * 16 + 1; break;
				case 'O': size = olc::vf2d(tileSize, tileSize) * 2; sprSheetID = (olc::vi2d(12, 4) + sprSheetIDOffset) * 16 + 1; break;
				case 'i': size = olc::vf2d(tileSize, tileSize) * 2; sprSheetID = (olc::vi2d(11, 5) + sprSheetIDOffset) * 16 + 1; break;
				case 'I': size = olc::vf2d(tileSize, tileSize) * 2; sprSheetID = (olc::vi2d(12, 5) + sprSheetIDOffset) * 16 + 1; break;

				// Bush
				case 'b': sprSheetID = olc::vi2d(13, 5) * 16 + 1; break;

				// Fence
				case 'H': size = olc::vi2d(tileSize, tileSize) * 2; sprSheetID = (olc::vi2d(11, 6) + sprSheetIDOffset) * 16 + 1; break;
				case 'h': size = olc::vi2d(tileSize, tileSize) * 2; sprSheetID = (olc::vi2d(12, 6) + sprSheetIDOffset) * 16 + 1; break;
				case 'E': size = olc::vi2d(tileSize, tileSize) * 2; sprSheetID = (olc::vi2d(13, 6) + sprSheetIDOffset) * 16 + 1; break;
				}

				game->DrawPartialDecal(olc::vf2d(x, y) * tileSize - camera->tileOffset, size, m_decTiles, sprSheetID, olc::vi2d(tileSize, tileSize));
			}

			olc::Pixel coinTint;
			switch (tileID)
			{
			case '1': game->DrawPartialDecal(olc::vf2d(x, y) * tileSize - camera->tileOffset, size, CoinPickUp::sDecal, olc::vi2d(CoinPickUp::sTileID.x, 0) * 16, olc::vi2d(tileSize, tileSize), olc::GREEN); break;
			case '2': game->DrawPartialDecal(olc::vf2d(x, y) * tileSize - camera->tileOffset, size, CoinPickUp::sDecal, olc::vi2d(CoinPickUp::sTileID.x, 0) * 16, olc::vi2d(tileSize, tileSize), olc::WHITE); break;
			case '3': game->DrawPartialDecal(olc::vf2d(x, y) * tileSize - camera->tileOffset, size, CoinPickUp::sDecal, olc::vi2d(CoinPickUp::sTileID.x, 0) * 16, olc::vi2d(tileSize, tileSize), olc::CYAN); break;
			case '4': game->DrawPartialDecal(olc::vf2d(x, y) * tileSize - camera->tileOffset, size, CoinPickUp::sDecal, olc::vi2d(CoinPickUp::sTileID.x, 0) * 16, olc::vi2d(tileSize, tileSize), olc::YELLOW); break;
			}			
		}
	}

	
}

void Map::ReadFile(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss;

	int lineCount = 0;
	while (getline(stream, line))
	{
		width = line.size();
		ss << line;
		++lineCount;
	}

	height = lineCount;

	m_sData = ss.str();
}

void Map::Reset()
{
	if (m_sData != m_sInitData)
		m_sData = m_sInitData;
}

void Map::LoadLevel()
{
	ReadFile("assets/levels/" + std::to_string(level) + ".lvl");	

	for (int i = 0; i < mBkgLayers.size(); i++)
		mBkgLayers[i].Load("assets/images/background/Level" + std::to_string(level) + "/layer" + std::to_string(i) + ".png");

	for (int i = 0; i < game->coinTypes.size(); i++)
	{
		game->coinTypes[i] = 0;
	}

	game->energyCounter = 0;
}

