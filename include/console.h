#pragma once

#define MAX_CONSOLE_OUTPUT 35

#include <iostream>
#include <vector>

struct console_log {
    std::string message;
    int num;
};

class Console {
    private:
        std::vector<console_log> consoleLog;
    public:
        Console() {

        }

        int printf (std::string message) {
            if ((int)this->consoleLog.size() > MAX_CONSOLE_OUTPUT)
                this->consoleLog.erase(this->consoleLog.begin());

            // Group duplicated messages
            //for (int i = 0; i < (int)this->consoleLog.size(); i++) {
                if (this->consoleLog[(int)this->consoleLog.size()-1].message == message) {
                    this->consoleLog[(int)this->consoleLog.size()-1].num += 1;
                    return 0;
                }
            //}

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
};