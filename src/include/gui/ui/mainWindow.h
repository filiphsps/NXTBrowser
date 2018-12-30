#pragma once
#include <stdio.h>

#include <libui/ui.h>

class MainWindow {
    public:
        MainWindow();
        ~MainWindow();
    private:
        uiWindow *window;

        uiEntry *address_bar_entry;
        uiForm *address_bar_group;
};