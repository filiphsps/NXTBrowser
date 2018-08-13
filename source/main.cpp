#include <string.h>
#include <fstream>
#include <stdio.h>
#include <iostream>

#include <switch.h>

#include "tinyxml2.h"
#include "sdl_helper.h"

using namespace std;

struct padding_data {
    int top;
    int right;
    int bottom;
    int left;
};

struct rgb_color_data {
    int r;
    int g;
    int b;
    int a;
};

struct element_data {
    int height;
    int width;
    padding_data padding;
    padding_data margin;

    bool center = false;
    bool floatLeft;
    bool floatRight;

    rgb_color_data color;
    rgb_color_data background;
};

struct page_data {
    int width;
    int height;
    std::string pageTitle;

    element_data styling;
};

std::string console_output = "Console output:\n";
std::string page ="";

int browser_height = 720;

SDL_Surface *_browser_surface;
SDL_Rect _browser_position = { 0, 68, 1280, 720 };
SDL_Rect _scroll_position = { 0, 0, 1280, browser_height };

int html_parser (const tinyxml2::XMLElement* child, std::string type, int position, element_data* elementData) {
    // H tags
    if (type.length() == 2 && type.at(0) == 'h') {
        // Dynamically generate h1-h6
        if (std::isdigit(type.at(1))) {
            std::string tag = type;
            tag.erase(0, 1);

            int h_type = std::stoi(tag); // h[n] where n = h_type
            int diff = 0;

            if (h_type != 1)
                diff = (h_type * 2);

            int padding = 15 - diff;
            int font_size = 32 - diff;

            if (h_type >= 7)
                return position; // H6 is the largest heading

            position += padding;

            TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", font_size);
            sdl_helper::drawText(_browser_surface, 15, position, child->GetText(), font, elementData->center);
            TTF_CloseFont(font);

            position += font_size + padding;
        }
    } else if (type == "p") {
        position += 5;

        TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", 16);
        sdl_helper::drawText(_browser_surface, 15, position, child->GetText(), font, elementData->center);
        TTF_CloseFont(font);

        position += 16 + 5;
    } else if (type == "code") {
        // FIXME: code tag
        position += 5;

        TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", 16);
        sdl_helper::drawText(_browser_surface, 15, position, child->GetText(), font, elementData->center);
        TTF_CloseFont(font);

        position += 16 + 5;
    } else if (type == "br") {
        position += 15;
    } else if (type == "center") {
        elementData->center = true;
    } else {
        console_output.append("unsupported tag '");
        console_output.append(type);
        console_output.append("'\n");
    }

    auto element = child;
    for(const tinyxml2::XMLElement* c = element->FirstChildElement(); c!=0; c=c->NextSiblingElement()) {
        std::string type = c->Value(); // TODO recursive
        position = html_parser(c, type, position, elementData);
    }
    return position;
}

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

        element_data* elementData;
        position = html_parser(child, type, position, elementData);
    }
    
    console_output.append(std::to_string(position));
    console_output.append("\n");
    console_output.append(std::to_string(browser_height));
    console_output.append("\n");

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

int main(int argc, char **argv) {
    romfsInit();
    sdl_helper::init();

    TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", 14);

    std::ifstream ifs("romfs:/pages/test.html");
    std::string page(std::istreambuf_iterator<char>{ifs}, {});

    //tinyxml2::XMLDocument doc;
    //doc.Parse((const char*)page.c_str(), page.size());

    _browser_surface = SDL_CreateRGBSurface(0, 1280, browser_height, 32, 0, 0, 0, 255);

    bool DOM_UPDATE = true;
    while(appletMainLoop()) {
        // FIXME: only render on change

        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break;
        else if (kDown & KEY_B) {
            console_output.append(std::to_string(_scroll_position.y));
            console_output.append("\n");
            console_output.append(std::to_string(browser_height));
            console_output.append("\n");
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

        if (DOM_UPDATE) {
            // Update surface height
            SDL_FreeSurface(_browser_surface);
            _browser_surface = SDL_CreateRGBSurface(0, 1280, browser_height, 32, 0, 0, 0, 255);

            // TODO: Draw browser page
            if(!dom_parser(page))
                DOM_UPDATE = false;

            // Draw title
            //sdl_helper::drawText(25, 27, doc.FirstChildElement("html")->FirstChildElement("head")->FirstChildElement("title")->GetText(), font, false);
        }

        SDL_BlitSurface(_browser_surface, &_scroll_position, _surface, &_browser_position);

        // Console log
        sdl_helper::drawText(_surface, 5, 0, console_output, font, false);

        SDL_RenderPresent(_renderer);
    }
    
    sdl_helper::exit();
    romfsExit();
    return 0;
}