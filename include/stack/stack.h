#pragma once
#include <iostream>

#include "../main.h"
#include "../console.h"

extern Console console;
extern SDL_Rect DEVICE;

struct stack {
    tinyxml2::XMLDocument *xmlParser;
    std::string source;
};

namespace browser {
    class STACK {
        private:
            tinyxml2::XMLDocument *doc;
            std::string source;
        public:
            STACK() {
                #ifdef __SWITCH__
                    romfsInit();
                #endif

                std::ifstream ifs;
    
                #ifdef __SWITCH__
                    TTF_Font *font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", 14);
                    ifs = std::ifstream("romfs:/pages/test.html");
                #else
                    TTF_Font *font = browser::utils::get_font_from_cache("../../resources/fonts/NintendoStandard.ttf", 14);
                    ifs = std::ifstream("/Users/filiphsandstrom/Desktop/simpleweb/resources/pages/test.html");
                #endif

                this->source = browser::validator::validate_and_fix(std::string(std::istreambuf_iterator<char>{ifs}, {}));
                this->doc = new tinyxml2::XMLDocument();
            }
            ~STACK() {
                #ifdef __SWITCH__
                    romfsExit();
                #endif
            }

            stack getCurrentPage() {
                stack Stack = {this->doc, this->source};
                return Stack;
            }

            void prepareTick() { }
            bool doTick() {
                this->doc->Parse((const char*)this->source.c_str(), this->source.size());
            }
    };
}