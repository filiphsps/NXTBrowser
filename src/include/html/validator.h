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
                }

                if (new_page_source[i] == '<'  && new_page_source[i+1] == 'h' &&
                    new_page_source[i+2] == 'r' && new_page_source[i+3] == '>') {
                    new_page_source.insert(i+3, 1, '/');
                    i--;
                }
            }

            tinyxml2::XMLDocument *doc;
            try {
                doc = new tinyxml2::XMLDocument();
                doc->Parse((const char*)new_page_source.c_str(), new_page_source.size());
            } catch(int ex) {
                console.printf("DOM->Validator->tinyxml2 exception: " + std::to_string(ex));
                tick +=1;

                // Only try to fix the document 5 times
                if (tick > 5) {
                    console.printf("DOM->Validator->Failed to fix the document. Error ID:" + std::to_string(doc->ErrorID()));
                    return new_page_source;
                } else
                    return validate_and_fix(new_page_source, tick);
            }

            // Recursivly fix till there aren't any errors
            if(doc->ErrorID()) {
                tick += 1;

                if (tick > 5) {
                    console.printf("DOM->Validator->Failed to fix the document. Error ID:" + std::to_string(doc->ErrorID()));
                    console.printf(std::string(doc->ErrorName()));
                    return new_page_source;
                }

                delete doc;
                return validate_and_fix(new_page_source, tick);
            } else {
                console.printf("DOM->Validator->Fixed the document!");
                return new_page_source;
            }
        }
    }
}