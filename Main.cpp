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

        context.clearWindow();
        tileMap.draw(context.world());
        context.swapWindow();
    }

    return 0;
}
