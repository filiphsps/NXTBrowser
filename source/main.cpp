#include <string.h>
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

struct element_data {
    int height;
    int width;
    padding_data padding;
    padding_data margin;

    bool center = false;
    bool floatLeft;
    bool floatRight;
};

std::string console_output = "Hello World!\n";
std::string page =
"<html> \
    <head><title>Test page</title></head> \
    <body> \
        <h1>h1</h1> \
        <h2>h2</h2> \
        <h3>h3</h3> \
        <br /> \
        <p>string</p> \
        <center> \
            <h1>title in center</h1> \
            <p>string in center</p> \
        </center> \
    </body> \
</html>";

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
            sdl_helper::drawText(15, position, child->GetText(), font, elementData->center);
            TTF_CloseFont(font);

            position += font_size + padding;
        }
    } else if (type == "p") {
        position += 5;

        TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", 16);
        sdl_helper::drawText(15, position, child->GetText(), font, elementData->center);
        TTF_CloseFont(font);

        position += 16 + 5;
    } else if (type == "br") {
        position += 15;
    } else if (type == "center") {
        auto element = child;
        for(const tinyxml2::XMLElement* c = element->FirstChildElement(); c!=0; c=c->NextSiblingElement()) {
            std::string type = c->Value(); // TODO recursive

            // TODO: set center to true in elementData
            elementData->center = true;
            position = html_parser(c, type, position, elementData);
        }
    } else {
        console_output.append("unsupported tag '");
        console_output.append(type);
        console_output.append("'\n");
    }

    return position;
}

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
        std::string type = child->Value(); // TODO recursive

        element_data* elementData;
        position = html_parser(child, type, position, elementData);
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

    bool DOM_UPDATE = true;
    while(appletMainLoop()) {
        // FIXME: only render on change

        if (DOM_UPDATE) {
            SDL_RenderClear(_renderer);

            sdl_helper::drawTexture(_surface, _background_texture, 0, 0);

            hidScanInput();
            u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

            if (kDown & KEY_PLUS)
                break;

            // TODO: Draw browser page
            dom_parser(page);

            // Draw title
            sdl_helper::drawText(25, 27, doc.FirstChildElement("html")->FirstChildElement("head")->FirstChildElement("title")->GetText(), font, false);

            // Console log
            sdl_helper::drawText(5, 0, console_output, font, false);

            SDL_RenderPresent(_renderer);

            DOM_UPDATE = false;
        }
    }
    
    sdl_helper::exit();
    romfsExit();
    return 0;
}