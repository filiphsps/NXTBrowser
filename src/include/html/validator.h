#pragma once

#include <iostream>
#include <tidy.h>
#include <tidybuffio.h>

#include "../main.h"
#include "../console.h"
#include "../tinyxml2.h"

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
                tidyOptSetBool(tdoc, TidyXhtmlOut, yes);

                int rc = tidySetErrorBuffer(tdoc, &errbuf);
                rc = tidyParseString(tdoc, (const char*)page_source.c_str());
                rc = tidyCleanAndRepair(tdoc);
                //rc = tidyRunDiagnostics(tdoc);
                rc = (tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1);
                rc = tidySaveString(tdoc, output, &size);
                free(output);
                output = (char *)malloc(size+1);
                rc = tidySaveString(tdoc, output, &size);
                tidyRelease(tdoc);

                new_page_source = output;

                for (size_t i = 0; i < new_page_source.size(); i++) {
                    if (new_page_source[i] == ' '  && new_page_source[i+1] == '/' && new_page_source[i+2] == '>') {
                        new_page_source.erase(i, 1);
                        if (i >= 1)
                            i--;
                    }
                    if (new_page_source[i] == '\n') {
                        new_page_source.erase(i, 1);
                        if (i >= 1)
                            i--;
                    }
                }
            } catch (int ex) {
                std::cout << "couldnt fix the document" << std::endl;
            }

            tinyxml2::XMLDocument *doc;
            try {
                doc = new tinyxml2::XMLDocument();
                doc->Parse((const char*)new_page_source.c_str(), new_page_source.size());
            } catch (int ex) {
                std::cout << doc->ErrorID() << std::endl;
                std::cout << doc->ErrorName() << std::endl;
            }

            std::cout <<  new_page_source << std::endl;
            return new_page_source;
        }
    }
}