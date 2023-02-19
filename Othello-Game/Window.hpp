#include <iostream>
#include <SDL.h>
#include <string>
#include "define.hpp"


class Window
{
public:
    Window();
    ~Window();

    void init(const char* title, int positionX, int positionY);
    bool running();
    void clean();
    void eventManager();
    void render();

    static int windowWidth;
    static int windowHeight;

    static SDL_Renderer* renderer;

private:

    bool isRunning;
    SDL_Window* window;
    

};