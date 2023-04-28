#include "Game.hpp"
#include "Window.hpp"



int Game::boardTopLeftX;
int Game::boardTopLeftY;

enum state
{
    WHITE_MOVE,
    BLACK_MOVE,
    GAME_OVER
};

state se = BLACK_MOVE;

Game::Game()
{
	bool playerCanMove;
	bool moveMade;

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
    playerOne = new Player(board -> bitboardBlack, board->popCount(board->bitboardBlack), 1);
    playerTwo = new Player(board -> bitboardWhite, board->popCount(board->bitboardWhite), 0);
	if (playerOne->isPlayerHuman() == 1)
	{
		playerCanMove = 1;
	}
    
}

void Game::render() 
{
	board->render(playerOne->getBitboard(), playerTwo->getBitboard());
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
	if(playerCanMove == 1 && se == BLACK_MOVE)
	{
    	moveMade = board -> handleMouseButtonDown(b, playerOne, playerTwo); 
		if(moveMade == 1)
		{
			cout << "Player made a move" << endl;
			moveMade = 0;
			nextTurn();
		}
	}
	else if(playerCanMove == 1 && se == WHITE_MOVE)
	{
		moveMade = board -> handleMouseButtonDown(b, playerTwo, playerOne);
		if(moveMade == 1)
		{
			cout << "Player made a move" << endl;
			moveMade = 0;
			nextTurn();
		}
	}
}

void Game::nextTurn()
{
	if(se == BLACK_MOVE)
	{
		se = WHITE_MOVE;
		cout << "WHITE MOVE" << endl;
	}
	else if(se == WHITE_MOVE)
	{
		
		se = BLACK_MOVE;
		cout << "BLACK MOVE" << endl;
	}
    if (board->generateMoves(playerOne->pieceColour, playerTwo->pieceColour) == 0 && 
        board->generateMoves(playerTwo->pieceColour, playerOne->pieceColour) == 0)
    {
        se = GAME_OVER;
        cout << "GAME OVER" << endl;
        if (board->popCount(playerOne->pieceColour) > board->popCount(playerTwo->pieceColour))
        {
            cout << "BLACK WINS!" << endl;
        }
        else if (board->popCount(playerTwo->pieceColour) > board->popCount(playerOne->pieceColour))
        {
            cout << "WHITE WINS!" << endl;
        }
    } 
    else if (se == BLACK_MOVE && board->generateMoves(playerOne->pieceColour, playerTwo->pieceColour) == 0)
    {
        se = WHITE_MOVE;
		cout << "BLACK HAS NO MOVES, WHITE MOVE" << endl;
    }
    else if (se == WHITE_MOVE && board->generateMoves(playerTwo->pieceColour, playerOne->pieceColour) == 0)
    {
        se = BLACK_MOVE;
		cout << "WHITE HAS NO MOVES, BLACK MOVE" << endl;
    }

	

	if(se == BLACK_MOVE && playerOne->isPlayerHuman() == 0)
	{
		playerCanMove = 0;
		computerTurn(playerOne, playerTwo);
	}
	else if(se == WHITE_MOVE && playerTwo->isPlayerHuman() == 0)
	{
		playerCanMove = 0;
		computerTurn(playerTwo, playerOne);
	}
	else
	{
		playerCanMove = 1;
	}
	
}

void Game::computerTurn(Player* currentPlayer, Player* opponentPlayer)
{
	int row = 0, col = 0;
	board->computeMove(currentPlayer->pieceColour, opponentPlayer->pieceColour, &row, &col);
	board->commitMove(&currentPlayer->pieceColour, &opponentPlayer->pieceColour, (row * 8 + col));
	cout << "Computer made move at X: "<< row << " Y: " << col<<endl;
	nextTurn();

}
