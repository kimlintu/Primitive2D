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
	printf("test\n");

	bool running = true;
	while(running) {
		SDL_Event event;

		// SDL_PollEvent pulls an available event from the queue and puts it in the provided 
		// SDL_Event structure
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN: 
					{
						SDL_Keycode key = event.key.keysym.sym;
						switch(key) {
							case SDLK_w: 
								{
									printf("key pressed: ");
									printf("W\n");
								} break;
							case SDLK_a: 
								{
									printf("key pressed: ");
									printf("A\n");
								} break;
						}
						fflush(stdout);
					} break;
				case SDL_KEYUP: 
					{
						SDL_Keycode key = event.key.keysym.sym;
						switch(key) {
							case SDLK_w: 
								{
									printf("key released: ");
									printf("W\n");
								} break;
							case SDLK_a: 
								{
									printf("key released: ");
									printf("A\n");
								} break;
						} 
						fflush(stdout);
					} break;
				case SDL_QUIT: 
					{
						running = false;
					} break;
			}
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	printf("done\n");

	return 0;
}
