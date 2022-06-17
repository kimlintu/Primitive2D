#include "includes/glad_i.h"
#include "includes/sdl_gl.h"

#include "../examples/breakout.cpp"

#include "engine.h"
#include "includes/shader_program.h"

#include <stdio.h>

float tri_vertices[] = {
    0.0f, 1.0f, 
    1.0f, 0.0f, 
    0.0f, 0.0f,  

    0.0f, 1.0f, 
    1.0f, 1.0f, 
    1.0f, 0.0f,  
};

const float MS_PER_TICK = 1000.0f / 30.0f; 

int main(int argc, char *argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR [Could not initialize SDL]: %s", SDL_GetError());

        return -1;
    } 

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
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR [Could not create a window]: %s", SDL_GetError());

        return -1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if(!gl_context) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR [Could not create an OpenGL context]: %s", SDL_GetError());

        return -1;
    }

    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR [Could not initialize GLAD with SDL function loader]");

        return -1;
    }

    glViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    glClearColor(0.3f, 0.5f, 0.0f, 1.0f);

    game_init();

    KeyboardState keyboard_state = {};

    uint64_t prev_tick = SDL_GetTicks64();
    uint64_t lag = 0;

    bool running = true;
    while(running) {
        SDL_Event event;

        // SDL_PollEvent pulls an available event from the queue and puts it in the provided 
        // SDL_Event structure
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_KEYDOWN: 
                case SDL_KEYUP: 
                    {
                        bool key_is_down = (event.type == SDL_KEYDOWN);

                        KeyboardKeyID key_id = MAX_KEY_AMT; // Invalid key id so we can check later if no keys were pressed

                        SDL_Keycode key = event.key.keysym.sym;
                        switch(key) {
                            case SDLK_w: 
                                {
                                    key_id = KEY_W;
                                } break;
                            case SDLK_a: 
                                {
                                    key_id = KEY_A;
                                } break;
                            case SDLK_s: 
                                {
                                    key_id = KEY_S;
                                } break;
                            case SDLK_d: 
                                {
                                    key_id = KEY_D;
                                } break;
                        } 

                        // Only modify state if a valid key has been pressed
                        if(key_id != MAX_KEY_AMT) {
                            if(keyboard_state.keys[key_id].is_down != key_is_down) {
                                keyboard_state.keys[key_id].is_down = key_is_down;
                                keyboard_state.keys[key_id].half_transition_count += 1;
                            }
                        }
                    } break;
                case SDL_QUIT: 
                    {
                        running = false;
                    } break;
            }
        }

        uint64_t current_tick = SDL_GetTicks64();
        uint64_t elapsed_ticks = current_tick - prev_tick;
        prev_tick = current_tick;
        lag += elapsed_ticks;

        while(lag >= MS_PER_TICK) {
            game_update(&keyboard_state);

            // TODO: Is this the best place to clear the transition count? In this state it's incredibly hard to catch a 'double press',
            // maybe the transition count should be reset at a slower rate indenpendently of the game update
            for(int key_id = 0; key_id < MAX_KEY_AMT; key_id++) {
                keyboard_state.keys[key_id].half_transition_count = 0;
            }

            lag -= MS_PER_TICK;
        }
        float dt = lag / MS_PER_TICK;

        glClear(GL_COLOR_BUFFER_BIT);

        game_render(dt);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("done\n");

    return 0;
}
