/**
 * Artificial Intelligence Final Project
 * Stephen Pridham
 *
 */

#include <iostream>
#include "Mustard.hpp"

using std::vector;

int main(int argc, char *argv[])
{
    RenderContext context(640, 480);
    context.setWindowTitle("Testing framework");

    TileMap tileMap;
    tileMap.load();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    bool do_quit = false;

    while (!do_quit) {
        
        /* Poll for Events */
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:
                    do_quit = true;
                    break;
                case SDL_KEYUP:
                    if (ev.key.keysym.sym == SDLK_ESCAPE) {
                        do_quit = true;
                    }
                    break;
            }
        }

        //Uint32 ticks = SDL_GetTicks();
        //SetUniform(text_sprite_props.program, UniformType::k1i, "uni_time", 1, (GLvoid*)&ticks);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glDepthFunc(GL_LEQUAL);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tileMap.draw(context.world());
        context.swapWindow();
    }

    return 0;
}
