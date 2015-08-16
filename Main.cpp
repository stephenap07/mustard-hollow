#include <iostream>
#include "Mustard.hpp"

int main(int argc, char *argv[])
{
    RenderContext context(640, 480);
    context.setWindowTitle("Testing framework");

    TileMap tileMap;
    tileMap.load();
    bool doQuit = false;

    while (!doQuit) {
        
        // Poll for Events
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:
                    doQuit = true;
                    break;
                case SDL_KEYUP:
                    if (ev.key.keysym.sym == SDLK_ESCAPE) {
                        doQuit = true;
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
