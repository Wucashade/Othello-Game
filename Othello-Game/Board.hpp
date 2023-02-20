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
	void resize();
	int getHeight();
	int getWidth();

	SDL_Color outlineColour;
	SDL_Color boxColour;





private:


	
	int boxWidth;
	int boxHeight;

};