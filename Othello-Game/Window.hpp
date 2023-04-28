#include <iostream>
#include <SDL.h>
#include <string>
#include "define.hpp"
#include "Game.hpp"
#pragma once

class Window
{
public:
    Window();
    ~Window();

    void init(const char* title, int positionX, int positionY);
    bool running();
    void clean();
    void update(float deltaTime);
    void eventManager();
    void render();
    void resize(int newWidth, int newHeight);
    void calculateWindowDimensions();

    static int windowWidth;
    static int windowHeight;

    static SDL_Renderer* renderer;

private:

    bool isRunning;
    bool frozen;
    SDL_Window* window;
    Game* game;
    

};