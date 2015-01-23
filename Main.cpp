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

    Sprite sprite;
    sprite.setTextureInfo(CreateTexture("smw_ground.png", TextureType::kT2RL));
    sprite.setSubRect(Rect({ 137.0f, 99.0f, 16.0f, 16.0f }));
    sprite.setPosition(glm::vec2(0, 0));

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    bool do_quit = false;
    Uint32 lastTicks = SDL_GetTicks();

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

        // Get ticks in ms
        Uint32 deltaTicks = SDL_GetTicks() - lastTicks;
        lastTicks = SDL_GetTicks();
        {
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            const float blockSpeed = 100.0f;
            const float delta = deltaTicks/1000.0f;
            if (state[SDL_SCANCODE_RIGHT]) {
                float x = sprite.position().x;
                float y = sprite.position().y;
                sprite.setPosition(glm::vec2(x + blockSpeed*delta, y));
            } else if(state[SDL_SCANCODE_LEFT]) {
                float x = sprite.position().x;
                float y = sprite.position().y;
                sprite.setPosition(glm::vec2(x - blockSpeed*delta, y));
            }
        }
        
        //SetUniform(text_sprite_props.program, UniformType::k1i, "uni_time", 1, (GLvoid*)&ticks);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glDepthFunc(GL_LEQUAL);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tileMap.draw(context.world());
        context.world()->draw(&sprite);
        context.swapWindow();
    }

    return 0;
}
