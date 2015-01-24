#pragma once

#include "Sprite.hpp"

struct RenderMessage {
    glm::mat4 transform;
    TextureInfo textureInfo;
    VertexBuffer vertexBuffer;
};

class Material {
    TextureInfo textureInfo;
};

class RenderWorld {
public:
    RenderWorld(glm::mat4 newProjection, glm::mat4 newView);
    RenderWorld();

    const glm::mat4 projection() const;
    void setProjection(glm::mat4 newProjection);
    const glm::mat4 view() const;
    void setView(glm::mat4 newView);
    const glm::mat4 transform() const;
    void draw(Sprite *sprite) const;
    void draw(RenderMessage *message) const;

private:
    glm::mat4 m_projection;
    glm::mat4 m_view;
};

class RenderContext {
public:
    RenderContext(float screen_width, float screen_height);
    RenderContext();
    ~RenderContext();

    SDL_Window *window();
    RenderWorld *world();
    const std::string windowTitle() const;
    void setWindowTitle(const std::string &newTitle);
    void swapWindow();

    const float windowWidth() const;
    const float windowHeight() const;

private:
    void init(const float screenWidth, const float screenHeight);

    RenderWorld m_world;
    SDL_Window *m_window;
    SDL_GLContext m_openGLContext;
    float m_height;
    float m_width;
};
