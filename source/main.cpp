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

std::string console_output = "Console output:\n";
std::string page ="<html> <head> <title> Test display of HTML elements </title> </head> <body> <h1>Testing display of HTML elements</h1> <h2>This is 2nd level heading</h2> <p>This is a test paragraph.</p> <h3>This is 3rd level heading</h3> <p>This is a test paragraph.</p> <h4>This is 4th level heading</h4> <p>This is a test paragraph.</p> <h5>This is 5th level heading</h5> <p>This is a test paragraph.</p> <h6>This is 6th level heading</h6> <p>This is a test paragraph.</p> <h2>Basic block level elements</h2> <p>This is a normal paragraph (<code>p</code> element). To add some length to it, let us mention that this page was primarily written for testing the effect of <strong>user style sheets</strong>. You can use it for various other purposes as well, like just checking how your browser displays various HTML elements by default. It can also be useful when testing conversions from HTML format to other formats, since some elements can go wrong then.</p> <p>This is another paragraph. I think it needs to be added that the set of elements tested is not exhaustive in any sense. I have selected those elements for which it can make sense to write user style sheet rules, in my opionion.</p> <div>This is a <code>div</code> element. Authors may use such elements instead of paragraph markup for various reasons. (End of <code>div</code>.)</div> <blockquote><p>This is a block quotation containing a single paragraph. Well, not quite, since this is not <em>really</em> quoted text, but I hope you understand the point. After all, this page does not use HTML markup very normally anyway.</p></blockquote> </body> </html>";

int html_parser (const tinyxml2::XMLElement* child, std::string type, int position, element_data* elementData) {
    if (position > 720-27)
        return position; //FIXME: Scroll

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