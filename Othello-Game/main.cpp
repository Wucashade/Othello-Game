// main.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <SDL.h>
#include "Window.hpp"
#include <string>


using namespace std;

int main(int argc, char* argv[])
{

    Window* window = new Window;
    window->init("Othello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);


    while (window->running())
    {
        window->eventManager();

        SDL_Rect rect = {SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};

        SDL_SetRenderDrawColor(window->renderer, 255, 255, 255, 255);

        SDL_RenderFillRect(window->renderer, &rect);

        SDL_RenderPresent(window -> renderer);

    }

    window->clean();
    delete window;

    return 0;



};