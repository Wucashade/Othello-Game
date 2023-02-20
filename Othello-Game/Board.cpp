#include "Board.hpp"
#include "Game.hpp"
#include "Window.hpp"






Board::Board()
{

};

Board::~Board()
{

};



void Board::init()
{


	boardColour1 = BLACK;
	boardColour2 = BOARD_COLOUR;
	

}

void Board::renderBoard() 
{

	boxWidth = SCREEN_WIDTH / BOARD_BOXES_X;
	boxHeight = SCREEN_HEIGHT / BOARD_BOXES_Y;

	SDL_Rect boardBuild;
	boardBuild.w = boxWidth;
	boardBuild.h = boxHeight;

	for (int i = 0; i < BOARD_BOXES_X; i++) 
	{
		for (int j = 0; j < BOARD_BOXES_Y; j++) 
		{
			SDL_Color currentColor = (i + j) % 2 == 0 ? boardColour1 : boardColour1;
			boardBuild.x = Game::boardTopLeftX + i * boxWidth;
			boardBuild.y = Game::boardTopLeftY + j * boxHeight;
			SDL_SetRenderDrawColor(Window::renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
			SDL_RenderDrawRect(Window::renderer, &boardBuild);
		
		}

		
	}

}

void Board::render() 
{
	renderBoard();

}

void Board::update() 
{

}
