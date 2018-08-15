#pragma once
#include <iostream>

#include "../main.h"
#include "../sdl_helper.h"
#include "../console.h"

extern Console console;
extern SDL_Rect DEVICE;

//#define SCROLLBAR_WIDTH 25
#define SCROLLBAR_WIDTH 0 //TODO

namespace browser {
    class GUI {
        public:
            SDL_Surface *_gui_surface = NULL;
            SDL_Surface *_browser_surface = NULL;
            SDL_Surface *_overlay_surface = NULL;

            GUI() {
                sdl_helper::init();
                console = Console();
            }
            ~GUI() {
                sdl_helper::exit();
            }

            void renderWindow () {
                #ifdef __SWITCH__
                    sdl_helper::drawTexture(this->_gui_surface, "romfs:/gui/browser.png", 0, 0);
                #else
                    sdl_helper::drawTexture(this->_gui_surface, "../../romFS/gui/browser.png", 0, 0);
                #endif
            }

            void prepareTick() {
                // Get Window Size
                int h, w;
                SDL_GetWindowSize(_window, &w, &h);
                DEVICE = {0, 0, w, h};

                SDL_RenderClear(_renderer);

                // Clear surfaces
                if(this->_gui_surface != NULL)
                    SDL_FreeSurface(this->_gui_surface);
                this->_gui_surface = SDL_CreateRGBSurface(0, DEVICE.w, DEVICE.w, 32, 0, 0, 0, 255);

                if(this->_overlay_surface != NULL)
                    SDL_FreeSurface(this->_overlay_surface);
                this->_overlay_surface = SDL_CreateRGBSurface(0, DEVICE.w, DEVICE.h, 32, 0, 0, 0, 0);
                SDL_SetColorKey(this->_overlay_surface, SDL_TRUE, SDL_MapRGB(this->_overlay_surface->format, 0, 0, 0));
            }

            void doTick() {
                SDL_Rect screen_pos = {0, 0, DEVICE.w, DEVICE.h};
                SDL_Rect browser_pos_src = {0, 0, DEVICE.w - SCROLLBAR_WIDTH, DEVICE.h};
                SDL_Rect browser_pos_dst = {0, 68, DEVICE.w - SCROLLBAR_WIDTH, DEVICE.h};

                SDL_RenderCopy(_renderer, SDL_CreateTextureFromSurface(_renderer, this->_gui_surface), &screen_pos, &screen_pos);
                SDL_RenderCopy(_renderer, SDL_CreateTextureFromSurface(_renderer, this->_browser_surface), &browser_pos_src, &browser_pos_dst);
                SDL_RenderCopy(_renderer, SDL_CreateTextureFromSurface(_renderer, this->_overlay_surface), NULL, NULL);
                SDL_RenderPresent(_renderer);

                SDL_PumpEvents();
                SDL_Delay(150);
            }
    };

    namespace UIElements {
        namespace AddressBar {
            void Render (browser::GUI *GUI) {
                #ifdef __SWITCH__
                    TTF_Font *font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", 14);
                #else
                    TTF_Font *font = browser::utils::get_font_from_cache("../../romFS/fonts/NintendoStandard.ttf", 14);
                #endif

                sdl_helper::renderText("http://www.example.com", GUI->_gui_surface, {25, 27, 250, 25}, 450, font, {0, 0, 0, 255});
            }
        }
        namespace Console {
            void Render (browser::GUI *GUI) {
                #ifdef __SWITCH__
                    TTF_Font *font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", 14);
                #else
                    TTF_Font *font = browser::utils::get_font_from_cache("../../romFS/fonts/NintendoStandard.ttf", 14);
                #endif

                sdl_helper::renderBackground (GUI->_overlay_surface, {
                    (DEVICE.w - DEBUG_CONSOLE_WIDTH - 30) + 15,
                    15,
                    DEBUG_CONSOLE_WIDTH,
                    DEVICE.h - 30
                }, {180, 180, 180, 55});

                sdl_helper::renderText(console.getFormattedOutput(), GUI->_overlay_surface,
                    {((DEVICE.w - DEBUG_CONSOLE_WIDTH) - 30) + 30, 30, 0, 0}, DEBUG_CONSOLE_WIDTH, font, {66, 66, 66, 255});
            }
        }
    }
}