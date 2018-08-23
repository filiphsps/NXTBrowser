#pragma once
#include <iostream>

#include "../main.h"
#include "../console.h"

#include "../gui/gui.h"
#include "../stack/stack.h"

extern Console console;
extern device_aspect DEVICE;


namespace browser {
    class DOM {
        public:
            bool SHOULD_UPDATE = true;

            DOM() { }
            ~DOM() { }

            void prepareTick() { }

            bool doTick(browser::STACK *STACK, browser::GUI *GUI) {
                try {
                    //if (GUI->State.action == "SELECTED_ADDRESSBAR") 
                    //    return false;

                    if(!this->SHOULD_UPDATE) //TODO
                        return false;
                    
                    console.printf("DOM->Started drawing the DOM...");
                    
                    if (browser_height < DEVICE.h) {
                        browser_height = DEVICE.h  - SCROLLBAR_WIDTH;
                    }

                    int position = 0; // TODO: good scroll
                    auto doc = STACK->getCurrentPage().xmlParser;

                    if(GUI->_browser_surface != NULL)
                        SDL_FreeSurface(GUI->_browser_surface);
                    GUI->_browser_surface = SDL_CreateRGBSurface(0, DEVICE.w, browser_height, 32, 0, 0, 0, 255);

                    SDL_Surface* browser = SDL_CreateRGBSurface(0, DEVICE.w, browser_height, 32, 0, 0, 0, 0);
                    SDL_FillRect(browser, NULL, SDL_MapRGBA(browser->format, 255, 255, 255, 255));
                    SDL_BlitSurface(browser, NULL, GUI->_browser_surface, NULL);
                    SDL_FreeSurface(browser);
                    
                    console.printf("DOM->Draw->Parsing Document...");
                    auto element = doc->FirstChildElement("html")->FirstChildElement("body");
                    for(const tinyxml2::XMLElement* child = element->FirstChildElement(); child != NULL; child=child->NextSiblingElement()) {
                        // TODO: Do this properly
                        std::string type = child->Value();
                        position = browser::parser::html_parser(child, type, position, GUI->_browser_surface);
                    }
                    console.printf("DOM->Draw->Done Parsing Document");

                    if (position > browser_height) {
                        browser_height = position * DEVICE.scaling;
                        //_scroll_position.h = browser_height;

                        // Re-render since we failed due to height last time around.
                        this->SHOULD_UPDATE = true;
                        doTick(STACK, GUI);
                    } else if (position < DEVICE.h) {
                        browser_height = DEVICE.h;
                        // _scroll_position.h = browser_height;
                    }

                    console.printf("DOM->Successfully drew the DOM");
                } catch(...) {
                    console.printf("DOM->Failed to draw DOM");
                }
                this->SHOULD_UPDATE = false;
            }
    };
}