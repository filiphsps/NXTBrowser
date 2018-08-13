#pragma once

#include "../main.h"

static int browser_height = 720;
static SDL_Surface *_browser_surface;
static SDL_Rect _browser_position = { 0, 68, 1280, 720 };
static SDL_Rect _scroll_position = { 0, 0, 1280, browser_height };

namespace browser {
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

    namespace parser {
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

                    TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", font_size);
                    int text_w, text_h;
                    TTF_SizeText(font, child->GetText(), &text_w, &text_h);

                    position += padding;

                    sdl_helper::drawText(_browser_surface, 0, position, child->GetText(), font, elementData->center);
                    TTF_CloseFont(font);

                    position += text_h + padding;
                }
            } else if (type == "p") {
                TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", 16);
                int text_w, text_h;
                TTF_SizeText(font, child->GetText(), &text_w, &text_h);

                position += 5;

                sdl_helper::drawText(_browser_surface, 0, position, child->GetText(), font, elementData->center);
                TTF_CloseFont(font);

                position += text_h + 5;
            } else if (type == "a") {
                // FIXME: a tag
                position += 5;

                TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", 16);
                int text_w, text_h;
                TTF_SizeText(font, child->GetText(), &text_w, &text_h);

                sdl_helper::drawText(_browser_surface, 0, position, child->GetText(), font, elementData->center, 0, 0, 255, 255);
                TTF_CloseFont(font);

                position += text_h + 5;

            } else if (type == "code") {
                // FIXME: code tag
                position += 5;

                TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", 16);
                elementData->background = {
                    r: 155,
                    g: 155,
                    b: 155
                };

                int text_w, text_h;
                TTF_SizeText(font, child->GetText(), &text_w, &text_h);

                sdl_helper::drawRect(_browser_surface, 15, position - 5, text_w, text_h + 10, elementData->background.r, elementData->background.g, elementData->background.b, elementData->background.a);
                sdl_helper::drawText(_browser_surface, 15, position, child->GetText(), font, elementData->center);
                TTF_CloseFont(font);

                position += 16 + 5;
            } else if (type == "button") {
                // FIXME: code tag
                position += 5;

                TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", 16);
                elementData->background = {
                    r: 155,
                    g: 155,
                    b: 155
                };

                int text_w, text_h;
                TTF_SizeText(font, child->GetText(), &text_w, &text_h);

                sdl_helper::drawRect(_browser_surface, 0, position - 5, text_w + 20, text_h + 10, elementData->background.r, elementData->background.g, elementData->background.b, elementData->background.a);
                sdl_helper::drawText(_browser_surface, 0 + 10, position, child->GetText(), font, elementData->center);
                TTF_CloseFont(font);

                position += 16 + 5;
            } else if (type == "br") {
                position += 15;
            } else if (type == "center") {
                elementData->center = true;
            } else if (type == "div") {
                // TODO: handle parent tags
            } else if (type == "header") {
                // TODO: handle parent tags
            } else if (type == "ul") {

            } else if (type == "li") {
                TTF_Font *font = TTF_OpenFont("romfs:/fonts/NintendoStandard.ttf", 16);
                int text_w, text_h;
                TTF_SizeText(font, child->GetText(), &text_w, &text_h);

                position += 5;

                sdl_helper::drawText(_browser_surface, 0, position, child->GetText(), font, elementData->center);
                TTF_CloseFont(font);

                position += text_h + 5;
            } else {
                /*console_output.append("unsupported tag '");
                console_output.append(type);
                console_output.append("'\n");*/
            }

            auto element = child;
            for(const tinyxml2::XMLElement* c = element->FirstChildElement(); c!=0; c=c->NextSiblingElement()) {
                std::string type = c->Value();
                position = html_parser(c, type, position, elementData);
            }
            return position;
        }
    }
}