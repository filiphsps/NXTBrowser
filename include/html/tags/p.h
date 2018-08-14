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
                    maxHeight: DEVICE_HEIGHT, maxWidth: DEVICE_WIDTH,

                    padding: {0, 0, 0, 0},
                    margin: {16, 0, 16, 0},

                    fontSize: 16,
                    fontStyle: Normal,

                    color: {55, 55, 55, 255},
                    background: {255, 255, 255, 0}
                };
                
                P(std::string content) : browser::elements::GenericTextElement(content) {
                    this->properties = this->defaultProperties;
                }
        };
    }
}