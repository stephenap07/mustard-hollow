/**
 * Artificial Intelligence Final Project
 * Stephen Pridham
 *
 */

#include <iostream>
#include <vector>
#include <memory>
#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp> 


#include "ScopeExit.hpp"
#include "Drawing.hpp"
#include "Sprite.hpp"

using std::vector;

int main(int argc, char *argv[])
{
    RenderContext context;

    SCOPE_EXIT(if (context.gl_context) SDL_GL_DeleteContext(context.gl_context););
    SCOPE_EXIT(if (context.window) SDL_DestroyWindow(context.window););
    SCOPE_EXIT(IMG_Quit(););
    SCOPE_EXIT(SDL_Quit(););

    Init(&context, SCREEN_WIDTH, SCREEN_HEIGHT);

    vector<ShaderInfo> shaders = {
        {GL_VERTEX_SHADER, "triangles.vert",
         CreateShader("triangles.vert", GL_VERTEX_SHADER)},
        {GL_FRAGMENT_SHADER, "triangles.frag",
         CreateShader("triangles.frag", GL_FRAGMENT_SHADER)}
    };    
    const GLuint program = CreateProgram(shaders);

    TextureInfo tex_info = CreateTexture("smw_ground.png", TextureType::kT2RL);
    SCOPE_EXIT(glDeleteTextures(1, &tex_info.texture););

    VertexBuffer buffer = CreateQuad(program);

    Sprite sprite;
    sprite.vbuffer = buffer;
    sprite.tinfo = tex_info;
    sprite.rect = { 137.0f, 99.0f, 16.0f, 16.0f };

    glm::vec4 texture_xy_uv = glm::vec4(sprite.rect.x / tex_info.w, sprite.rect.y / tex_info.h, sprite.rect.w / tex_info.w, sprite.rect.h / tex_info.h);
    SetUniform(program, UniType::k4fv, "uni_tex_xy_uv", 1, glm::value_ptr(texture_xy_uv));

    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    SetUniform(program, UniType::k4fv, "uni_color", 1, glm::value_ptr(color));

    glm::mat4 model = glm::mat4(1.0);
    model = glm::scale(model, glm::vec3(sprite.rect.w / SCREEN_WIDTH, sprite.rect.h / SCREEN_HEIGHT, 1.0f));
    model = glm::scale(model, glm::vec3(20.0f));
    SetUniform(program, UniType::kMatrix4fv, "uni_model", 1, glm::value_ptr(model));

    GLint is_textured = 1;
    SetUniform(program, UniType::k1i, "uni_is_textured", 1, (GLvoid*)&is_textured);


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

        Uint32 ticks = SDL_GetTicks();
        SetUniform(program, UniType::k1i, "uni_time", 1, (GLvoid*)&ticks);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, sprite.tinfo.texture);
        DrawQuad(sprite.vbuffer);

        glFlush();
        SDL_GL_SwapWindow(context.window);
    }

    return 0;
}
