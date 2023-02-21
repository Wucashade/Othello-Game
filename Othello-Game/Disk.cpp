#include "Disk.hpp"
#include "Window.hpp"

SDL_Texture* Disk::blackDisk;
SDL_Texture* Disk::whiteDisk;

uint8_t Disk::black;
uint8_t Disk::white;

void Disk::init() 
{
	SDL_Surface* surface;
	surface = IMG_Load("disctextures/blackdisc.png");
	blackDisk = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("disctextures/whitedisc.png");
	whiteDisk = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	black = 1;
	white = 2;

}

