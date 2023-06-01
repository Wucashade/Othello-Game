#include "Game.hpp"
#include "Window.hpp"
#include <thread>
#include <unistd.h>
#include <fstream>
#include <time.h>



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
	
	*/
    
}

void Game::initGame()
{
	se = BLACK_MOVE;	// Set move to black move
	board = new Board();
	board->init();	//Load board function
	resize();
	if(pVCTrue)
	{
		if(playerColourBlack)
		{	//Player black, ai white
			playerOne = new Player(board -> bitboardBlack, board->popCount(board->bitboardBlack), 1);
    		playerTwo = new Player(board -> bitboardWhite, board->popCount(board->bitboardWhite), 0);
		}
		else
		{	//Ai black, player white
			playerOne = new Player(board -> bitboardBlack, board->popCount(board->bitboardBlack), 0);
    		playerTwo = new Player(board -> bitboardWhite, board->popCount(board->bitboardWhite), 1);
		}
	}
	else
	{	//PVP
		playerOne = new Player(board -> bitboardBlack, board->popCount(board->bitboardBlack), 1);
    	playerTwo = new Player(board -> bitboardWhite, board->popCount(board->bitboardWhite), 1);
	}
    
	if (playerOne->isPlayerHuman() == 1)
	{
		stats();
		playerCanMove = 1;
		cout << "BLACK MOVE" << endl;

	}
	else
	{
		stats();
		playerCanMove = 0;
		cout << "BLACK MOVE" << endl;
		thread engineThread(aiThread, this);
		engineThread.detach();
	}
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

		TTF_CloseFont( globalFont );
    	globalFont = NULL;
		SDL_DestroyTexture(words);
	}



}

void Game::renderGameOver()
{
	SDL_Rect gameOverText;
	SDL_Rect gameOverText2;
	SDL_Rect gameOverBackground;
	SDL_Rect victor;
	SDL_Rect score;
	SDL_Surface* surface;
	SDL_Texture* words;
    string scoreText =  to_string(board->popCount(playerOne->pieceColour)) + " - " + to_string(board->popCount(playerTwo->pieceColour));
	SDL_Color white = {255,255,255};

	TTF_Font* globalFont = TTF_OpenFont("fonts/oswald/Oswald-Bold.ttf", 24);

	//DEFINE END GAME BOX AREA

	gameOverBackground.w = mainBoxWidth * 11;
	gameOverBackground.h = mainBoxHeight * 6;
	gameOverBackground.x = boardTopLeftX + 2 * mainBoxWidth;
	gameOverBackground.y = boardTopLeftY + 4 * mainBoxHeight;

	//DEFINE END GAME TEXT AREA

	gameOverText.w = mainBoxWidth * 7;
	gameOverText.h = mainBoxHeight;
	gameOverText.x = boardTopLeftX + 4 * mainBoxWidth;
	gameOverText.y = boardTopLeftY + 4 * mainBoxHeight;

	//DEFINE VICTOR TEXT AREA

	victor.w = mainBoxWidth * 3;
	victor.h = mainBoxHeight;
	victor.x = boardTopLeftX + 6 * mainBoxWidth;
	victor.y = boardTopLeftY + 5 * mainBoxHeight;

	//DEFINE SCORE TEXT AREA

	score.w = mainBoxWidth * 5;
	score.h = mainBoxHeight;
	score.x = boardTopLeftX + 5 * mainBoxWidth;
	score.y = boardTopLeftY + 6 * mainBoxHeight;

	//DEFINE SECOND GAME OVER TEXT AREA

	gameOverText2.w = mainBoxWidth * 9;
	gameOverText2.h = mainBoxHeight;
	gameOverText2.x = boardTopLeftX + 3 * mainBoxWidth;
	gameOverText2.y = boardTopLeftY + 8 * mainBoxHeight;

	//DRAW END GAME BOX

	SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(Window::renderer, &gameOverBackground);

	//DRAW END GAME TEXT
	surface = TTF_RenderText_Solid(globalFont, "GAME OVER", white);
	words = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_RenderCopy(Window::renderer, words, NULL, &gameOverText);
	SDL_FreeSurface(surface);

	if(board->popCount(playerOne->pieceColour) > board->popCount(playerTwo->pieceColour))
	{
		//DRAW "BLACK WINS"
		surface = TTF_RenderText_Solid(globalFont, "BLACK WINS", white);	
	}
	else if(board->popCount(playerTwo->pieceColour) > board->popCount(playerOne->pieceColour))
	{
		//DRAW "WHITE WINS"
		surface = TTF_RenderText_Solid(globalFont, "WHITE WINS", white);
	}
	else
	{
		//DRAW "DRAW"
		surface = TTF_RenderText_Solid(globalFont, "DRAW", white);
	}
	words = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_RenderCopy(Window::renderer, words, NULL, &victor);
	SDL_FreeSurface(surface);

	//DRAW SCORE TEXT

	surface = TTF_RenderText_Solid(globalFont, scoreText.c_str(), white);
	words = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_RenderCopy(Window::renderer, words, NULL, &score);
	SDL_FreeSurface(surface);

	//DRAW SECOND GAME TEXT
	
	surface = TTF_RenderText_Solid(globalFont, "PRESS ANYWHERE TO CONTINUE", white);
	words = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_RenderCopy(Window::renderer, words, NULL, &gameOverText2);
	SDL_FreeSurface(surface);

	// FREE SPACE

	TTF_CloseFont( globalFont );
	SDL_DestroyTexture(words);
    globalFont = NULL;
}

void Game::render() 
{
	string currentTurn;
    
	// render depending on state
	if(se != MAIN_MENU)
	{
		if(se == BLACK_MOVE)
		{
			currentTurn = "BLACK";
		}
		else
		{
			currentTurn = "WHITE";
		}
		board->render(playerOne, playerTwo, currentTurn);
		if(se == GAME_OVER)
		{
			renderGameOver();
		}
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
	mainBoxWidth = Window::windowWidth / 11;
	mainBoxHeight = Window::windowHeight / 11;
	if (mainBoxWidth > mainBoxHeight) 
	{
		mainBoxWidth = mainBoxHeight;
	}
	else 
	{
		mainBoxHeight = mainBoxWidth;
	}
	
	
	if(se != MAIN_MENU)
	{
		board->resize();

		boardTopLeftX = (Window::windowWidth - board->getWidth()) / 2;
		boardTopLeftY = (Window::windowHeight - board->getHeight()) / 2;
	}
	else
	{
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
			else if((boardX > 2 && boardX < 8) && (boardY == 10) && (pVPTrue == 1 || pVCTrue == 1))
			{
				initGame();
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

	else if(playerCanMove == 1 && se == BLACK_MOVE)
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
	else if(se == GAME_OVER)
	{
		if(b.button == SDL_BUTTON_LEFT)
		{
			se = MAIN_MENU;
			resize();
			delete(board);
			delete(playerOne);
			delete(playerTwo);
		}
	}
	if(se != MAIN_MENU)
	{
		if(b.button == SDL_BUTTON_LEFT ){
			int x, y, boardX, boardY;
			SDL_GetMouseState(&x, &y);
			boardX = (x - boardTopLeftX) / mainBoxWidth;
			boardY = (y - boardTopLeftY) / mainBoxHeight;

			if(boardX < 3 && boardY == 0 && (!(playerOne->isPlayerHuman() == 0 && se == BLACK_MOVE) || !(playerTwo->isPlayerHuman() == 0 && se == WHITE_MOVE)))
			{
				se = MAIN_MENU;
				resize();
				delete(board);
				delete(playerOne);
				delete(playerTwo);
			}
		}
	}
	//else if(GAME_OVER || )
}

void Game::stats()
{
	int turn = ((board->popCount(playerOne->pieceColour)+board->popCount(playerTwo->pieceColour))-3);
	
	cout<< "Turn: " << turn << endl;


	U64 blackMoves = board->generateMoves(playerOne->pieceColour, playerTwo->pieceColour);
	U64 whiteMoves = board->generateMoves(playerTwo->pieceColour, playerOne->pieceColour);
	int blackScore = board->popCount(playerOne->pieceColour);
	int whiteScore = board->popCount(playerTwo->pieceColour);

	cout<< "Black Score: " << blackScore << " White Score: " << whiteScore << endl;
	cout<< "Black moves: " << board->popCount(blackMoves) << " White moves: " << board->popCount(whiteMoves) << endl;

	
	ofstream resultsFile("results.txt", std::ios::app);

  	resultsFile << turn << " " << blackScore << " " << whiteScore << " " << board->popCount(blackMoves) << " " << board->popCount(whiteMoves) << "\n" ;
  	resultsFile.close();
}

void Game::nextTurn()
{
	stats();
	
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
	
}

void Game::aiThread(Game* game)
{

	if(game->playerOne->isPlayerHuman() == 0)
	{
		game->computerTurn(game->playerOne, game->playerTwo);
	}
	else
	{
		game->computerTurn(game->playerTwo, game->playerOne);
	}
	
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
		clock_t tStart = clock();
		playerCanMove = 0;
		thread engineThread(aiThread, this);
		engineThread.detach();
		cout << "Time: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
		
		//computerTurn(currentPlayer, oppPlayer);
	}
}
