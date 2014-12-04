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
    context.Init(SCREEN_WIDTH, SCREEN_HEIGHT);

    vector<ShaderInfo> shaders = {
        ShaderInfo("Triangles.vert", GL_VERTEX_SHADER),
        ShaderInfo("Triangles.frag", GL_FRAGMENT_SHADER)
    };    

    GLuint program = CreateProgram(shaders);

    /* Sprite Creation */
    Sprite sprite;
    sprite.vbuffer = CreateQuad(program);
    sprite.tinfo = CreateTexture("smw_ground.png", TextureType::kT2RL);
    sprite.sub_rect = { 137.0f, 99.0f, 16.0f, 16.0f };
    sprite.pos.x = sprite.sub_rect.w / 2;
    sprite.pos.y = sprite.sub_rect.h / 2;

    /* Sprite properties */
    SpriteProperties sprite_props;
    sprite_props.program = program;
    sprite_props.is_textured = true;
    sprite_props.texture_xy_uv = glm::vec4(sprite.sub_rect.x / sprite.tinfo.w, sprite.sub_rect.y / sprite.tinfo.h, sprite.sub_rect.w / sprite.tinfo.w, sprite.sub_rect.h / sprite.tinfo.h);
    sprite_props.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    SetUniform(sprite_props.program, UniType::k4fv, "uni_tex_xy_uv", 1, glm::value_ptr(sprite_props.texture_xy_uv));
    SetUniform(sprite_props.program, UniType::k4fv, "uni_color", 1, glm::value_ptr(sprite_props.color));

    /* Sprite Destruction */
    SCOPE_EXIT(glDeleteTextures(1, &sprite.tinfo.texture););
    SCOPE_EXIT(glDeleteProgram(sprite.vbuffer.program););
    SCOPE_EXIT(glDeleteBuffers(1, &sprite.vbuffer.vbo););
    SCOPE_EXIT(glDeleteBuffers(1, &sprite.vbuffer.ebo););

    glm::mat4 proj = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f); 
    glm::mat4 view = glm::lookAt(
        glm::vec3(0, 0, 1), // pos
        glm::vec3(0, 0, 0), // look
        glm::vec3(0, 1, 0)  // up
    );

    RenderWorld world;
    world.proj = proj;
    world.view = view;

/*
    glGenBuffers(1, &global_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, global_ubo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, global_uniform_binding,
                      global_ubo, 0, 2 * sizeof(glm::mat4));

    glBindBuffer(GL_UNIFORM_BUFFER, global_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection)); 
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    */

    GLint is_textured = 1;
    SetUniform(sprite.vbuffer.program, UniType::k1i, "uni_is_textured", 1, (GLvoid*)&is_textured);

    std::vector<std::vector<int>> map = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };

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
        SetUniform(sprite_props.program, UniType::k1i, "uni_time", 1, (GLvoid*)&ticks);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for (int j = 0; j < map.size(); ++j) { 
            for (int i = 0; i < map[j].size(); ++i) {
                sprite.pos.x = i * sprite.sub_rect.w + (sprite.sub_rect.w / 2);
                sprite.pos.y = j * sprite.sub_rect.h + (sprite.sub_rect.h / 2);
                DrawSprite(world, sprite);
            }
        }

        glFlush();
        SDL_GL_SwapWindow(context.window);
    }

    return 0;
}
