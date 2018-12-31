#include "gui/gui.h"

namespace browser {
    GUI::GUI() {
        #ifndef ___NATIVE_GUI___
            // sdl_helper::init();
        #else
            uiInitOptions options;
            memset(&options, 0, sizeof (uiInitOptions));
            uiInit(&options);
        #endif
    }
    GUI::~GUI() {
        #ifndef ___NATIVE_GUI___
            // sdl_helper::exit();
        #else
            uiUninit();
        #endif
    }

    void GUI::prepareTick() { }

    bool GUI::doTick() {
        return true;
    }
}