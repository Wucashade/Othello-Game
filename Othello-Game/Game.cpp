#include "Game.hpp"


void Game::init()
{

	boardWidth = SCREEN_WIDTH / BOARD_BOXES_X;
	boardHeight = SCREEN_HEIGHT / BOARD_BOXES_Y;

	for (int i = 0; i < 8; i++) 
	{
		for (int j = 0; j < 8; j++) 
		{
			SDL_Rect rect;
			rect.x = 250;
			rect.y = 150;
			rect.w = 200;
			rect.h = 200;

			SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(Window::renderer, &rect);

			SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 255);

			SDL_RenderPresent(Window::renderer);
		
		}
	}

	void render();

	void update();


}