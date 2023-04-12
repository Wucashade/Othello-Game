// main.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <SDL.h>
#include "Window.hpp"
#include <string>




int main(int argc, char* argv[])
{

    Window* window = new Window;
    window->init("Othello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);


    while (window->running())
    {
        window->eventManager();
        window->update();
        window->render();
    }

    window->clean();
    delete window;

    return 0;



};