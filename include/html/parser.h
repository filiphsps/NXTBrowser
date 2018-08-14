#pragma once
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#include <iostream>
#include <vector>

#include "../main.h"
#include "../console.h"

#include "html/elements.h"

static int browser_height = 720;
extern SDL_Surface *_browser_surface;
extern SDL_Rect _browser_position;
extern SDL_Rect _scroll_position;

extern Console console;
extern std::string console_output;

struct font {
    TTF_Font* font;
    std::string fontPath;
    int fontSize;
};

static std::vector<font> fontCache;

namespace browser {
    // TODO move this to /tags & then rework it
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

    namespace utils {
        TTF_Font* get_font_from_cache(std::string path, int size) {
            for (int i = 0; i < (int)fontCache.size(); i++) {
                if (fontCache[i].fontSize == size && fontCache[i].fontPath == path) {
                    console.printf("DOM->used a cached font...");
                    return fontCache[i].font;
                }
            }

            TTF_Font *font = TTF_OpenFont(path.c_str(), size);
            fontCache.push_back({ font, path, size });
            return font;
        }
    }

    namespace parser {
        // TODO: Remove duplicate code ("position +=", "TTF_Font *font", etc)

        // FIXME: create new rendering system where every element is its own surface
        // that way we only need to re-draw whatever changes
        int html_parser (const tinyxml2::XMLElement* child, std::string type, int position, element_data* elementData) {
            // H tags
            if (type.length() == 2 && type.at(0) == 'h') {
                // Dynamically generate h1-h6
                if (std::isdigit(type.at(1))) {
                    std::string tag = type;
                    std::string text = child->GetText();
                    tag.erase(0, 1);

                    int h_type = std::stoi(tag); // h[n] where n = h_type
                    int diff = 0;

                    if (h_type != 1)
                        diff = (h_type * 2);

                    int padding = 15 - diff;
                    int font_size = 32 - diff;

                    if (h_type >= 7)
                        return position; // H6 is the largest heading

                    TTF_Font *font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", font_size);
                    int text_w, text_h;
                    TTF_SizeText(font, text.c_str(), &text_w, &text_h);

                    position += padding + 5;

                    SDL_Rect size;

                    #ifdef DEBUG_DRAW_DOM
                        size = sdl_helper::drawText(_browser_surface, 0, position, text, font, elementData->center);
                        text_h = size.h;

                        sdl_helper::drawRect(_browser_surface, 0, position - padding, text_w, text_h + 5 + (padding*2), position, 55, 255, 255);
                        sdl_helper::drawText(_browser_surface, 0, position, text, font, elementData->center);
                    #else
                        size = sdl_helper::drawText(_browser_surface, 0, position, text, font, elementData->center);
                        text_h = size.h;
                    #endif

                    position += text_h + padding;
                }
            } else if (type == "p") {
                std::string text = child->GetText();
                TTF_Font *font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", 16);

                browser::elements::P *element = new browser::elements::P((browser::elements::properties*)nullptr, text);
                element->SetFont(font);
                browser::elements::renderQueueItem renderItem = element->getRenderQueueItem();

                #ifdef DEBUG_DRAW_DOM
                    sdl_helper::renderBackground(_browser_surface, {
                        renderItem.properties->margin.left, //x
                        position + renderItem.properties->margin.top, //y
                        renderItem.properties->width + renderItem.properties->padding.left + renderItem.properties->padding.right, //w
                        renderItem.properties->height + renderItem.properties->padding.top + renderItem.properties->padding.bottom //h
                    }, {(unsigned char)position, 55, 255, 255});
                #endif

                position += renderItem.properties->margin.top;
                sdl_helper::renderText(text, _browser_surface,
                    {renderItem.properties->margin.left + renderItem.properties->padding.left,
                    position, 0, 0}, renderItem.properties->width, font, {0, 0, 0, 255});
                position += renderItem.properties->height;
                position += renderItem.properties->margin.bottom;

            } else if (type == "br") {
                browser::elements::Br *element = new browser::elements::Br((browser::elements::properties*)nullptr);
                browser::elements::renderQueueItem renderItem = element->getRenderQueueItem();

                position += renderItem.size.height;
            } else {
                console.printf("DOM->Unsupported Tag: " + type);
            }

            auto element = child;

            for(const tinyxml2::XMLElement* c = element->FirstChildElement(); c!=0; c=c->NextSiblingElement()) {
                std::string type = c->Value();
                position = browser::parser::html_parser(c, type, position, elementData);
            }
            return position;
        }
    }
}