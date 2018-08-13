#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

static SDL_Window *_window;
static SDL_Renderer *_renderer;
static SDL_Surface *_surface;
static SDL_Texture *_background_texture;

namespace sdl_helper {
    void init() {
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_CreateWindowAndRenderer(1280, 720, 0, &_window, &_renderer);
        _surface = SDL_GetWindowSurface(_window);
        SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP | IMG_INIT_TIF);
        TTF_Init();
        SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

        SDL_Surface *_background_surface = IMG_Load("romfs:/gui/browser.png");
        _background_texture = SDL_CreateTextureFromSurface(_renderer, _background_surface);
    }

    void exit() {
        TTF_Quit();
        IMG_Quit();
        SDL_DestroyRenderer(_renderer);
        //SDL_FreeSurface(_surface);
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }

    void drawTexture(SDL_Surface *surf, SDL_Texture *tex, int x, int y) {
        SDL_Rect position;
        position.x = x;
        position.y = y;
        position.w = surf->w;
        position.h = surf->h;
        SDL_RenderCopy(_renderer, tex, NULL, &position);
    }

    void drawText(SDL_Surface *surf, int x, int y, std::string text, TTF_Font *font, bool center = false) {
        SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), {
            r: 0,
            g: 0,
            b: 0,
            a: 255
        }, 1280);
        SDL_SetSurfaceAlphaMod(surface, 255);
        SDL_Rect position;
        if (!center)
            position = { x, y, surf->w, surf->h };
        else {
            int screen_w = (surf->w / 2);
            int text_w = (surface->w / 2);

            position = { (screen_w - text_w) + x, y, surf->w, surf->h };
        }
        SDL_BlitSurface(surface, NULL, surf, &position);
        SDL_FreeSurface(surface);
    }

    void print ();
}