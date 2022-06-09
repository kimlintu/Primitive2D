#include <SDL.h>

#include <stdio.h>

int main(int argc, char *argv[]) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		// TODO: Error logging

		return -1;
	} 

	int DISPLAY_WIDTH = 800; 
	int DISPLAY_HEIGHT = 600; 
	SDL_Window *window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window) {
		// TODO: Error logging

		return -1;
	}

	SDL_Delay(5000);

	SDL_DestroyWindow(window);
	SDL_Quit();

	printf("done\n");

	return 0;
}
