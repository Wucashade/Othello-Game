#pragma once

#include <iostream>
#include <SDL.h>
#include <string>
#include "define.hpp"
#include "Game.hpp"


class Window
{
public:
    Window();
    ~Window();

    void init(const char* title, int positionX, int positionY);
    bool running();
    void clean();
    void update();
    void eventManager();
    void render();
    void resize(int newWidth, int newHeight);
    void calculateWindowDimensions();

    static int windowWidth;
    static int windowHeight;

    static SDL_Renderer* renderer;

private:

    bool isRunning;
    SDL_Window* window;
    Game* game;
    

};