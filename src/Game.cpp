#include "Game.h"
#include <sstream>
#include <fstream>

Game::Game()
{
	sAppName = "Save the Worlds";
}

Game::~Game()
{	
	delete player;
	delete map;

	CoinPickUp::DeleteSprite();
}

bool Game::OnUserCreate()
{
	CoinPickUp::CreateSprite();

	map = new Map();
	player = new Player(olc::vf2d(1.0f, 17.0f), olc::vf2d(20.0f, 13.0f), olc::BLUE, olc::vi2d(map->tileSize, map->tileSize));

	state = GameState::START;

	for (int i = 0; i < coinTypes.size(); i++)
	{
		coinTypes[i] = 0;
		levelsDone[i] = false;
	}

	endingSprite.Load("assets/images/background/TheEnd.png");

	energyCounter = 0;

	levelComplete = false;

	return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
	if (state == START)
	{
		StartText();
	}
	else if (state == GAMEPLAY)
	{
		player->Behaviour(fElapsedTime);

		CoinPickUp::Animation(fElapsedTime);

		map->camera->Update();

		if (energyCounter >= 80)
		{
			levelsDone[map->level - 1] = true;

			if (levelsDone[0] && levelsDone[1] && levelsDone[2] && levelsDone[3])
				state = END;
			else
				NextLevel();
		}

		switch (map->level)
		{
		case 1: case 3: Clear(olc::Pixel(66, 154, 215)); break;
		case 2: case 4: Clear(olc::Pixel(136, 100, 147)); break;
		}

		map->Draw();

		player->Draw();

		DrawStringDecal(olc::vi2d(0, 0), std::to_string(energyCounter) + "/80", olc::WHITE, {2.0f, 2.0f});

	}
	else if (state == END)
	{
		Clear(olc::Pixel(66, 154, 215));

		DrawDecal(olc::vi2d(144, 18), endingSprite.Decal(), {1.0f, 1.0f});

		DrawStringDecal(olc::vi2d(48, 144), "Well Done! The worlds are saved and", olc::BLACK, { 2.0f, 2.0f });
		DrawStringDecal(olc::vi2d(136, 164), "the future is secured", olc::BLACK, { 2.0f, 2.0f });


		game->DrawStringDecal(olc::vf2d(144, 288), "Thank you for playing", olc::BLACK, { 2.0f, 2.0f });
	}	

	return !GetKey(olc::ESCAPE).bPressed;
}

void Game::NextLevel()
{
	int maxCount = std::max(std::max(coinTypes[0], coinTypes[1]), std::max(coinTypes[2], coinTypes[3]));

	if (maxCount == coinTypes[0])	   map->level = 1;
	else if (maxCount == coinTypes[1]) map->level = 2;
	else if (maxCount == coinTypes[2]) map->level = 3;
	else if (maxCount == coinTypes[3]) map->level = 4;

	if (map->level == 3)
		player->position = olc::vf2d(1.0f, 5.0f);
	else
		player->position = olc::vf2d(1.0f, 17.0f);

	map->LoadLevel();
}

void Game::StartText()
{
	if (GetKey(olc::ENTER).bReleased)
	{
		if (startPhaseText >= 5)
			state = GAMEPLAY;
		else
			startPhaseText++;
	}

	Clear(olc::Pixel(66, 154, 215));

	if (startPhaseText == 0)
	{
		game->DrawStringDecal(olc::vf2d(16, 96), "Hello, welcome to this olcCodeJam 2021", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(96, 116), "entry! There is little time", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(32, 136), "for introduction, i need your help!", olc::BLACK, { 2.0f, 2.0f });
	}
	else if (startPhaseText == 1)
	{
		game->DrawStringDecal(olc::vf2d(24, 96),  "You may notice there are no graphics", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(48, 116), "occuring! This is because of the", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(40, 136), "virus that has spread across four", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(64, 156),  "worlds and has sucked out all", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(208, 176), "of their life!", olc::BLACK, { 2.0f, 2.0f });
	}
	else if (startPhaseText == 2)
	{
		game->DrawStringDecal(olc::vf2d(0, 96), "However, this virus has contained all of", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(0, 116), "the world's life energy into small coins,", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(24, 136), "thus the future of these worlds is not", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(96, 156), "determined by the virus yet!", olc::BLACK, { 2.0f, 2.0f });
	}
	else if (startPhaseText == 3)
	{
		game->DrawStringDecal(olc::vf2d(16, 96), "These coins also seem to be the key to", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(48, 116), "warp you to another world. Very", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(96, 136), "convenient for your goal.", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(120, 156), "Which, speaking of....", olc::BLACK, { 2.0f, 2.0f });
	}
	else if (startPhaseText == 4)
	{
		game->DrawStringDecal(olc::vf2d(8, 96), "Your goal is to collect these coins and", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(40, 116), "restore the world's life energy in", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(32, 136), "order to secure the future of these", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(264, 156), "worlds!", olc::BLACK, { 2.0f, 2.0f });
	}
	else if (startPhaseText == 5)
	{
		game->DrawStringDecal(olc::vf2d(16, 96), "I know you may have a lot of questions", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(16, 116), "but there is no time to explain! All i", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(32, 136), "can tell you is some coins warp you", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(24, 156), "elsewhere but others return you back!", olc::BLACK, { 2.0f, 2.0f });
		game->DrawStringDecal(olc::vf2d(64, 176), "You must go now! Best of luck!", olc::BLACK, { 2.0f, 2.0f });
	}

	game->DrawStringDecal(olc::vf2d(128, 288), "Press enter to continue", olc::BLACK, { 2.0f, 2.0f });
}
