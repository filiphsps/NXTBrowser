#include <iostream>
#include "dom/dom.h"

#include "../gui/gui.h"
#include "../html/parser.h"
#include "../stack/stack.h"

namespace browser {
    bool DOM::doTick(browser::STACK *STACK, browser::GUI *GUI) {
        try {
            //if (GUI->State.action == "SELECTED_ADDRESSBAR") 
            //    return false;

            if(!this->SHOULD_UPDATE) //TODO
                return false;
            
            printf("DOM->Started drawing the DOM...\n");
            
            if (browser_height < DEVICE.h) {
                browser_height = DEVICE.h  - SCROLLBAR_WIDTH;
            }

            int position = 0; // TODO: good scroll
            auto doc = STACK->getCurrentPage().xmlParser;

            #ifndef ___NATIVE_GUI___
                if(GUI->_browser_surface != NULL)
                    SDL_FreeSurface(GUI->_browser_surface);
                GUI->_browser_surface = SDL_CreateRGBSurface(0, DEVICE.w, browser_height, 32, 0, 0, 0, 255);

                SDL_Surface* browser = SDL_CreateRGBSurface(0, DEVICE.w, browser_height, 32, 0, 0, 0, 0);
                SDL_FillRect(browser, NULL, SDL_MapRGBA(browser->format, 255, 255, 255, 255));
                SDL_BlitSurface(browser, NULL, GUI->_browser_surface, NULL);
                SDL_FreeSurface(browser);
            #endif
            
            printf("DOM->Draw->Parsing Document...\n");
            auto element = doc->FirstChildElement("html")->FirstChildElement("body");
            for(const tinyxml2::XMLElement* child = element->FirstChildElement(); child != NULL; child=child->NextSiblingElement()) {
                // TODO: Do this properly
                std::string type = child->Value();
                position = browser::parser::html_parser(child, type, position, GUI->_browser_surface);
            }
            printf("DOM->Draw->Done Parsing Document\n");

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

            printf("DOM->Successfully drew the DOM\n");
        } catch(...) {
            printf("DOM->Failed to draw DOM\n");
        }
        this->SHOULD_UPDATE = false;

        return true;
    }
}