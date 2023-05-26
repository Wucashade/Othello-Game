#include "define.hpp"
#include <SDL.h>
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
		void render();
		void update();
		void resize();
		
		void handleMouseButtonDown(SDL_MouseButtonEvent& b);
		void computerMove();
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
			WHITE_MOVE,
			BLACK_MOVE,
			GAME_OVER
		};
		state se = BLACK_MOVE;


	private:

	
		
		Board* board;
		Player* playerOne;
		Player* playerTwo;

};
