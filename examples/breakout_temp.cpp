// The idea is that resources such as models, entities, probably more, are statically allocated. The user
// will call functions to get pointers to these resources.
static p2d_quad_entity *paddle;
static p2d_quad_entity bricks[32];

void game_init() {
    // Models doe
    p2d_quad_model *paddle_model = p2p_quad_model_new(width, height);
    //p2d_quad_model textured_paddle_model = p2p_quad_model_new(width, height, "path/to/texture");
    
    // Initialize shaders, create shader program
    
    // The entity variables would have to be globals 
    paddle = p2d_quad_entity_new(position, rotation, (const *p2p_quad_model)&paddle_model, shader_program);
    //p2d_textured_quad_entity paddle = p2d_quad_textured_entity_new(position, rotation, &textured_paddle_model);


}

void game_update(keyboard_state, delta) {
    // Decide state of game (eg. MENU, INGAME, POSTGAME)
    // switch(global_game_state) {
    //      MENU: 
    //          keyboard press -> switch menu selection
    //      INGAME: 
    //          Update paddle position based on keyboard state
    //          Update ball position
    //          Check ball-paddle collision
    //          Check ball-bricks collision
    //          Particle updates
    //          ...
    //
    //      and so on..
    // }
    
}

void game_render(delta) {
    // Decide state of game (eg. MENU, INGAME, POSTGAME)
    //
    // Example INGAME state:
    p2d_quad_render(&paddle);

    for(int i = 0; i < NR_OF_BRICKS; i++) {
        p2d_quad_render(&bricks[i]);
    }
}
