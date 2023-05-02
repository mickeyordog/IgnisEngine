#include <SDL.h>
#include "text.h"

Text::Text(const char* text, TTF_Font* font, Renderer* renderer)
{
    SDL_Surface* surface = TTF_RenderUTF8_Solid_Wrapped(font, text, SDL_Color{0, 0, 0, 255}, 640);
	this->texture = SDL_CreateTextureFromSurface(renderer->getRenderer(), surface);
	SDL_FreeSurface(surface);
}

Text::~Text()
{
    SDL_DestroyTexture(this->texture);
}

void Text::render(Renderer* renderer)
{
    
}
