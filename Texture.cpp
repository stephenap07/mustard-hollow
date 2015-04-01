#include <cstdio>
#include <cstdlib>
#include <cassert>

#include <SDL2/SDL_image.h>

#include "ScopeExit.hpp"
#include "Drawing.hpp"


GLuint CreateTextureFromSurface(SDL_Surface *surface, GLenum target)
{
	assert(surface != nullptr);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(target, textureID);

	GLenum format;
    GLint internalFormat = GL_RGB8;
	switch (surface->format->BytesPerPixel) {
        case 1:
            format = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
            break;
		case 4:
			format = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
            internalFormat = GL_RGBA8;
			break;
		case 3:
			format = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
			break;
		default:
            fprintf(stderr, "Invalid format for surface: %d\n", surface->format->BytesPerPixel);
			assert(false);
	}

    switch(target) {
        case GL_TEXTURE_1D:
            glTexImage1D(
                target,
                0, internalFormat,
                surface->w, 0,
                format, GL_UNSIGNED_BYTE,
                surface->pixels);
            break;
        case GL_TEXTURE_2D:
            glTexImage2D(
                target,
                0, internalFormat,
                surface->w, surface->h, 0,
                format, GL_UNSIGNED_BYTE,
                surface->pixels);
            break;
        default:
            fprintf(stderr, "Unsupported texture target\n");
    }

	glBindTexture(target, 0);

    return textureID;
}

TextureInfo CreateTexture(const char *filename, TextureType textureType)
{
    TextureInfo texInfo;

    SDL_Surface *optimizedSurface = nullptr;
    SDL_Surface *surface = IMG_Load(filename);
    if (surface) {
        optimizedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0);
    }
    SCOPE_EXIT(if (optimizedSurface) SDL_FreeSurface(optimizedSurface););
    SDL_FreeSurface(surface);

    texInfo.w = optimizedSurface->w;
    texInfo.h = optimizedSurface->h;
    
    if (textureType == TextureType::kT2RL) {
        texInfo.texture = CreateTextureFromSurface(optimizedSurface, GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, texInfo.texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    return texInfo;
}
