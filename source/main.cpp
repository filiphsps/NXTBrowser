#include <string.h>
#include <fstream>
#include <stdio.h>
#include <iostream>

#include <switch.h>

#include "main.h"
#include "html.h"
#include "console.h"

using namespace std;

Console console;
SDL_Surface *_browser_surface;
SDL_Rect _browser_position = { 0, 68, 1280, 720 };
SDL_Rect _scroll_position = { 0, 0, 1280, browser_height };

bool dom_parser (std::string source) {
    int position = 0; // TODO: scroll

    tinyxml2::XMLDocument doc;
    doc.Parse((const char*)source.c_str(), source.size());

    SDL_Surface* browser = SDL_CreateRGBSurface(0, 1280, browser_height, 32, 0, 0, 0, 0);
    SDL_FillRect(browser, NULL, SDL_MapRGBA(browser->format, 255, 255, 255, 255));
    SDL_BlitSurface(browser, NULL, _browser_surface, NULL);

    auto element = doc.FirstChildElement("html")->FirstChildElement("body");
    for(const tinyxml2::XMLElement* child = element->FirstChildElement(); child!=0; child=child->NextSiblingElement()) {

        // TODO: Do this properly
        std::string type = child->Value();

        browser::element_data elementData;
        elementData.height = position;
        position = browser::parser::html_parser(child, type, position, &elementData);
    }
    
    console.printf("position: " + std::to_string(position));
    console.printf("browser_height: " + std::to_string(browser_height));

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

bool PAGE_ERROR = false;
int main(int argc, char **argv) {
    romfsInit();
    sdl_helper::init();

    console = Console();
    
    TTF_Font *font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", 14);

    std::ifstream ifs("romfs:/pages/test.html");
    std::string page(std::istreambuf_iterator<char>{ifs}, {});

    std::string title = "Error loading the page...";

    tinyxml2::XMLDocument doc;

    // Validate and fix current page source
    page = browser::validator::validate_and_fix(page);
    doc.Parse((const char*)page.c_str(), page.size());

    if (!doc.ErrorID()) {
        title = doc.FirstChildElement("html")->FirstChildElement("head")->FirstChildElement("title")->GetText();
    } else {
        PAGE_ERROR = true;
        console.printf("DOM->Threw error with ID: " + std::to_string(doc.ErrorID()));
    }

    _browser_surface = SDL_CreateRGBSurface(0, 1280, browser_height, 32, 0, 0, 0, 255);

    bool DOM_UPDATE = true;
    int i = 0;
    while(appletMainLoop()) {
        // FIXME: only render on change

        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) {
            break;
        } else if (kDown & KEY_B) {
            console.printf("DOM->Current height: " + std::to_string(browser_height));
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

        SDL_RenderClear(_renderer);
        sdl_helper::drawTexture(_surface, _background_texture, 0, 0);

        // Draw title only on DOM_UPDATE
        sdl_helper::drawText(_surface, 25, 27, title, font, false);

        if (DOM_UPDATE && !PAGE_ERROR) {
            // Update surface height
            SDL_FreeSurface(_browser_surface);
            _browser_surface = SDL_CreateRGBSurface(0, 1280, browser_height, 32, 0, 0, 0, 255);

            // TODO: Draw browser page
            if(!dom_parser(page))
                DOM_UPDATE = false;
        }

        if(!PAGE_ERROR)
            SDL_BlitSurface(_browser_surface, &_scroll_position, _surface, &_browser_position);
        else {
            // Error code
            sdl_helper::drawText(_surface, 200, 640, doc.ErrorName(), font, false, 55, 55, 55, 55);
        }

        #ifdef DEBUG
            // Console log
            sdl_helper::drawRect(_surface, (1280 - 250 - 30) + 15, 15, 250, 720 - 30, 55, 55, 55, 155);
            sdl_helper::drawText(_surface, (1280 - 250 - 30) + 30, 30, console.getFormattedOutput(), font, false, 255, 0, 0, 55);
        #endif
        SDL_RenderPresent(_renderer);
    }
    
    sdl_helper::exit();
    romfsExit();
    return 0;
}