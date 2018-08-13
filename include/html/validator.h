#pragma once

#include <iostream>

#include "../tinyxml2.h"

namespace browser {
    std::string validate_and_fix (std::string page_source, int tick = 0) {
        std::string new_page_source = page_source;

        // TODO: unclosed tags
        // TODO: invalid tags
        //new_page_source.replace(new_page_source.find("<br>"), std::string("<br>").length(), "<br/>");
        //new_page_source.replace(new_page_source.find("<hr>"), std::string("<hr>").length(), "<hr/>");

        tinyxml2::XMLDocument doc;
        doc.Parse((const char*)new_page_source.c_str(), new_page_source.size());

        // Recursivly fix till there aren't any errors
        if(doc.ErrorID()) {
            /*tick += 1;

            // Only try to fix the document 20 times
            if (tick < 20)*/
                return page_source;;

            //new_page_source = validate_and_fix(new_page_source, tick);
        }

        return new_page_source;
    }
}