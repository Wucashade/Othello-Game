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

    // Initialises the window and renderer

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

        cout << "Subsystems Initialised" << endl;

        window = SDL_CreateWindow(title, positionX, positionY, SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_RESIZABLE);

        if (window) 
        {
            cout << "Window created" << endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {

            SDL_SetRenderDrawColor(renderer, 0, 250, 154, 0);
            cout << "Renderer Created" << endl;

        }

        isRunning = true;

        game = new Game();
        game->init();
    }
    else 
    {
        isRunning = false;

    }

};

bool Window::running()
{
    return isRunning;
};

void Window::clean()
{

    // Cleans the window after it is closed
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
};

void Window::update() 
{
    
}

void Window::eventManager()
{

    // Manages events that relate to SDL 
    SDL_Event windowEvent;
    if (SDL_PollEvent(&windowEvent))
    {
        switch (windowEvent.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            break;


        }

    }


};

void Window::render() 
{

    //Renders all the graphics

    boardColour = BOARD_COLOUR;

    SDL_SetRenderDrawColor(renderer, boardColour.r, boardColour.g, boardColour.b, boardColour.a);

    SDL_RenderClear(renderer);

    game->render();

    SDL_RenderPresent(renderer);
}
