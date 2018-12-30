#pragma once
#include <iostream>
#include <curl/curl.h>
#include <curl/easy.h>

#include "main.h"
#include "console.h"

#include "../stack/stack.h"
#include "../dom/dom.h"

extern device_aspect DEVICE;

std::string data;
size_t write_data(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);

    data = ((std::string*)userp)->c_str();
    return size * nmemb;
}

namespace browser {
    class NET {
        public:
            NET() {};
            ~NET() {};

            void prepareTick() {}

            bool doTick(browser::STACK *STACK, browser::DOM *DOM) {
                auto curl = curl_easy_init();
                stack Stack = STACK->getCurrentPage();
                std::string source;

                if (!Stack.go)
                    return false;

                printf("NET->Downloading: "); printf(Stack.path.c_str()); printf("...\n");

                curl_easy_setopt(curl, CURLOPT_URL, Stack.path.c_str());
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &source);
                CURLcode res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                if (res == 23 || !data.empty()) {
                    STACK->setSource(data, false);
                    DOM->SHOULD_UPDATE = true;

                    printf("NET->Download Completed: "); printf(Stack.path.c_str()); printf("\n");
                } else {
                    printf("NET->Failed To Download: "); printf(Stack.path.c_str()); printf("\n");
                }
                return DOM->SHOULD_UPDATE;
            };
    };
}