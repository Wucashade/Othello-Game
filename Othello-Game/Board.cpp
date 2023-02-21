#include "Board.hpp"
#include "Game.hpp"
#include "Window.hpp"
#include "Disk.hpp"








Board::Board()
{

};

Board::~Board()
{

};



void Board::init()
{

	Disk::init(); 


	outlineColour = BLACK;
	boxColour = BOARD_COLOUR;

	boxWidth = Window::windowWidth / BOARD_BOXES_X;
	boxHeight = Window::windowHeight / BOARD_BOXES_Y;
	
	uint8_t** board = new uint8_t * [BOARD_BOXES_X];
	for (int i = 0; i < BOARD_BOXES_X; i++) 
	{
		board[i] = new uint8_t[BOARD_BOXES_Y];
	}
	for (int i = 0; i < BOARD_BOXES_X; i++)
	{
		for (int j = 0; j < BOARD_BOXES_Y; j++)
		{
			
			board[i][j] = 0;

		}

	}
	



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
	renderDisks();

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


void Board::renderDisks() 
{
	
	for (int i = 0; i < BOARD_BOXES_X; i++) 
	{
		for (int j = 0; j < BOARD_BOXES_Y; j++) 
		{
			if(board[i][j] != 0)
			{
				renderDisk(i, j);
			}
		
		}


	}

	
}

void Board::renderDisk(int i, int j) 
{
	uint8_t currentDisk = board[i][j];


}
