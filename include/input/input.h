#pragma once
#include <iostream>

#include "../main.h"
#include "../console.h"

#include "../gui/gui.h"
#include "../stack/stack.h"
#include "../dom/dom.h"

extern Console console;
extern device_aspect DEVICE;

namespace browser {
    class INPUT {
        private:
            SDL_Event *Events;

        public:
            INPUT() { }
            ~INPUT() { }

            void prepareTick(SDL_Event *events) { this->Events = events; }

            bool doTick(browser::STACK *STACK, browser::DOM *DOM, browser::GUI *GUI) {
                switch(this->Events->type) {
                    case SDL_KEYDOWN:
                        switch(this->Events->key.keysym.sym){
                            case SDLK_F12:
                                console.toggleConsole();
                                break;
                            case SDLK_UP:
                                GUI->scroll(0, 1);
                                break;
                            case SDLK_DOWN:
                                GUI->scroll(0, -1);
                                break;
                        }
                        break;

                    case SDL_MOUSEWHEEL:
                        GUI->scroll(this->Events->wheel.x, this->Events->wheel.y);
                        break;
                }
            }
    };
}