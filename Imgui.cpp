#include <vector>
#include <SDL2/SDL.h>
#include "Imgui.h"
#include "Drawing.hpp"

namespace ui {
	static const SDL_Color CYAN   = {0, 255, 255};
	static const SDL_Color RED    = {255, 0, 0};
	static const SDL_Color GREEN  = {0, 255, 0};

    Context::Context()
    {
    }

	void Context::begin()
	{
		hotItem = 0;
	}

	void Context::end()
	{
		if (!mouseDown) {
			activeItem = 0;
		} else {
			if (activeItem == 0) {
				activeItem = -1;
			}
		}
	}

	bool Context::mouseHit(int x, int y, int w, int h)
	{
		if (mouseX < x || mouseY < y || mouseX >= x + w || mouseY >= y + h)
			return false;
		return true;
	}

	void Context::drawRect(SDL_Renderer *renderer, int x, int y, int w, int h, const SDL_Color color)
	{
		SDL_Rect rect = {x, y, w, h};
		uint8_t r, g, b, a;
		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	bool Context::button(int id, SDL_Renderer *renderer, int x, int y, int w, int h)
	{
		SDL_Color color = CYAN;

		if (mouseHit(x, y, w, h)) {
			hotItem = id;
			if (activeItem == 0 && mouseDown) {
				activeItem = id;
			}
		}
		if (hotItem == id) {
			if (activeItem == id) {
				color = GREEN;
			} else {
				color = RED;
			}
		}

		drawRect(renderer, x, y, w, h, color);

		if (hotItem == id && activeItem == id && !mouseDown) {
			return true;
		}

		return false;
	}
} // namespace ui
