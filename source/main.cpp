#include <string.h>
#include <fstream>
#include <stdio.h>
#include <iostream>

#ifdef __SWITCH__
#include <switch.h>
#else

#endif

#include "main.h"
#include "html/html.h"
#include "gui/gui.h"
#include "console.h"

using namespace std;

Console console;

SDL_Rect DEVICE = {0, 0, 0, 0};

tinyxml2::XMLDocument doc;
bool dom_parser (std::string source, browser::GUI* GUI) {
    int position = 0; // TODO: good scroll

    if(GUI->_browser_surface != NULL)
        SDL_FreeSurface(GUI->_browser_surface);
    GUI->_browser_surface = SDL_CreateRGBSurface(0, DEVICE.w - SCROLLBAR_WIDTH, browser_height, 32, 0, 0, 0, 255);
    
    doc.Parse((const char*)source.c_str(), source.size());

    SDL_Surface* browser = SDL_CreateRGBSurface(0, DEVICE.w, browser_height, 32, 0, 0, 0, 0);
    SDL_FillRect(browser, NULL, SDL_MapRGBA(browser->format, 255, 255, 255, 255));
    SDL_BlitSurface(browser, NULL, GUI->_browser_surface, NULL);
    SDL_FreeSurface(browser);

    auto element = doc.FirstChildElement("html")->FirstChildElement("body");
    for(const tinyxml2::XMLElement* child = element->FirstChildElement(); child != NULL; child=child->NextSiblingElement()) {
        // TODO: Do this properly
        std::string type = child->Value();
        position = browser::parser::html_parser(child, type, position, GUI->_browser_surface);
    }

    if (position > browser_height) {
        browser_height = position;
        //_scroll_position.h = browser_height;

        // Re-render since we failed due to height last time around.
        return true;
    } else if (position < DEVICE.h) {
        browser_height = DEVICE.h;
       // _scroll_position.h = browser_height;
    }

    return false;
}

void update_window_size () {
    int h, w;
    SDL_GetWindowSize(_window, &w, &h);

    DEVICE = {0, 0, w, h};
}

int main(int argc, char **argv) {
    browser::GUI *GUI = new browser::GUI();

    std::ifstream ifs;
    
    #ifdef __SWITCH__
    romfsInit();

    TTF_Font *font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", 14);
    ifs = std::ifstream("romfs:/pages/test.html");
    #else
    TTF_Font *font = browser::utils::get_font_from_cache("../../romFS/fonts/NintendoStandard.ttf", 14);
    ifs = std::ifstream("/Users/filiphsandstrom/Desktop/simpleweb/romFS/pages/test.html");
    #endif

    std::string page(std::istreambuf_iterator<char>{ifs}, {});

    // Validate and fix current page source
    page = browser::validator::validate_and_fix(page);
    doc.Parse((const char*)page.c_str(), page.size());

    #ifdef __SWITCH__
    while(appletMainLoop()) {
    #else
    while(true) {
    #endif
        update_window_size();
        //INPUT->prepareTick();
        //NET->prepareTick();
        GUI->prepareTick();

        GUI->renderWindow();

        // DOM
        dom_parser(page, GUI);

        browser::UIElements::AddressBar::Render(GUI);
        browser::UIElements::Console::Render(GUI);
        //INPUT->doTick();
        //NET->doTick();
        GUI->doTick();
    }
    
    #ifdef __SWITCH__
    romfsExit();
    #endif
    return 0;
}