#include "Window.hpp"

SDL_Renderer* Window::renderer = nullptr;


using namespace std;

Window::Window()
{

};

Window::~Window()
{

};

void Window::init(const char* title, int positionX, int positionY)
{

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(title, positionX, positionY, SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer) 
    {
        
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        cout << "Renderer Created";

        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
           
    }

    isRunning = true;


};

bool Window::running()
{
    return isRunning;
};

void Window::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
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

void Window::render() 
{

    SDL_SetRenderDrawColor(renderer, 122, 122, 122, 255);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
