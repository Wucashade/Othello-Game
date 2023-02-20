#include "Game.hpp"


int Game::boardTopLeftX;
int Game::boardTopLeftY;



Game::Game()
{

}

Game::~Game()
{
	delete(board);

};


void Game::init()
{

	boardTopLeftX = boardTopLeftY = 0;
	board = new Board();
	board->init();

}

void Game::render() 
{
	board->render();

}

void Game::update()
{

}