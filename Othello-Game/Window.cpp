#include "Window.hpp"

SDL_Renderer* Window::renderer = nullptr;


using namespace std;

int Window::windowHeight = 0;
int Window::windowWidth = 0;

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
        calculateWindowDimensions();

        window = SDL_CreateWindow(title, positionX, positionY, Window::windowHeight, Window::windowWidth, SDL_WINDOW_RESIZABLE);

        if (window) 
        {
            cout << "Window created" << endl;
            SDL_SetWindowMinimumSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {

            SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
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

        case SDL_WINDOWEVENT:
            if (windowEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) 
            {
                resize(windowEvent.window.data1, windowEvent.window.data2);
            }
        case SDL_MOUSEBUTTONDOWN:
            game -> handleMouseButtonDown(windowEvent.button);
		break;

        default:
            break;


        }

    }


};

void Window::render() 
{

    //Renders all the graphics
    SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255);

    SDL_RenderClear(renderer);

    game->render();

    SDL_RenderPresent(renderer);
}

void Window::resize(int newWidth, int newHeight) 
{

    Window::windowHeight = newHeight;
    Window::windowWidth = newWidth;
    game->resize();

}

void Window::calculateWindowDimensions()
{
    SDL_DisplayMode dimensions;
    SDL_GetCurrentDisplayMode(0, &dimensions);
    auto Width = dimensions.w;
    auto Height = dimensions.h;

    double squareWidth;
    if (Width > Height) 
    {
        squareWidth = .8 * Height;
    
    }
    else 
    {

        squareWidth = .8 * Width;

    }
    Window::windowWidth = Window::windowHeight;

}
