#pragma once
#include <iostream>
#include <curl/curl.h>
#include <curl/easy.h>

#include "main.h"
#include "console.h"

#include "../stack/stack.h"
#include "../dom/dom.h"

extern Console console;
extern device_aspect DEVICE;

size_t write_data(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t newLength = size * nmemb;
    size_t oldLength = s->size();
    s->resize(oldLength + newLength);

    std::copy((char*)contents, (char*)contents+newLength, s->begin() + oldLength);
}

namespace browser {
    class NET {

        public:
            NET() {};
            ~NET() {};

            void prepareTick() {};

            bool doTick(browser::STACK *STACK, browser::DOM *DOM) {
                auto curl = curl_easy_init();
                stack Stack = STACK->getCurrentPage();
                std::string source;

                if (!Stack.go)
                    return false;
                
                curl_easy_setopt(curl, CURLOPT_URL, Stack.path.c_str());
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &source);
                CURLcode res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                if (res == 23) {
                    STACK->setSource(source, false);
                    DOM->SHOULD_UPDATE = true;
                
                    //std::cout << res << std::endl;
                    //std::cout << source << std::endl;
                }
                return DOM->SHOULD_UPDATE;
            };
    };
}