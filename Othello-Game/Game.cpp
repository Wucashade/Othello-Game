#include "Game.hpp"
#include "Window.hpp"
#include <thread>
#include <unistd.h>



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
    playerOne = new Player(board -> bitboardBlack, board->popCount(board->bitboardBlack), 0);
    playerTwo = new Player(board -> bitboardWhite, board->popCount(board->bitboardWhite), 1);
	if (playerOne->isPlayerHuman() == 1)
	{
		playerCanMove = 1;
		cout << "BLACK MOVE" << endl;

	}
	else
	{
		playerCanMove = 0;
		cout << "BLACK MOVE" << endl;
		computerTurn(playerOne, playerTwo);
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
	int turn = ((board->popCount(playerOne->pieceColour)+board->popCount(playerTwo->pieceColour))-4);
	
	cout<< "Turn: " << turn << endl;
	



	if(checkGameOver())
	{
		return;
	}
	
	if(se == BLACK_MOVE)
	{
		if (board->generateMoves(playerTwo->pieceColour, playerOne->pieceColour) == 0)
		{
			cout << "WHITE HAS NO MOVES, BLACK MOVE" << endl;
			switchPlayer(playerOne, playerTwo);
		}
		else{
			cout << "WHITE MOVE" << endl;
			se = WHITE_MOVE;
			switchPlayer(playerTwo, playerOne);
		}
	}
	else if(se == WHITE_MOVE)
	{
		if (board->generateMoves(playerOne->pieceColour, playerTwo->pieceColour) == 0)
		{
			cout << "BLACK HAS NO MOVES, WHITE MOVE" << endl;
			switchPlayer(playerTwo, playerOne);
		}
		else
		{
			cout << "BLACK MOVE" << endl;
			se = BLACK_MOVE;
			switchPlayer(playerOne, playerTwo);
		}
		
	}
	int blackMoves = board->generateMoves(playerOne->pieceColour, playerTwo->pieceColour);
	int whiteMoves = board->generateMoves(playerTwo->pieceColour, playerOne->pieceColour);
	int blackScore = board->popCount(playerOne->pieceColour);
	int whiteScore = board->popCount(playerTwo->pieceColour);

	cout<< "Black Score: " << blackScore << " White Score: " << whiteScore << endl;
	cout<< "Black moves: " << board->popCount(blackMoves) << " White moves: " << board->popCount(whiteMoves) << endl;
	
}

void Game::aiThread(Game* game)
{

	
	game->computerTurn(game->playerOne, game->playerTwo);
	return;

}

void Game::computerTurn(Player* currentPlayer, Player* opponentPlayer)
{
	int row = 0, col = 0;
	board->computeMove(currentPlayer->pieceColour, opponentPlayer->pieceColour, &row, &col);
	board->commitMove(&currentPlayer->pieceColour, &opponentPlayer->pieceColour, (row * 8 + col));
	cout << "Computer made move at X: "<< row << " Y: " << col<<endl;
	nextTurn();
	
}

bool Game::checkGameOver()
{
	if (board->generateMoves(playerOne->pieceColour, playerTwo->pieceColour) == 0 && 
        board->generateMoves(playerTwo->pieceColour, playerOne->pieceColour) == 0)
    {
        se = GAME_OVER;
        cout << "GAME OVER" << endl;
        if (board->popCount(playerOne->pieceColour) > board->popCount(playerTwo->pieceColour))
        {
            cout << "BLACK WINS!" << endl;
			return 1;
        }
        else if (board->popCount(playerTwo->pieceColour) > board->popCount(playerOne->pieceColour))
        {
            cout << "WHITE WINS!" << endl;
			return 1;
        }
		else
		{
			cout << "DRAW" << endl;
			return 1;
		}
    }
	return 0;
}

void Game::switchPlayer(Player* currentPlayer, Player* oppPlayer)
{
	if(currentPlayer->isPlayerHuman())
	{
		playerCanMove = 1;
	}
	else
	{	
		playerCanMove = 0;
		thread engineThread(aiThread, this);
		engineThread.join();

		//computerTurn(currentPlayer, oppPlayer);
	}
}
