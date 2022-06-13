#include <SDL.h>
#include <SDL_opengl.h>

#include <stdio.h>

int main(int argc, char *argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        // TODO: Error logging

        return -1;
    } 
    printf("test\n");

    // We need to initialize OpenGL before creating our window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);                                    // Use double buffering instead of single buffering 
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);                              // Enable hardware acceleration (don't use software rendering)
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);                                        // TODO: See what happens for different sizes than 8    
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);                                      // TODO: See what happens for different sizes than 8    
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);                                       // TODO: See what happens for different sizes than 8    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);                           // TODO: Should a more recent version be used instead of 3.3?
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    int DISPLAY_WIDTH = 800; 
    int DISPLAY_HEIGHT = 600; 
    SDL_Window *window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(!window) {
        // TODO: Error logging

        return -1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if(!gl_context) {
        // TODO: Error logging

        printf("ERROR: [could not create OpenGL context]: %s\n", SDL_GetError());
        return -1;
    }

    glViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    glClearColor(0.3, 0.5, 0, 1);

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
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("done\n");

    return 0;
}
