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
#include "input/input.h"
#include "stack/stack.h"
#include "dom/dom.h"
#include "console.h"

Console console;
device_aspect DEVICE;

long getMemoryUsage() {
    #ifdef __MACOS__
        struct rusage usage;
        if(0 == getrusage(RUSAGE_SELF, &usage))
            return usage.ru_maxrss/1000/1000;
    #endif

    return 0;
}

bool running = true;
int main(int argc, char* argv[]) {
    browser::INPUT *INPUT = new browser::INPUT();
    //browser::NET *NET = new browser::NET();
    browser::STACK *STACK = new browser::STACK();
    browser::DOM *DOM = new browser::DOM();
    browser::GUI *GUI = new browser::GUI();

    if (argc > 1)
        STACK->setSource(argv[1], true);

    SDL_Event events;
    unsigned int currentTick = 0, lastTick = 0, delta = 0, lastSize = DEVICE.h + DEVICE.w;
    #ifdef __SWITCH__
    while(appletMainLoop() && running) {
    #else
    while(running) {
    #endif
        currentTick = SDL_GetTicks();
        delta = currentTick - lastTick;

        if (SDL_WaitEvent(&events) != 0) {
            //TODO: move to input class
            switch (events.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
            
            INPUT->prepareTick(&events);
            //NET->prepareTick();
            STACK->prepareTick();
            DOM->prepareTick();
            GUI->prepareTick();

            browser::UIElements::AddressBar::Render(GUI, STACK);
            browser::UIElements::Console::Render(GUI);
            browser::UIElements::Console::RenderStat(GUI, 1,
                std::string("FPS: " + std::to_string(1000 / delta)));
            browser::UIElements::Console::RenderStat(GUI, 2,
                std::string("Memory: " + std::to_string(
                    getMemoryUsage()) + "/" +
                    std::to_string(SDL_GetSystemRAM())
                    + " MB"));
            browser::UIElements::Console::RenderStat(GUI, 3,
                std::string("Browser Aspect: " + std::to_string(GUI->_gui_surface->w)
                + "/" + std::to_string(GUI->_gui_surface->h) + ", Scaling: " + std::to_string(DEVICE.scaling)));

            if (lastSize != (DEVICE.h + DEVICE.w)) {
                lastSize = DEVICE.h + DEVICE.w;
                DOM->SHOULD_UPDATE = true;
            }

            INPUT->doTick(STACK, DOM, GUI);
            //NET->doTick();
            STACK->doTick();
            DOM->doTick(STACK, GUI);
            GUI->doTick();

            lastSize = DEVICE.h + DEVICE.w;
        }

        SDL_PumpEvents();
        lastTick = currentTick;
    }

    delete GUI, DOM, STACK/*, NET*/, INPUT;
    return 0;
}