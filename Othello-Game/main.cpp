// main.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <SDL.h>
#include "Window.hpp"
#include <string>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <ctime>
#if defined(_WIN32) and defined(_MSC_VER)
	#include <crtdbg.h>
#endif



int main(int argc, char* argv[])
{
    
    srand(time(NULL));
	const int frameDelay = 1000 / 60;

    Uint32 frameStart = SDL_GetTicks();
	int frameTime = 0;
    Window* window = new Window;
    float deltaTime;
    window->init("Othello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);


    while (window->running())
    {
        //Optimising to make sure that app doesn't render too often
        deltaTime = SDL_GetTicks() - frameStart;    
		frameStart = SDL_GetTicks();
        window->eventManager();
        window->update(float(deltaTime) / 1000.0);
        window->render();
        frameTime = SDL_GetTicks() - frameStart;

        //Adjust to delay
        if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
    }

    window->clean();
    delete window;
    


    return 0;



};