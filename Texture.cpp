#include <iostream>
#include <SDL2/SDL_image.h>

#include "ScopeExit.hpp"
#include "Drawing.hpp"


namespace internal {

//==============================================================================
// Convert a SDL pixel to something OpenGL can understand.
inline void SDLToGLPixelFormat (
    const SDL_Surface *surface,
    GLenum &format,
    GLint &internalFormat)
{
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
            std::cerr << "Invalid format for surface: "
                      << surface->format->BytesPerPixel
                      << std::endl;
	}
}

} // end namespace internal

//==============================================================================
GLuint Create2DTextureFromSurface (SDL_Surface *surface)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLenum format;
    GLint internalFormat = GL_RGB8;
    internal::SDLToGLPixelFormat(surface, format, internalFormat);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        surface->w,
        surface->h,
        0,
        format,
        GL_UNSIGNED_BYTE,
        surface->pixels);

    return textureID;
}

//==============================================================================
TextureInfo Create2DTexture (const char *filename, TextureType textureType)
{
    TextureInfo texInfo;

    SDL_Surface *surface = IMG_Load(filename);
    SDL_Surface *optimizedSurface = SDL_ConvertSurfaceFormat(
        surface,
        SDL_PIXELFORMAT_ARGB8888,
        0);
    SDL_FreeSurface(surface);
    SCOPE_EXIT(SDL_FreeSurface(optimizedSurface););

    texInfo.w = optimizedSurface->w;
    texInfo.h = optimizedSurface->h;
    texInfo.texture = Create2DTextureFromSurface(optimizedSurface);

    glBindTexture(GL_TEXTURE_2D, texInfo.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texInfo;
}
