#include <string.h>
#include <fstream>
#include <stdio.h>
#include <iostream>

#ifdef __SWITCH__
    #include <switch.h>
#endif

#include "main.h"
#include "html/html.h"
#include "gui/gui.h"
#include "stack/stack.h"
#include "dom/dom.h"
#include "console.h"

Console console;
SDL_Rect DEVICE = {0, 0, 0, 0};

int main(int argc, char **argv) {
    //browser::INPUT *INPUT = new browser::INPUT();
    //browser::NET *NET = new browser::NET();
    browser::STACK *STACK = new browser::STACK();
    browser::DOM *DOM = new browser::DOM();
    browser::GUI *GUI = new browser::GUI();


    #ifdef __SWITCH__
    while(appletMainLoop()) {
    #else
    while(true) {
    #endif
        //INPUT->prepareTick();
        //NET->prepareTick();
        STACK->prepareTick();
        DOM->prepareTick();
        GUI->prepareTick();

        GUI->renderWindow();

        browser::UIElements::AddressBar::Render(GUI);
        browser::UIElements::Console::Render(GUI);

        //INPUT->doTick();
        //NET->doTick();
        STACK->doTick();
        DOM->doTick(STACK, GUI);
        GUI->doTick();
    }
    return 0;
}