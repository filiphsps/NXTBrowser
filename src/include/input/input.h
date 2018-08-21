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
        public:
            INPUT() { }
            ~INPUT() { }

            void prepareTick() {
                #ifdef __SWITCH__
                    hidScanInput();
                #endif
            }

            bool doTick(browser::STACK *STACK, browser::DOM *DOM, browser::GUI *GUI) {
                #ifdef __SWITCH__
                    u64 kDown   = hidKeysDown(CONTROLLER_P1_AUTO);
                    u64 kHeld   = hidKeysHeld(CONTROLLER_P1_AUTO);
                    u64 kUp     = hidKeysUp(CONTROLLER_P1_AUTO);

                    if ((kDown & KEY_LSTICK) && (kDown & KEY_RSTICK)) console.toggleConsole();
                    if (kHeld & KEY_DUP)    GUI->scroll(0, 1);
                    if (kHeld & KEY_DDOWN)  GUI->scroll(0, -1);

                #else
                    const Uint8 *state = SDL_GetKeyboardState(NULL);
                    bool go = false;

                    if (state[SDL_SCANCODE_ESCAPE])
                        exit(0); //FIXME: BAD

                    if (state[SDL_SCANCODE_DOWN])
                        GUI->scroll(0, -1);
                    else if (state[SDL_SCANCODE_UP])
                        GUI->scroll(0, 1);

                    if (state[SDL_SCANCODE_F12])
                        console.toggleConsole();
                    
                    if (GUI->State.action == "SELECTED_ADDRESSBAR") {
                        std::string path = STACK->getCurrentPage().path;

                        if (state[SDL_SCANCODE_BACKSPACE] && path.length() > 0)
                            path.pop_back();
                        
                        if (state[SDL_SCANCODE_RETURN])
                            go = true;

                        
                        STACK->setPath(path, go);
                    }
                #endif
            }
    };
}