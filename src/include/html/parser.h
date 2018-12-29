// TODO:
// --------------------------------------------------------------------------
// 
//                  New branch-based rendering/parse system
//
// --------------------------------------------------------------------------
// Every branch should be able to live in its own thread spawning another
// branch thread for every child
//
// body -> header -> h1
//      |         └> h2 -> span -> i
//      |
//      ├> main -> ...
//      |       |
//      |       ├> ...
//      |       |
//      |       └> ...
//      |
//      └> footer
//
// --------------------------------------------------------------------------
// Then we keep track of the relative index where n is the relative
// tree/branch index:
//    
// header       = 0:0
//   h1         = 0:0
//   h2         = 0:1
//     span     = 0:1:0
//       i      = 0:1:0:0
// main         = 1:0
// footer       = 2:0
//
// Each branch should be rendered into it's own surface.
// Which would give use the ability to only update the branch that changed.
// In this example that would be "h2", "main" and "footer"
//
// It would oossible even re-use surfaces that are identical, provided
// ofcourse that it saves more performance than it takes to compare
// each branch with eachother.
//
// --------------------------------------------------------------------------
//

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
        int html_parser (const tinyxml2::XMLElement* child, std::string type, int position, SDL_Surface* _browser_surface) {
            browser::elements::GenericElement *tag;
            browser::elements::renderQueueItem renderItem;

            std::string text = "";
            if (child && child->GetText())
                text = child->GetText();

            if (type == "h1") {
                tag = new browser::elements::H1(text);
            } else if (type == "h2") {
                tag = new browser::elements::H2(text);
            } else if (type == "h3") {
                tag = new browser::elements::H3(text);
            } else if (type == "h4") {
                tag = new browser::elements::H4(text);
            } else if (type == "h5") {
                tag = new browser::elements::H5(text);
            } else if (type == "h6") {
                tag = new browser::elements::H6(text);
            }
            
            else if (type == "p") {
                tag = new browser::elements::P(text);
            } else if (type == "font") {
                tag = new browser::elements::P(text);
            } else if (type == "aside") {
                tag = new browser::elements::P(text);
            } else if (type == "pre") {
                tag = new browser::elements::P(text);
            }
            
            else if (type == "b") {
                tag = new browser::elements::B(text);
            } else if (type == "strong") {
                tag = new browser::elements::Strong(text);
            }

            else if (type == "li") {
                text = std::string("• ") + text; // FIXME: let tag handle list icon
                tag = new browser::elements::Li(text);
            }

            else if (type == "a") {
                tag = new browser::elements::A(text);
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