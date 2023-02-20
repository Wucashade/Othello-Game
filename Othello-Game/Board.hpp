#pragma once

#include <SDL.h>
#include <iostream>
#include "define.hpp"

class Board
{

public:
	Board();
	~Board();

	void init();
	void renderBoard();
	void render();
	void update();

	SDL_Color boardColour1;
	SDL_Color boardColour2;




private:


	
	int boxWidth;
	int boxHeight;

};