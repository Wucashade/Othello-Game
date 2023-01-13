#include "Window.hpp"


using namespace std;
const int WIDTH = 800, HEIGHT = 600;

Window::Window()
{

};

Window::~Window()
{

};

void Window::init(const char* title, int xsize, int ysize)
{

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(title, xsize, ysize, 400, 400, SDL_WINDOW_RESIZABLE);

    isRunning = true;


};

bool Window::running()
{
    return isRunning;
};

void Window::clean()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
};

void Window::eventManager()
{
    SDL_Event windowEvent;
    if (SDL_PollEvent(&windowEvent))
    {
        switch (windowEvent.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        }
    }


};