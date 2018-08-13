#include <string.h>
#include <stdio.h>
#include <iostream>

#include <switch.h>

#include "tinyxml2.h"
#include "sdl_helper.h"

using namespace std;

std::string console_output = "Hello World!\n";
std::string page =
"<html> \
    <head><title>Test page</title></head> \
    <body> \
        <h1>A title!</h1> \
        <p>Hello World</p> \
    </body> \
</html>";

void dom_parser (std::string source) {
    int position = 68;

    tinyxml2::XMLDocument doc;
    doc.Parse((const char*)source.c_str(), source.size());

    SDL_Surface* browser = SDL_CreateRGBSurface(0, 1280, 720, 32, 0, 0, 0, 0);
    SDL_FillRect(browser, NULL, SDL_MapRGB(browser->format, 255, 255, 255));
    sdl_helper::drawTexture(_surface, SDL_CreateTextureFromSurface(_renderer, browser), 0, 68);

    auto element = doc.FirstChildElement("html")->FirstChildElement("body");
    for(const tinyxml2::XMLElement* child = element->FirstChildElement(); child!=0; child=child->NextSiblingElement()) {

        // TODO: Do this properly
        std::string type = child->Value();
        if (type == "h1") {
            position += 10;

            TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", 32);
            sdl_helper::drawText(15, position, child->GetText(), font);

            position += 32 + 10;
        } else if (type == "p") {
            position += 5;

            TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", 16);
            sdl_helper::drawText(15, position, child->GetText(), font);

            position += 16 + 5;
        } else {
            console_output.append("unsupported tag '");
            console_output.append(type);
            console_output.append("'\n");
        }
    }

    SDL_FreeSurface(browser);
    return;
}

int main(int argc, char **argv) {
    romfsInit();
    sdl_helper::init();

    TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", 14);

    tinyxml2::XMLDocument doc;
    doc.Parse((const char*)page.c_str(), page.size());

    while(appletMainLoop()) {
        SDL_RenderClear(_renderer);

        sdl_helper::drawTexture(_surface, _background_texture, 0, 0);

        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break;

        // TODO: Draw browser page
        dom_parser(page);

        // Draw title
        sdl_helper::drawText(25, 27, doc.FirstChildElement("html")->FirstChildElement("head")->FirstChildElement("title")->GetText(), font);

        // Console log
        sdl_helper::drawText(0, 0, console_output, font);

        SDL_RenderPresent(_renderer);
    }
    
    sdl_helper::exit();
    romfsExit();
    return 0;
}