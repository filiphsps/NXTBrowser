#pragma once

#include <iostream>
#include <vector>

struct device_aspect {
    short scaling = 1;
    int w = 0;
    int h = 0;
};

struct console_log {
    std::string message;
    int num;
};

extern device_aspect DEVICE;

class Console {
    private:
        std::vector<console_log> consoleLog;
    public:
        bool hidden = false;
        
        Console();
        ~Console();

        int printf (std::string message);

        std::string getFormattedOutput ();

        void toggleConsole ();
};