#pragma once

#include "../properties.h"

namespace browser {
    namespace elements {
        class GenericElement {
            private:
                browser::elements::properties *properties;
                const browser::elements::properties *defaultProperties;
            public:
                GenericElement (browser::elements::properties *props) {
                    //TODO: Merge defaultProperties and props into properties
                    this->properties = props;
                }
                ~GenericElement () {

                }

                const browser::elements::properties* getDefaultProperties () {
                    return this->defaultProperties;
                }

                virtual browser::elements::renderQueueItem getRenderDetails () {
                    // Get element height and width
                    int width = this->properties->width, height  = this->properties->height;
                    
                    // Add Padding
                    width       += this->properties->padding.right + this->properties->padding.right,
                    height      += this->properties->padding.top + this->properties->padding.bottom;

                    // Add Margin
                    width       += this->properties->margin.right + this->properties->margin.right;
                    height      += this->properties->margin.top + this->properties->margin.bottom;

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