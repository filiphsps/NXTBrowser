#pragma once
#include <iostream>

#include "../main.h"
#ifdef ___NATIVE_GUI___
    #include "ui/mainWindow.h"
#endif

namespace browser {
    class GUI {
        public:
            GUI();
            ~GUI();

            void prepareTick();
            bool doTick();
    };
}