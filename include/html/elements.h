#pragma once
#include "../../main.h"

namespace browser {
    namespace elements {
        struct properties {

        }

        class element {
            private:
                browser::elements::properties properties;
            public:
                browser::elements::properties getDefaultData () {
                    return this.properties;
                }
        };
    }
}