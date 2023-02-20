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


	outlineColour = BLACK;
	boxColour = BOARD_COLOUR;

	boxWidth = Window::windowWidth / BOARD_BOXES_X;
	boxHeight = Window::windowHeight / BOARD_BOXES_Y;

}

void Board::renderBoard() 
{


	

	SDL_Rect boardBuild;
	boardBuild.w = boxWidth;
	boardBuild.h = boxHeight;

	for (int i = 0; i < BOARD_BOXES_X; i++) 
	{
		for (int j = 0; j < BOARD_BOXES_Y; j++) 
		{
			boardBuild.x = Game::boardTopLeftX + i * boxWidth;
			boardBuild.y = Game::boardTopLeftY + j * boxHeight;
			SDL_SetRenderDrawColor(Window::renderer, boxColour.r, boxColour.g, boxColour.b, boxColour.a);
			SDL_RenderFillRect(Window::renderer, &boardBuild);

			
			SDL_SetRenderDrawColor(Window::renderer, outlineColour.r, outlineColour.g, outlineColour.b, outlineColour.a);
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

void Board::resize() 
{
	boxWidth = Window::windowWidth / BOARD_BOXES_X;
	boxHeight = Window::windowHeight / BOARD_BOXES_Y;

	if (boxWidth > boxHeight) {

		boxWidth = boxHeight;
	}
	else 
	{
		boxHeight = boxWidth;
	}

}

int Board::getWidth() 
{
	return BOARD_BOXES_X * boxWidth;
}

int Board::getHeight() 
{
	return BOARD_BOXES_Y * boxHeight;
}

