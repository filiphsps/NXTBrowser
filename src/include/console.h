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
        
        Console() {
            // TODO: log to file
        }
        ~Console() {
            // FIXME: close log file
        }

        int printf (std::string message) {
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

        std::string getFormattedOutput () {
            std::string console_output = "";

            for (int i = 0; i < (int)this->consoleLog.size(); i++) {
                if (this->consoleLog[i].num > 0)
                    console_output.append("(" + std::to_string(this->consoleLog[i].num) + ") ");

                console_output.append(this->consoleLog[i].message);
                console_output.append("\n");
            }

            return console_output;
        }

        void toggleConsole () {
            this->hidden = !this->hidden;
        }
};