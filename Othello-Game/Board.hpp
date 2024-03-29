#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "define.hpp"
#include "Player.hpp"
#include <string>
#pragma once

typedef uint64_t U64;

class Board
{

public:
	Board();
	~Board();

	void init();
	void renderBoard();
	void renderMenu(Player* playerOne, Player* playerTwo, string currentTurn);
	void render(Player* playerOne, Player* playerTwo, string currentTurn);
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
	bool isValid(U64 bbOwn, U64 bbOpponent, int index);
	void commitMove(U64 *bbOwn, U64 *bbOpponent, int index);
	bool handleMouseButtonDown(SDL_MouseButtonEvent& b, Player* bbOwn, Player* bbOpponent);
	void printSingleBitboard(U64 bb);
	void printBitboard(U64 bbOwn, U64 bbOpponent);
	int frontierDiscs(U64 bbOwn, U64 bbOpponent);
	int discSquareValue(U64 bbOwn, U64 bbOpponent);
	int evaluateMove(U64 bbOwn, U64 bbOpponent, U64 ownMoves, U64 oppMoves);
	int searchMove(U64 bbOwn, U64 bbOpponent, int maxDepth, int alpha, int beta, int *bestMove, int *evaluationCount);
	int iterativeSearchMove(U64 &bbOwn, U64 &bbOpponent, int startDepth, int evaluationBudget);
	void computeMove(U64 &bbOwn, U64 &bbOpponent, int *row, int *col);


	SDL_Color outlineColour;
	SDL_Color boxColour;


	U64 bitboardWhite = 0x1008000000ULL;
	U64 bitboardBlack = 0x810000000ULL;

	bool moveMade;

	

private:

	int boxWidth;
	int boxHeight;

};