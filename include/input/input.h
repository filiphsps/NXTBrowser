#pragma once
#include <iostream>

#include "../main.h"
#include "../console.h"

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

            bool doTick() { }
    };
}