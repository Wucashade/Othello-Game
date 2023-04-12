#pragma once

#include "define.hpp"
#include <SDL.h>
#include "Board.hpp"
#include "Player.hpp"


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
		void play();

		static int boardTopLeftX;
		static int boardTopLeftY;


	private:

	

		Board* board;
		Player* playerOne;
		Player* playerTwo;

};
