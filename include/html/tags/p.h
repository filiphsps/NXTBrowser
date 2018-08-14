#pragma once

#include <iostream>

#include "../properties.h"

namespace browser {
    namespace elements {
        class P: public browser::elements::GenericTextElement {
            private:
            public:
                const browser::elements::properties defaultProperties = {
                    display: Block,
                    x: 0, y: 0,
                    height: 0, width: 0,

                    padding: {0, 0, 0, 0},
                    margin: {16, 0, 16, 0},

                    fontSize: 16,
                    fontStyle: Normal,

                    color: {55, 55, 55, 255},
                    background: {255, 255, 255, 0}
                };
                
                P(browser::elements::properties *props, std::string content) : browser::elements::GenericTextElement(props, content) {
                    this->properties = (browser::elements::properties*)&this->defaultProperties;
                }
        };
    }
}