#include <iostream>
#include <vector>

#include "console.h"

Console::Console() {
    // TODO: log to file
}
Console::~Console() {
    // FIXME: close log file
}

int Console::printf (std::string message) {
    #ifndef __SWITCH__
        std::cout << message << std::endl;
        return 0;
    #endif

    if(DEVICE.h <= 0)
        return -1;
    
    if ((int)this->consoleLog.size() > ((DEVICE.h-30)/20*DEVICE.scaling))
        this->consoleLog.erase(this->consoleLog.begin());

    // Group duplicated messages
    for (int i = 0; i < (int)this->consoleLog.size(); i++) {
        if (this->consoleLog[(int)this->consoleLog.size()-1].message == message) {
            this->consoleLog[(int)this->consoleLog.size()-1].num += 1;
            return 0;
        }
    }

    this->consoleLog.push_back({ message, 0 });
    return 0;
}

std::string Console::getFormattedOutput () {
    std::string console_output = "";

    for (int i = 0; i < (int)this->consoleLog.size(); i++) {
        if (this->consoleLog[i].num > 0)
            console_output.append("(" + std::to_string(this->consoleLog[i].num) + ") ");

        console_output.append(this->consoleLog[i].message);
        console_output.append("\n");
    }

    return console_output;
}

void Console::toggleConsole () {
    this->hidden = !this->hidden;
}