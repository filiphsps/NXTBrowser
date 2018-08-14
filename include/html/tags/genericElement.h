#pragma once

#include "../../console.h"

#include "../properties.h"

extern Console console;

namespace browser {
    namespace elements {
        class GenericElement {
            private:
                const browser::elements::properties *defaultProperties;
            public:
                browser::elements::properties *properties;

                GenericElement (browser::elements::properties *props) {
                    //TODO: Merge defaultProperties and props into properties
                    if(props != nullptr)
                        this->properties = props;
                    else
                        this->properties = (browser::elements::properties*)this->getDefaultProperties();
                }
                ~GenericElement () {

                }

                const browser::elements::properties* getDefaultProperties () {
                    return this->defaultProperties;
                }

                virtual browser::elements::renderQueueItem getRenderQueueItem () {
                    // Get element height and width
                    int width = this->properties->width, height = this->properties->height;

                    if (!width)
                        width = 0;
                    if (!height)
                        height = 0;
                    
                    // Add Padding
                    width       += this->properties->padding.right + this->properties->padding.right,
                    height      += this->properties->padding.top + this->properties->padding.bottom;

                    // Add Margin
                    width       += this->properties->margin.right + this->properties->margin.right;
                    height      += this->properties->margin.top + this->properties->margin.bottom;

                    //console.printf("DOM->PARSER->GenericElement->Height: " + std::to_string(height));

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