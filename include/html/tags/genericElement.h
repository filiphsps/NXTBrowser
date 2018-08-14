#pragma once

#include "../properties.h"

namespace browser {
    namespace elements {
        class GenericElement {
            private:
                browser::elements::properties properties;
                const browser::elements::properties defaultProperties;
            public:
                browser::elements::properties getDefaultProperties () {
                    return this->properties;
                }
        };
    }
}