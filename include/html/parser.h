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
    namespace utils {
        TTF_Font* get_font_from_cache(std::string path, int size) {
            // cache at most 5 fonts (need to figure out a good limit)
            /*if ((int)fontCache.size() > 5) {
                TTF_CloseFont(fontCache.begin()->font);
                fontCache.erase(fontCache.begin());
            }*/

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
        // FIXME: create new rendering system where every element is its own surface
        // that way we only need to re-draw whatever changes
        int html_parser (const tinyxml2::XMLElement* child, std::string type, int position) {
            browser::elements::GenericElement *tag;
            browser::elements::renderQueueItem renderItem;

            std::string text = "";

            if (type == "h1") {
                text = child->GetText();
                tag = new browser::elements::H1(text);
            } else if (type == "h2") {
                text = child->GetText();
                tag = new browser::elements::H2(text);
            } else if (type == "h3") {
                text = child->GetText();
                tag = new browser::elements::H3(text);
            } else if (type == "h4") {
                text = child->GetText();
                tag = new browser::elements::H4(text);
            } else if (type == "h5") {
                text = child->GetText();
                tag = new browser::elements::H5(text);
            } else if (type == "h6") {
                text = child->GetText();
                tag = new browser::elements::H6(text);
            } else if (type == "p") {
                text = child->GetText();
                tag = new browser::elements::P(text);
            } else if ((type == "b") || (type == "strong")) {
                text = child->GetText();
                tag = new browser::elements::B(text);
            } else if (type == "br") {
                tag = new browser::elements::Br();
            } else {
                tag = new browser::elements::GenericElement();
                //console.printf("DOM->Parser->Unsupported Tag: " + type);
            }
            
            TTF_Font *fontFamily;
            switch (tag->elementType) {
                case browser::elements::elementTypes::Text:
                    fontFamily = browser::utils::get_font_from_cache("../../romFS/fonts/NintendoStandard.ttf", (int)tag->properties.fontSize);
                    tag->SetFont(fontFamily);
                    renderItem = tag->getRenderQueueItem();

                    #ifdef DEBUG_DRAW_DOM
                        sdl_helper::renderBackground(_browser_surface, {
                            renderItem.properties.margin.left, //x
                            position + renderItem.properties.margin.top, //y
                            renderItem.properties.width + renderItem.properties.padding.left + renderItem.properties.padding.right, //w
                            renderItem.properties.height + renderItem.properties.padding.top + renderItem.properties.padding.bottom //h
                        }, {(unsigned char)position, 55, 255, 255});
                    #endif

                    position += renderItem.properties.margin.top;
                    sdl_helper::renderText(text, _browser_surface,
                        {renderItem.properties.margin.left + renderItem.properties.padding.left,
                        position, 0, 0}, renderItem.properties.width, fontFamily, {0, 0, 0, 255});
                    position += renderItem.properties.height;
                    position += renderItem.properties.margin.bottom;
                    break;
                case browser::elements::elementTypes::Container:
                    renderItem = tag->getRenderQueueItem();
                    position += renderItem.size.height;
                    break;
                default:
                    // TODO:
                    break;
            }

            //delete tag, renderItem;
            for(const tinyxml2::XMLElement* c = child->FirstChildElement(); c != NULL; c = c->NextSiblingElement()) {
                std::string type = c->Value();
                position = browser::parser::html_parser(c, type, position);
            }

            return position;
        }
    }
}