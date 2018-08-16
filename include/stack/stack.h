#pragma once
#include <iostream>

#include "../main.h"
#include "../console.h"

extern Console console;
extern device_aspect DEVICE;

struct stack {
    tinyxml2::XMLDocument *xmlParser;
    std::string source;
    std::string path;
};

namespace browser {
    class STACK {
        private:
            tinyxml2::XMLDocument *doc;
            std::string source = "";
            std::string path = "";

        public:
            STACK() {
                #ifdef __SWITCH__
                    romfsInit();
                #endif

                std::ifstream ifs;
    
                #ifdef __SWITCH__
                    ifs = std::ifstream("romfs:/pages/test.html");
                #else
                    ifs = std::ifstream("../../resources/pages/test.html");
                #endif

                if(this->source.empty())
                    this->source = browser::validator::validate_and_fix(std::string(std::istreambuf_iterator<char>{ifs}, {}));
                this->doc = new tinyxml2::XMLDocument();

                if(this->source.empty())
                    this->source = "<html><head>404</head><body><h1>Error</h1><p>We couldn't find the requested file :(</p></body></html>"; // TODO: Embedd error page
            }
            ~STACK() {
                #ifdef __SWITCH__
                    romfsExit();
                #endif
            }

            stack getCurrentPage() {
                stack Stack = {this->doc, this->source, this->path};
                return Stack;
            }

            void setSource (std::string page, bool file = false) {
                if (!file) {
                    this->source = browser::validator::validate_and_fix(std::string(page));
                    return;
                }

                console.printf(std::string("STACK->Loading file: " + page));
                
                std::ifstream ifs = std::ifstream(page);
                this->source = browser::validator::validate_and_fix(std::string(std::istreambuf_iterator<char>{ifs}, {}));
                this->path = "file://" + page;
            }

            void prepareTick() { }
            bool doTick() {
                this->doc->Parse((const char*)this->source.c_str(), this->source.size());
            }
    };
}