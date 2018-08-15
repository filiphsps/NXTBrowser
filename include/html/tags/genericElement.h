#pragma once

#include "../../console.h"

#include "../properties.h"

extern Console console;

namespace browser {
    namespace elements {
        class GenericElement {
            private:
            public:
                browser::elements::elementTypes elementType = browser::elements::elementTypes::Container;
                const browser::elements::properties defaultProperties;
                browser::elements::properties properties;

                GenericElement () {
                    this->properties = this->defaultProperties;
                }
                ~GenericElement () {
                    //delete &this->properties, &this->defaultProperties;
                }

                const browser::elements::properties getDefaultProperties () {
                    return defaultProperties;
                }

                void SetFont(TTF_Font *font) {};

                browser::elements::renderQueueItem getRenderQueueItem () {
                    // Get element height and width
                    int width = this->properties.width, height = this->properties.height;
                    
                    // Add Padding
                    width       += this->properties.padding.right + this->properties.padding.right,
                    height      += this->properties.padding.top + this->properties.padding.bottom;

                    // Add Margin
                    width       += this->properties.margin.right + this->properties.margin.right;
                    height      += this->properties.margin.top + this->properties.margin.bottom;

                    //console.printf("DOM->Parser->GenericElement->height: " + std::to_string(height));
                    //console.printf("DOM->Parser->GenericElement->width: " + std::to_string(width));

                    return (browser::elements::renderQueueItem){
                        size: {
                            height: height,
                            width: width
                        },
                        properties: this->properties
                    };
                }
        };
    }
}