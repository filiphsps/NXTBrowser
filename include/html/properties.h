#pragma once
#include "../main.h"

namespace browser {
    namespace elements {
        enum elementTypes {
            Container,
            Text,
            Interactable,
            Special
        };

        enum displayModes {
            Inline,
            InlineBlock,
            InlineFlex,
            InlineGrid,
            InlineTable,

            Block,
            Contents,
            Flex,
            Grid,
            Table
        };

        enum fontModes {
            Normal,
            Bold,
            Italic,
            Underline,
            Strikethrough,
        };

        struct color {
            short r = 0, g = 0, b = 0, a = 0;
        };

        struct position {
            int top = 0, right = 0, 
            bottom = 0, left = 0;
        };

        struct size {
            int height = 0,
                width = 0;
        };

        struct properties {
            displayModes display = Block;
            int x = 0, y = 0;
            int height = 0, width = 0;
            int maxHeight = DEVICE_HEIGHT, maxWidth = DEVICE_WIDTH;
            int minHeight = 0, minWidth = 0;

            browser::elements::position padding;
            browser::elements::position margin;

            short lineHeight = 0;
            short fontSize = 0;
            fontModes fontStyle = Normal;

            browser::elements::color color;
            browser::elements::color background;
        };

        struct renderQueueItem {
            browser::elements::size size;
            browser::elements::properties *properties;
        };
    }
}