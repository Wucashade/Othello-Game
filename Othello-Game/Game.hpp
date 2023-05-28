#include "define.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Board.hpp"
#include "Player.hpp"
#include <thread>
#pragma once


typedef uint64_t U64;

class Game 
{

	public:

		Game();
		~Game();

		void init();
		void initGame();
		void render();
		void renderGameOver();
		void renderMainMenu();
		void update();
		void resize();
		
		void handleMouseButtonDown(SDL_MouseButtonEvent& b);
		void computerMove();
		void stats();
		void nextTurn();
		static void aiThread(Game* game);
		void computerTurn(Player* currentPlayer, Player* opponentPlayer);
		bool checkGameOver();
		void switchPlayer(Player* thisPlayer, Player* thatPlayer);

		static int boardTopLeftX;
		static int boardTopLeftY;

		bool playerCanMove;
		bool moveMade;

		enum state
		{
			MAIN_MENU,
			WHITE_MOVE,
			BLACK_MOVE,
			GAME_OVER
		};
		state se = MAIN_MENU;

		bool pVPTrue = 0;
		bool pVCTrue = 0;
		bool playerColourBlack = 1;
		bool difficultyOneTrue = 0;
		bool difficultyTwoTrue = 0;
		bool difficultyThreeTrue = 0;

		


	private:


		int mainBoxWidth;
		int mainBoxHeight;

		Board* board;
		Player* playerOne;
		Player* playerTwo;

};
