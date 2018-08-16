#pragma once
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#include <iostream>
#include <vector>

#include "../main.h"
#include "../console.h"
#include "utils.h"

#include "html/elements.h"

static int browser_height = 0;

extern Console console;
extern std::string console_output;

namespace browser {
    namespace parser {
        // FIXME: create new rendering system where every element is its own surface
        // that way we only need to re-draw whatever changes
        int html_parser (const tinyxml2::XMLElement* child, std::string type, int position, SDL_Surface* _browser_surface) {
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
            }
            
            else if (type == "p") {
                text = child->GetText();
                tag = new browser::elements::P(text);
            } else if (type == "aside") {
                text = child->GetText();
                tag = new browser::elements::P(text);
            }
            
            else if (type == "b") {
                text = child->GetText();
                tag = new browser::elements::Aside(text);
            } else if (type == "strong") {
                text = child->GetText();
                tag = new browser::elements::Strong(text);
            }

            else if (type == "li") {
                text = std::string("• ") + child->GetText(); // FIXME: let tag handle list icon
                tag = new browser::elements::Li(text);
            }
            
            else if (type == "br") {
                tag = new browser::elements::Br();
            }
            
            else {
                tag = new browser::elements::GenericElement();
                console.printf("DOM->Parser->Unsupported Tag: " + type);
            }
            
            switch (tag->elementType) {
                case browser::elements::elementTypes::Text:
                    renderItem = tag->getRenderQueueItem(_browser_surface);

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
                        position, 0, 0}, renderItem.properties.width, tag->getFont(), {0, 0, 0, 255});
                    position += renderItem.properties.height;
                    position += renderItem.properties.margin.bottom;
                    break;
                case browser::elements::elementTypes::Container:
                    renderItem = tag->getRenderQueueItem(_browser_surface);
                    position += renderItem.size.height;
                    break;
                default:
                    // TODO:
                    break;
            }

            delete tag, renderItem;
            for(const tinyxml2::XMLElement* c = child->FirstChildElement(); c != NULL; c = c->NextSiblingElement()) {
                std::string type = c->Value();
                position = browser::parser::html_parser(c, type, position, _browser_surface);
            }

            return position;
        }
    }
}