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

    double loaded = 75;
    bool go = false;
};

namespace browser {
    class STACK {
        private:
            stack Stack;

        public:
            STACK() {
                this->Stack.source = "";
                this->Stack.path = "http://www.motherfuckingwebsite.com";
                this->Stack.loaded = 0;
                this->Stack.go = true;

                #ifdef __SWITCH__
                    romfsInit();
                #endif

                this->Stack.xmlParser = new tinyxml2::XMLDocument();
            }
            ~STACK() {
                #ifdef __SWITCH__
                    romfsExit();
                #endif
            }

            stack getCurrentPage() {
                return this->Stack;
            }

            void setPath (std::string path, bool go) {
                this->Stack.path = path;

                if (go) {
                    this->Stack.go = true;
                    this->Stack.loaded = 0;
                }
            }
            void setSource (std::string page, bool file = false) {
                this->Stack.go = false;
                
                if (file) {
                    console.printf(std::string("STACK->Loading file: " + page));
                    //std::ifstream ifs = std::ifstream(page);
                    //this->Stack.source = browser::validator::validate_and_fix(std::string(std::istreambuf_iterator<char>{ifs}, {}));
                    this->Stack.path = "file://" + page;
                } else {
                    this->Stack.source = browser::validator::validate_and_fix(std::string(page, {}));
                    
                }

            }

            void prepareTick() {
                if(this->Stack.source.empty())
                    this->Stack.source = "<html><head>404</head><body><h1>Error</h1><p>We couldn't find the requested file :(</p></body></html>"; // TODO: Embedd error page
            }
            bool doTick() {
                this->Stack.xmlParser->Parse((const char*)this->Stack.source.c_str(), this->Stack.source.size());
            }
    };
}