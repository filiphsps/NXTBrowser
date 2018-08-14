#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../../console.h"

#include "../properties.h"
#include "genericElement.h"

extern Console console;

namespace browser {
    namespace elements {
        class Br: public browser::elements::GenericElement {
            private:
            public:
                const browser::elements::properties defaultProperties = {
                    display: Block,
                    x: 0, y: 0,
                    height: 0, width: 0,

                    padding: {0, 0, 0, 0},
                    margin: {16, 0, 16, 0},
                };
                
                Br(browser::elements::properties *props) : browser::elements::GenericElement(props) {
                    this->properties = (browser::elements::properties*)&this->defaultProperties;
                }

                virtual browser::elements::renderQueueItem getRenderQueueItem () {
                    return browser::elements::GenericElement::getRenderQueueItem();
                }
        };
    }
}