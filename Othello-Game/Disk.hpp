#pragma once

#include <SDL.h>
#include <SDL_image.h>



class Disk 
{

	public:

		static void init();
		static SDL_Texture* blackDisk;
		static SDL_Texture* whiteDisk;

		static uint8_t white;
		static uint8_t black;


	private:


};