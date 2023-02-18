#include <SDL.h>
#include <iostream>
#include "Window.hpp"
#include "define.hpp"

class Game
{

public:
	Game();
	~Game();

	void init();
	void render();
	void update();




private:

	
	int boardWidth;
	int boardHeight;


};