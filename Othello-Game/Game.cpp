#include "Game.hpp"
#include "Window.hpp"
#include <thread>
#include <unistd.h>



int Game::boardTopLeftX;
int Game::boardTopLeftY;

int mainBoxWidth;
int mainBoxHeight;

enum state
{
	MAIN_MENU,
    WHITE_MOVE,
    BLACK_MOVE,
    GAME_OVER
};

state se = MAIN_MENU;

Game::Game()
{
	bool playerCanMove;
	bool moveMade;
	int mainBoxWidth;
	int mainBoxHeight;

}

Game::~Game()
{
	//delete(board);

};




void Game::init()
{
	boardTopLeftX = boardTopLeftY = 0;

	mainBoxWidth =  (Window::windowWidth) / 11;
	mainBoxHeight = (Window::windowHeight) / 11;

	


	
	
	/*
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
	*/
    
}

void Game::renderMainMenu()
{
	SDL_Surface* surface;
	SDL_Rect menuBackground;
	SDL_Rect title;
	SDL_Rect startGame;
	SDL_Rect gameMode;
	SDL_Rect pVP;
	SDL_Rect pVC;
	SDL_Rect playerColourText;
	SDL_Rect playerColour;
	SDL_Rect difficultyText;
	SDL_Rect difficultyOne;
	SDL_Rect difficultyTwo;
	SDL_Rect difficultyThree;
	SDL_Texture* words;
	SDL_Color black = {0,0,0};
	SDL_Color white = {255,255,255};

	TTF_Font* globalFont = TTF_OpenFont("fonts/oswald/Oswald-Bold.ttf", 24);
	
	//DEFINE MAIN MENU BG AREA

	menuBackground.w = mainBoxWidth * 7;
	menuBackground.h = mainBoxHeight * 11;
	menuBackground.x = boardTopLeftX + 2 * mainBoxWidth;
	menuBackground.y = boardTopLeftY;

	//DEFINE TITLE AREA

	title.w = mainBoxWidth * 7;
	title.h = mainBoxHeight;
	title.x = boardTopLeftX + 2 * mainBoxWidth;
	title.y = boardTopLeftY;

	//DEFINE GAME MODE TEXT AREA

	gameMode.w = mainBoxWidth * 5;
	gameMode.h = mainBoxHeight;
	gameMode.x = boardTopLeftX + 3 * mainBoxWidth;
	gameMode.y = boardTopLeftY + 2 *mainBoxHeight;

	//DEFINE PVP BUTTON AREA

	pVP.w = mainBoxWidth * 2;
	pVP.h = mainBoxHeight;
	pVP.x = boardTopLeftX + 3 * mainBoxWidth;
	pVP.y = boardTopLeftY + 3 * mainBoxHeight;
	

	//DEFINE PVC BUTTON AREA

	pVC.w = mainBoxWidth * 2;
	pVC.h = mainBoxHeight;
	pVC.x = boardTopLeftX + 6 * mainBoxWidth;
	pVC.y = boardTopLeftY + 3 * mainBoxHeight;

	//DEFINE PLAYER COLOUR TEXT AREA

	playerColourText.w = mainBoxWidth * 5;
	playerColourText.h = mainBoxHeight;
	playerColourText.x = boardTopLeftX + 3 * mainBoxWidth;
	playerColourText.y = boardTopLeftY + 4 *mainBoxHeight;

	//DEFINE PLAYER COLOUR BUTTON AREA

	playerColour.w = mainBoxWidth * 5;
	playerColour.h = mainBoxHeight;
	playerColour.x = boardTopLeftX + 3 * mainBoxWidth;
	playerColour.y = boardTopLeftY + 5 * mainBoxHeight;

	//DEFINE DIFFICULTY TEXT AREA

	difficultyText.w = mainBoxWidth * 5;
	difficultyText.h = mainBoxHeight;
	difficultyText.x = boardTopLeftX + 3 * mainBoxWidth;
	difficultyText.y = boardTopLeftY + 7 *mainBoxHeight;


	//DEFINE DIFFICULTY ONE BUTTON AREA

	difficultyOne.w = mainBoxWidth;
	difficultyOne.h = mainBoxHeight;
	difficultyOne.x = boardTopLeftX + 3 * mainBoxWidth;
	difficultyOne.y = boardTopLeftY + 8 * mainBoxHeight;

	//DEFINE DIFFICULTY TWO BUTTON AREA

	difficultyTwo.w = mainBoxWidth;
	difficultyTwo.h = mainBoxHeight;
	difficultyTwo.x = boardTopLeftX + 5 * mainBoxWidth;
	difficultyTwo.y = boardTopLeftY + 8 * mainBoxHeight;

	//DEFINE DIFFICULTY THREE BUTTON AREA

	difficultyThree.w = mainBoxWidth;
	difficultyThree.h = mainBoxHeight;
	difficultyThree.x = boardTopLeftX + 7 * mainBoxWidth;
	difficultyThree.y = boardTopLeftY + 8 * mainBoxHeight;


	//DEFINE START BUTTON AREA

	startGame.w = mainBoxWidth * 5;
	startGame.h = mainBoxHeight;
	startGame.x = boardTopLeftX + 3 * mainBoxWidth;
	startGame.y = boardTopLeftY + 10 * mainBoxHeight;

	//DISPLAY BG

	SDL_SetRenderDrawColor(Window::renderer, 169, 169, 169, 0);
	SDL_RenderFillRect(Window::renderer, &menuBackground);

	//DISPLAY TITLE

	surface = TTF_RenderText_Solid(globalFont, "OTHELLO", black);
    words = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_RenderCopy(Window::renderer, words, NULL, &title);
    SDL_FreeSurface(surface);

	//DISPLAY START BUTTON

	surface = TTF_RenderText_Solid(globalFont, "START GAME", black);
    words = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 0);
	SDL_RenderDrawRect(Window::renderer, &startGame);
	SDL_RenderCopy(Window::renderer, words, NULL, &startGame);
    SDL_FreeSurface(surface);

	//DISPLAY GAMEMODE TEXT

	surface = TTF_RenderText_Solid(globalFont, "GAMEMODE:", black);
    words = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_RenderCopy(Window::renderer, words, NULL, &gameMode);
    SDL_FreeSurface(surface);

	//DISPLAY PVP BUTTON

	if(pVPTrue == 1)
	{
		surface = TTF_RenderText_Solid(globalFont, "PVP", white);
	}
	else
	{
		surface = TTF_RenderText_Solid(globalFont, "PVP", black);
	}
	
    words = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 0);
	SDL_RenderDrawRect(Window::renderer, &pVP);
	SDL_RenderCopy(Window::renderer, words, NULL, &pVP);
    SDL_FreeSurface(surface);

	//DISPLAY PVC BUTTON

	if(pVCTrue == 1)
	{
		surface = TTF_RenderText_Solid(globalFont, "PVC", white);
	}
	else
	{
		surface = TTF_RenderText_Solid(globalFont, "PVC", black);
	}
    words = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 0);
	SDL_RenderDrawRect(Window::renderer, &pVC);
	SDL_RenderCopy(Window::renderer, words, NULL, &pVC);
    SDL_FreeSurface(surface);

	if(pVCTrue == 1)
	{

		//DISPLAY PLAYER COLOUR TEXT

		surface = TTF_RenderText_Solid(globalFont, "CHOOSE YOUR COLOUR:", black);
		words = SDL_CreateTextureFromSurface(Window::renderer, surface);
		SDL_RenderCopy(Window::renderer, words, NULL, &playerColourText);
		SDL_FreeSurface(surface);

		//DISPLAY PLAYER COLOUR BUTTON

		if(playerColourBlack == 1)
		{
			surface = TTF_RenderText_Solid(globalFont, "BLACK", black);
		}
		else
		{
			surface = TTF_RenderText_Solid(globalFont, "WHITE", white);
		}
		
		words = SDL_CreateTextureFromSurface(Window::renderer, surface);
		SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 0);
		SDL_RenderDrawRect(Window::renderer, &playerColour);
		SDL_RenderCopy(Window::renderer, words, NULL, &playerColour);
		SDL_FreeSurface(surface);

		//DISPLAY DIFFICULTY TEXT

		surface = TTF_RenderText_Solid(globalFont, "DIFFICULTY:", black);
		words = SDL_CreateTextureFromSurface(Window::renderer, surface);
		SDL_RenderCopy(Window::renderer, words, NULL, &difficultyText);
		SDL_FreeSurface(surface);

		//DISPLAY DIFFICULTY ONE BUTTON

		if(difficultyOneTrue == 1)
		{
			surface = TTF_RenderText_Solid(globalFont, "1", white);
		}
		else
		{
			surface = TTF_RenderText_Solid(globalFont, "1", black);
		}
		
		words = SDL_CreateTextureFromSurface(Window::renderer, surface);
		SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 0);
		SDL_RenderDrawRect(Window::renderer, &difficultyOne);
		SDL_RenderCopy(Window::renderer, words, NULL, &difficultyOne);
		SDL_FreeSurface(surface);

		//DISPLAY DIFFICULTY TWO BUTTON

		if(difficultyTwoTrue == 1)
		{
			surface = TTF_RenderText_Solid(globalFont, "2", white);
		}
		else
		{
			surface = TTF_RenderText_Solid(globalFont, "2", black);
		}
		
		words = SDL_CreateTextureFromSurface(Window::renderer, surface);
		SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 0);
		SDL_RenderDrawRect(Window::renderer, &difficultyTwo);
		SDL_RenderCopy(Window::renderer, words, NULL, &difficultyTwo);
		SDL_FreeSurface(surface);

		//DISPLAY DIFFICULTY THREE BUTTON

		if(difficultyThreeTrue == 1)
		{
			surface = TTF_RenderText_Solid(globalFont, "3", white);
		}
		else
		{
			surface = TTF_RenderText_Solid(globalFont, "3", black);
		}
		
		words = SDL_CreateTextureFromSurface(Window::renderer, surface);
		SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 0);
		SDL_RenderDrawRect(Window::renderer, &difficultyThree);
		SDL_RenderCopy(Window::renderer, words, NULL, &difficultyThree);
		SDL_FreeSurface(surface);
	}



}

void Game::render() 
{
	if(se != MAIN_MENU)
	{
		board->render(playerOne->getBitboard(), playerTwo->getBitboard());
	}
	else
	{
		renderMainMenu();
	}
}

void Game::update()
{

}

void Game::resize() 
{
	
	if(se != MAIN_MENU)
	{
		board->resize();

		boardTopLeftX = (Window::windowWidth - board->getWidth()) / 2;
		boardTopLeftY = (Window::windowHeight - board->getHeight()) / 2;
	}
	else
	{
		mainBoxWidth = Window::windowWidth / 11;
		mainBoxHeight = Window::windowHeight / 11;

		if (mainBoxWidth > mainBoxHeight) {

			mainBoxWidth = mainBoxHeight;
		}
		else 
		{
			mainBoxHeight = mainBoxWidth;
		}
		boardTopLeftX = (Window::windowWidth - (11 * mainBoxWidth)) /2;
		boardTopLeftY = (Window::windowHeight - (11 * mainBoxHeight)) /2;
	}
	
}

void Game::handleMouseButtonDown(SDL_MouseButtonEvent& b)
{
	if(se == MAIN_MENU)
	{
		if(b.button == SDL_BUTTON_LEFT )
		{
			int x, y, boardX, boardY;
			SDL_GetMouseState(&x, &y);
			boardX = (x - boardTopLeftX) / mainBoxWidth;
        	boardY = (y - boardTopLeftY) / mainBoxHeight;

			cout << "board clicked at X: " << boardX << " Y: " << boardY<<endl;

			if((boardX == 3 || boardX == 4) && boardY == 3)
			{
				if(pVPTrue == 0)
				{
					pVPTrue = 1;
					if(pVCTrue == 1)
					{
						pVCTrue = 0;
					}
				}
			}
			else if((boardX == 6 || boardX == 7) && boardY == 3)
			{
				if(pVCTrue == 0)
				{
					pVCTrue = 1;
					if(pVPTrue == 1)
					{
						pVPTrue = 0;
					}
				}
			}
			else if((boardX > 2 && boardX < 8) && boardY == 10)
			{
				cout<<"hi";
			}
			else if(pVCTrue == 1)
			{
				if((boardX > 2 && boardX < 8) && boardY == 5)
				{
					if(playerColourBlack == 0)
					{
						playerColourBlack = 1;
					}
					else
					{
						playerColourBlack = 0;
					}
				}
				else if(boardX == 3 && boardY == 8)
				{
					if(difficultyOneTrue == 0)
					{
						difficultyOneTrue = 1;
					}
					if(difficultyTwoTrue == 1 || difficultyThreeTrue == 1)
					{
						difficultyTwoTrue = 0;
						difficultyThreeTrue = 0;
					}
				}
				else if(boardX == 5 && boardY == 8)
				{
					if(difficultyTwoTrue == 0)
					{
						difficultyTwoTrue = 1;
					}
					if(difficultyOneTrue == 1 || difficultyThreeTrue == 1)
					{
						difficultyOneTrue = 0;
						difficultyThreeTrue = 0;
					}
				}
				else if(boardX == 7 && boardY == 8)
				{
					if(difficultyThreeTrue == 0)
					{
						difficultyThreeTrue = 1;
					}
					if(difficultyTwoTrue == 1 || difficultyOneTrue == 1)
					{
						difficultyTwoTrue = 0;
						difficultyOneTrue = 0;
					}
				}

			}
			

		}

	}

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
