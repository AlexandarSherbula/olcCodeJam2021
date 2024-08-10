#ifndef GAME_H
#define GAME_H

#include "olcPixelGameEngine.h"

#include "Map.h"
#include "Object.h"

#include <array>

enum GameState
{
	START,
	GAMEPLAY,
	END
};

class Game : public olc::PixelGameEngine
{
public:
	Game();
	~Game();	

	Map* map = nullptr;
	Player* player = nullptr;

	olc::Renderable endingSprite;

	std::array<int, 4> coinTypes;
	std::array<bool, 4> levelsDone;
	int energyCounter;
	int startPhaseText = 0;

	bool levelComplete;

	GameState state;
public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	void NextLevel();

	void StartText();
};

inline Game* game = new Game();

#endif // !GAME_H