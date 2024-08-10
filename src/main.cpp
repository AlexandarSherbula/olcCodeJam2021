#include "Game.h"

int main()
{	
	if(game->Construct(640, 320, 2, 2, false, false))
		game->Start();	
	delete game;
	return 0;
}