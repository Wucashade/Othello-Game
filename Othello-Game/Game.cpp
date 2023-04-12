#include "Game.hpp"
#include "Window.hpp"

int Game::boardTopLeftX;
int Game::boardTopLeftY;

U64 playerOne = 0;
U64 playerTwo = 0;

enum state
{
    WHITE_MOVE,
    BLACK_MOVE,
    GAME_OVER
};

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
    playerOne = new Player(board -> bitboardBlack, board->popCount(board->bitboardBlack), 1);
    playerTwo = new Player(board -> bitboardWhite, board->popCount(board->bitboardWhite), 1);
    cout << playerTwo->score << endl;
	board->init();
    play();

}

void Game::play()
{

    int row = 0, col = 0;
    int cnt = 0;


    state s = BLACK_MOVE;

    while (!(s == GAME_OVER))
    {
        if(s == BLACK_MOVE)
        {

        }
        if (s == WHITE_MOVE)
        {

        }
        if(board -> generateMoves(playerOne, playerTwo))

    }

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

void Game::handleMouseButtonDown(SDL_MouseButtonEvent& b)
{
    board -> handleMouseButtonDown(b, playerOne, playerTwo);
}

