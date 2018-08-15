#include <string.h>
#include <fstream>
#include <stdio.h>
#include <iostream>

#ifdef __SWITCH__
#include <switch.h>
#else

#endif

#include "main.h"
#include "html.h"
#include "console.h"

using namespace std;

Console console;
SDL_Surface *_browser_surface;
SDL_Rect _browser_position = { 0, 68, DEVICE_WIDTH, DEVICE_HEIGHT };
SDL_Rect _scroll_position = { 0, 0, DEVICE_WIDTH, browser_height };

tinyxml2::XMLDocument doc;
bool dom_parser (std::string source) {
    int position = 0; // TODO: good scroll
    
    doc.Parse((const char*)source.c_str(), source.size());

    SDL_Surface* browser = SDL_CreateRGBSurface(0, 1280, browser_height, 32, 0, 0, 0, 0);
    SDL_FillRect(browser, NULL, SDL_MapRGBA(browser->format, 255, 255, 255, 255));
    SDL_BlitSurface(browser, NULL, _browser_surface, NULL);

    auto element = doc.FirstChildElement("html")->FirstChildElement("body");
    for(const tinyxml2::XMLElement* child = element->FirstChildElement(); child != NULL; child=child->NextSiblingElement()) {
        // TODO: Do this properly
        std::string type = child->Value();
        position = browser::parser::html_parser(child, type, position);
    }

    SDL_FreeSurface(browser);

    if (position > browser_height) {
        browser_height = position;
        _scroll_position.h = browser_height;

        // Re-render since we failed due to height last time around.
        return true;
    } else if (position < 720) {
        browser_height = 720;
        _scroll_position.h = browser_height;
    }

    return false;
}

void update_window_size () {
    int h, w;
    SDL_GetWindowSize(_window, &w, &h);

    _browser_position.w = w;
    _scroll_position.w = w;
    _browser_position.h = h;
    _scroll_position.h = h;
}

int main(int argc, char **argv) {
    #ifdef __SWITCH__
    romfsInit();
    #endif
    sdl_helper::init();
    console = Console();

    std::ifstream ifs;
    
    #ifdef __SWITCH__
    TTF_Font *font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", 14);
    ifs = std::ifstream("romfs:/pages/test.html");
    #else
    TTF_Font *font = browser::utils::get_font_from_cache("../../romFS/fonts/NintendoStandard.ttf", 14);
    ifs = std::ifstream("/Users/filiphsandstrom/Desktop/simpleweb/romFS/pages/test.html");
    #endif

    std::string page(std::istreambuf_iterator<char>{ifs}, {});
    std::string title = "Error loading the page...";

    // Validate and fix current page source
    page = browser::validator::validate_and_fix(page);
    doc.Parse((const char*)page.c_str(), page.size());

    if (!doc.ErrorID()) {
        title = doc.FirstChildElement("html")->FirstChildElement("head")->FirstChildElement("title")->GetText();
    }

    #ifdef __SWITCH__
    while(appletMainLoop()) {
    #else
    while(true) {
    #endif
        // FIXME: only render on change

        #ifdef __SWITCH__
            hidScanInput();
            u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
            u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);

            if (kDown & KEY_PLUS) {
                break;
            } else if (kDown & KEY_B) {
                console.hidden = !console.hidden;
            }

            if (kHeld & KEY_DDOWN) {
                _scroll_position.y += 5;

                if ((_scroll_position.y + (720 - 68)) >= browser_height)
                    _scroll_position.y = browser_height - 720 + 68; // browser_height + device_height + offset;
            } else if (kHeld & KEY_DUP) {
                _scroll_position.y -= 5;

                if (_scroll_position.y <= 0)
                    _scroll_position.y = 0;
            }
        #endif

        update_window_size();
        SDL_RenderClear(_renderer);

        if(_surface != NULL)
            SDL_FreeSurface(_surface);
        _surface = SDL_CreateRGBSurface(0, _browser_position.w, _browser_position.h, 32, 0, 0, 0, 255);

        if(_browser_surface != NULL)
            SDL_FreeSurface(_browser_surface);
        _browser_surface = SDL_CreateRGBSurface(0, _browser_position.w, _browser_position.h, 32, 0, 0, 0, 255);

        #ifdef __SWITCH__
        sdl_helper::drawTexture(_surface, "romfs:/gui/browser.png", 0, 0);
        #else
        sdl_helper::drawTexture(_surface, "../../romFS/gui/browser.png", 0, 0);
        #endif

        // UI
        sdl_helper::renderText(title, _surface, {25, 27, 250, 25}, 450, font, {0, 0, 0, 255});

        // DOM
        dom_parser(page);

        #ifdef DEBUG
            // Console log
            if (!console.hidden) {
                //sdl_helper::drawRect(_surface, (_browser_position.w - DEBUG_CONSOLE_WIDTH - 30) + 15, 15, DEBUG_CONSOLE_WIDTH, 720 - 30, 55, 55, 55, 155);
                sdl_helper::renderText(console.getFormattedOutput(), _surface, {((_browser_position.w - DEBUG_CONSOLE_WIDTH) - 30) + 30, 30, 0, 0}, DEBUG_CONSOLE_WIDTH, font, {255, 0, 0, 255});
            }
        #endif

        SDL_Rect pos = {0, 0, _browser_position.w, _browser_position.h};
        SDL_RenderCopy(_renderer, SDL_CreateTextureFromSurface(_renderer, _surface), &pos, &pos);
        SDL_RenderCopy(_renderer, SDL_CreateTextureFromSurface(_renderer, _browser_surface), &pos, &_browser_position);
        SDL_RenderPresent(_renderer);
        SDL_PumpEvents();
        SDL_Delay(150);
    }
    
    sdl_helper::exit();

    #ifdef __SWITCH__
    romfsExit();
    #endif
    return 0;
}