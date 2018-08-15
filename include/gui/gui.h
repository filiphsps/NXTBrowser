#pragma once
#include <iostream>

#include "../main.h"
#include "../sdl_helper.h"
#include "../html/utils.h"
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

            void renderWindow () { }

            void prepareTick() {
                // Get Window Size
                int h, w;
                SDL_GetRendererOutputSize(_renderer, &w, &h);
                //SDL_GetWindowSize(_window, &w, &h);
                DEVICE = {0, 0, w, h};

                SDL_RenderClear(_renderer);

                // Clear surfaces
                if(this->_gui_surface != NULL)
                    SDL_FreeSurface(this->_gui_surface);
                this->_gui_surface = SDL_CreateRGBSurface(0, DEVICE.w, DEVICE.h, 32, 0, 0, 0, 255);

                if(this->_overlay_surface != NULL)
                    SDL_FreeSurface(this->_overlay_surface);
                this->_overlay_surface = SDL_CreateRGBSurface(0, DEVICE.w, DEVICE.h, 32, 0, 0, 0, 0);
                SDL_SetColorKey(this->_overlay_surface, SDL_TRUE, SDL_MapRGB(this->_overlay_surface->format, 0, 0, 0));
            }

            bool doTick() {
                SDL_Rect screen_pos = {0, 0, DEVICE.w, DEVICE.h};
                SDL_Rect browser_pos_dst = {0, 75, DEVICE.w - SCROLLBAR_WIDTH, DEVICE.h};

                SDL_Texture *gui = SDL_CreateTextureFromSurface(_renderer, this->_gui_surface);
                SDL_RenderCopy(_renderer, gui, &screen_pos, &screen_pos);
                SDL_DestroyTexture(gui);

                SDL_Texture *browser = SDL_CreateTextureFromSurface(_renderer, this->_browser_surface);
                SDL_RenderCopy(_renderer, browser, &screen_pos, &browser_pos_dst);
                SDL_DestroyTexture(browser);

                SDL_Texture *overlay = SDL_CreateTextureFromSurface(_renderer, this->_overlay_surface);
                SDL_RenderCopy(_renderer, overlay, &screen_pos, &screen_pos);
                SDL_DestroyTexture(overlay);

                SDL_RenderPresent(_renderer);
            }
    };

    namespace UIElements {
        namespace AddressBar {
            void Render (browser::GUI *GUI) {
                #ifdef __SWITCH__
                    TTF_Font *font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", 22);
                #else
                    TTF_Font *font = browser::utils::get_font_from_cache("../../resources/fonts/NintendoStandard.ttf", 22);
                #endif

                sdl_helper::renderBackground (GUI->_gui_surface, {
                    0,
                    0,
                    DEVICE.w,
                    75 //TODO: Scaling
                }, {235, 235, 235, 255});
                sdl_helper::renderBackground (GUI->_gui_surface, {
                    100,
                    10,
                    DEVICE.w - 125,
                    55 //TODO: Scaling
                }, {255, 255, 255, 255});

                sdl_helper::renderText("http://www.example.com", GUI->_gui_surface, {115, 26, 250, 25}, 450, font, {0, 0, 0, 255});

                // Home Icon
                sdl_helper::renderBackground (GUI->_gui_surface, {
                    20,
                    10,
                    60,
                    55 //TODO: Scaling
                }, {255, 255, 255, 255});
            }
        }
        namespace Console {
            void Render (browser::GUI *GUI) {
                #ifdef __SWITCH__
                    TTF_Font *font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", 14);
                #elif __MACOS__
                    TTF_Font *font = browser::utils::get_font_from_cache("/Library/Fonts/Arial.ttf", 14);
                #else
                    TTF_Font *font = browser::utils::get_font_from_cache("../../resources/fonts/NintendoStandard.ttf", 14);
                #endif

                sdl_helper::renderBackground (GUI->_overlay_surface, {
                    (DEVICE.w - (DEBUG_CONSOLE_WIDTH + 30)) + 15,
                    15,
                    DEBUG_CONSOLE_WIDTH,
                    DEVICE.h - 30
                }, {180, 180, 180, 255});

                sdl_helper::renderText(console.getFormattedOutput(), GUI->_overlay_surface,
                    {((DEVICE.w - DEBUG_CONSOLE_WIDTH) - 30) + 30, 30, 0, 0}, DEBUG_CONSOLE_WIDTH, font, {66, 66, 66, 255});
            }
            void RenderStat (browser::GUI *GUI, short pos, std::string text) {
                #ifdef __SWITCH__
                    TTF_Font *font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", 28);
                #elif __MACOS__
                    TTF_Font *font = browser::utils::get_font_from_cache("/Library/Fonts/Arial.ttf", 28);
                #else
                    TTF_Font *font = browser::utils::get_font_from_cache("../../resources/fonts/NintendoStandard.ttf", 28);
                #endif

                sdl_helper::renderBackground (GUI->_overlay_surface, {
                    40,
                    DEVICE.h - (30+(40*pos)),
                    500,
                    35
                }, {180, 180, 180, 55});

                sdl_helper::renderText(text, GUI->_overlay_surface,
                    {40, DEVICE.h - (30+(40*pos)), DEVICE.w, 30}, DEVICE.w, font, {0, 255, 0, 255});
            }
        }
    }
}