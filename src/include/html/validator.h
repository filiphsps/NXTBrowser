#pragma once

#include <iostream>
#include <tidy.h>
#include <tidybuffio.h>

#include "../main.h"
#include "../console.h"
#include "../libs/tinyxml2/tinyxml2.h"

extern Console console;

namespace browser {
    namespace validator {
        std::string validate_and_fix (std::string page_source) {
            char *output = (char *)malloc(1);
            uint size = 0;
            TidyBuffer errbuf = {0};

            std::string new_page_source = page_source;

            try {
                TidyDoc tdoc = tidyCreate();
                tidyOptSetBool(tdoc, TidyShowInfo, yes);
                tidyOptSetBool(tdoc, TidyAsciiChars, yes); //FIXME: no
                tidyOptSetBool(tdoc, TidyEncloseBodyText, yes);
                tidyOptSetBool(tdoc, TidyEncloseBlockText, yes);
                tidyOptSetBool(tdoc, TidyMakeBare, yes);
                tidyOptSetBool(tdoc, TidyXhtmlOut, yes);

                int rc = tidySetErrorBuffer(tdoc, &errbuf);
                rc = tidyParseString(tdoc, (const char*)page_source.c_str());
                rc = tidyCleanAndRepair(tdoc);
                rc = tidyRunDiagnostics(tdoc);
                rc = (tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1);
                rc = tidySaveString(tdoc, output, &size);
                free(output);
                output = (char *)malloc(size);
                rc = tidySaveString(tdoc, output, &size);
                tidyRelease(tdoc);

                new_page_source = output;

                /*for (size_t i = 0; i < new_page_source.size(); i++) {
                    if (new_page_source[i] == ' '  && new_page_source[i+1] == '/' && new_page_source[i+2] == '>') {
                        new_page_source.erase(i, 1);
                        continue;
                    }
                    if (new_page_source[i] == '\n') {
                        new_page_source.erase(i, 1);
                        continue;
                    }
                }*/
            } catch (...) {
                console.printf("DOM->Validator->Tidy->Failed to fix the element");
            }

            tinyxml2::XMLDocument *doc;
            try {
                doc = new tinyxml2::XMLDocument();
                doc->Parse((const char*)new_page_source.c_str(), new_page_source.size());
            } catch (...) {
                console.printf("DOM->Validator->tinyxml2->ErrorName" + std::string(doc->ErrorName()));
                console.printf("DOM->Validator->tinyxml2->ErrorId" + std::to_string(doc->ErrorID()));
            }
            
            return new_page_source;
        }
    }
}