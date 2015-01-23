#include <iostream>
#include "Mustard.hpp"

using std::vector;

int main(int argc, char *argv[])
{
    RenderContext context(640, 480);
    context.setWindowTitle("Pokedex with opengl");

    Sprite sprite;
    sprite.setTextureInfo(CreateTexture("assets/pokedex.png", TextureType::kT2RN));
    sprite.setPosition(glm::vec2(context.windowWidth()/2.0f - sprite.textureInfo().w*1.5/2.0f,
                                 context.windowHeight()/2.0f - sprite.textureInfo().h*1.5));
    sprite.setScale(glm::vec2(sprite.textureInfo().w*2, sprite.textureInfo().h*2));
    int initialY = sprite.position().y;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    bool doQuit = false;
    Uint32 lastTicks = SDL_GetTicks();

    while (!doQuit) {
        
        /* Poll for Events */
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
        sprite.setY(initialY + 5*sin(lastTicks/100.0f));

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glDepthFunc(GL_LEQUAL);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        context.world()->draw(&sprite);
        context.swapWindow();
    }

    return 0;
}
