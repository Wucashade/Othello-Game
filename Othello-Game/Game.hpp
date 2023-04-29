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
		void computerTurn(Player* currentPlayer, Player* opponentPlayer);

		static int boardTopLeftX;
		static int boardTopLeftY;

		bool playerCanMove;
		bool moveMade;


	private:

	

		Board* board;
		Player* playerOne;
		Player* playerTwo;

};
