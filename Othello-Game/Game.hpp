#pragma once

#include "define.hpp"
#include <SDL.h>
#include "Board.hpp"

class Game 
{

	public:

		Game();
		~Game();

		void init();
		void render();
		void update();

		static int boardTopLeftX;
		static int boardTopLeftY;


	private:

	

		Board* board;

};
