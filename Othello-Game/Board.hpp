#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "define.hpp"

typedef uint64_t U64;

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
	U64 getBitboard(U64 bb);
	void renderDisks(U64 bbOne, U64 bbTwo);
	void drawImage(const string& imagePath, int x, int y);
	U64 shiftOne(U64 bb, int dir8);
	int popCount(U64 bb);
	U64 generateMoves(U64 bbOwn, U64 bbOpponent);
	bool isValid(U64 bb, int index);
	void commitMove(U64 *bbOwn, U64 *bbOpponent, int index);
	void handleMouseButtonDown(SDL_MouseButtonEvent& b, U64 bbOwn, U64 bbOpponent);
	void printSingleBitboard(U64 bb);
	void printBitboard(U64 bbOwn, U64 bbOpponent);
	
	

	SDL_Color outlineColour;
	SDL_Color boxColour;

	U64 bitboardWhite;
	U64 bitboardBlack;

	

private:

	int boxWidth;
	int boxHeight;

};