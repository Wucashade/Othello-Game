#include "Game.hpp"
#include "Window.hpp"


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

void Game::resize() 
{

	board->resize();

	boardTopLeftX = (Window::windowWidth - board->getWidth()) / 2;
	boardTopLeftY = (Window::windowHeight - board->getHeight()) / 2;
}