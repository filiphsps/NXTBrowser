#pragma once
#include <iostream>
#ifdef __SWITCH__
    #include <switch.h>
#endif

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

            void prepareTick(SDL_Event *events) {
                #ifdef __SWITCH__
                    hidScanInput();
                #else
                    this->Events = events;
                #endif
            }

            bool doTick(browser::STACK *STACK, browser::DOM *DOM, browser::GUI *GUI) {
                bool go = false;

                if (GUI->State.action == "SELECTED_ADDRESSBAR") {
                    if(this->Events->type == SDL_KEYDOWN || this->Events->type == SDL_TEXTINPUT) {
                        std::string path = STACK->getCurrentPage().path;

                        if (this->Events->key.keysym.sym == SDLK_BACKSPACE && path.length() > 0)
                            path.pop_back();

                        if (this->Events->type == SDL_TEXTINPUT)
                            if( !( ( this->Events->text.text[ 0 ] == 'c' || this->Events->text.text[ 0 ] == 'C' ) &&
                                ( this->Events->text.text[ 0 ] == 'v' || this->Events->text.text[ 0 ] == 'V' )
                                && SDL_GetModState() & KMOD_CTRL ) ) {
                                path += this->Events->text.text;
                            }
                        
                        if (this->Events->key.keysym.sym == SDLK_RETURN)
                            go = true;

                        
                        STACK->setPath(path, go);
                    }
                }

                #ifdef __SWITCH__
                    u64 kDown   = hidKeysDown(CONTROLLER_P1_AUTO);
                    u64 kHeld   = hidKeysHeld(CONTROLLER_P1_AUTO);
                    u64 kUp     = hidKeysUp(CONTROLLER_P1_AUTO);

                    if ((kDown & KEY_LSTICK) && (kDown & KEY_RSTICK)) console.toggleConsole();
                    if (kHeld & KEY_DUP)    GUI->scroll(0, 1);
                    if (kHeld & KEY_DDOWN)  GUI->scroll(0, -1);

                #else
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

                        /*case SDL_MOUSEWHEEL:
                            GUI->scroll(this->Events->wheel.x, this->Events->wheel.y);
                            break;*/
                    }
                #endif
            }
    };
}