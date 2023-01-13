#include <iostream>
#include <SDL.h>
#include <string>


class Window
{
public:
    Window();
    ~Window();

    void init(const char* title, int xsize, int ysize);
    bool running();
    void clean();
    void eventManager();
private:

    bool isRunning;
    SDL_Window* window;

};