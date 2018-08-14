#pragma once

#include <iostream>

#include "../properties.h"
#include "genericElement.h"

namespace browser {
    namespace elements {
        class GenericTextElement: public browser::elements::GenericElement {
            private:
            public:
                GenericTextElement(browser::elements::properties *props, std::string content) : browser::elements::GenericElement(props) {
                    
                }
        };
    }
}