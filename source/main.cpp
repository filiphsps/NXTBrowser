#include <string.h>
#include <fstream>
#include <stdio.h>
#include <iostream>

#ifdef __SWITCH__
    #include <switch.h>
#elif __MACOS__
    #include <sys/time.h>
    #include <sys/resource.h>
#elif __WIN32__
#endif

#include "main.h"
#include "html/html.h"
#include "gui/gui.h"
#include "stack/stack.h"
#include "dom/dom.h"
#include "console.h"

Console console;
SDL_Rect DEVICE = {0, 0, 0, 0};

long getMemoryUsage() {
    #ifdef __MACOS__
        struct rusage usage;
        if(0 == getrusage(RUSAGE_SELF, &usage))
            return usage.ru_maxrss/1000/1000;
    #endif

    return 0;
}

int main(int argc, char **argv) {
    //browser::INPUT *INPUT = new browser::INPUT();
    //browser::NET *NET = new browser::NET();
    browser::STACK *STACK = new browser::STACK();
    browser::DOM *DOM = new browser::DOM();
    browser::GUI *GUI = new browser::GUI();

    unsigned int currentTick = 0, lastTick = 0, delta = 0;
    #ifdef __SWITCH__
    while(appletMainLoop()) {
    #else
    while(true) {
    #endif
        currentTick = SDL_GetTicks();
        delta = currentTick - lastTick;

        //INPUT->prepareTick();
        //NET->prepareTick();
        STACK->prepareTick();
        DOM->prepareTick();
        GUI->prepareTick();

        GUI->renderWindow();

        browser::UIElements::AddressBar::Render(GUI);
        browser::UIElements::Console::Render(GUI);
        browser::UIElements::Console::RenderStat(GUI, 1,
            std::string("FPS: " + std::to_string(1000 / delta)));
        browser::UIElements::Console::RenderStat(GUI, 2,
            std::string("Memory: " + std::to_string(
                getMemoryUsage()) + "/" +
                std::to_string(SDL_GetSystemRAM())
                + " MB"));

        //INPUT->doTick();
        //NET->doTick();
        STACK->doTick();
        DOM->doTick(STACK, GUI);
        GUI->doTick();

        lastTick = currentTick;
    }

    delete GUI, DOM, STACK/*, NET, INPUT*/;
    return 0;
}