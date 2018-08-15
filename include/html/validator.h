#pragma once

#include <iostream>

#include "../main.h"
#include "../console.h"
#include "../tinyxml2.h"

extern Console console;

namespace browser {
    namespace validator {
        std::string validate_and_fix (std::string page_source, int tick = 0) {
            console.printf("DOM->Validator->Trying to validate & fix the document...");

            std::string new_page_source = page_source;

            // TODO: unclosed tags
            // TODO: invalid tags
            for (size_t i = 0; i < new_page_source.size(); i++) {
                if (new_page_source[i] == '<'  && new_page_source[i+1] == 'b' &&
                    new_page_source[i+2] == 'r' && new_page_source[i+3] == '>') {
                    new_page_source.insert(i+3, 1, '/');
                    i--;
                } else if (new_page_source[i] == '<'  && new_page_source[i+1] == 'h' &&
                    new_page_source[i+2] == 'r' && new_page_source[i+3] == '>') {
                    new_page_source.insert(i+3, 1, '/');
                    i--;
                }

            }

            tinyxml2::XMLDocument doc;
            doc.Parse((const char*)new_page_source.c_str(), new_page_source.size());

            // Recursivly fix till there aren't any errors
            if(doc.ErrorID()) {
                tick += 1;

                // Only try to fix the document 20 times
                if (tick > 20) {
                    console.printf("DOM->Validator->Failed to fix the document. Error ID:" + std::to_string(doc.ErrorID()));
                    return page_source;
                }

                new_page_source = validate_and_fix(new_page_source, tick);
            } else {
                console.printf("DOM->Validator->Fixed the document!");
            }

            return new_page_source;
        }
    }
}