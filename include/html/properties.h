#pragma once
namespace browser {
    namespace elements {
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

        struct color {
            short r = 0, g = 0, b = 0, a = 0;
        };

        struct position {
            int top = 0, right = 0, 
            bottom = 0, left = 0;
        };

        struct properties {
            displayModes display = Block;
            int x = 0, y = 0;
            int height = 0, width = 0;

            browser::elements::position padding;
            browser::elements::position margin;

            short lineHeight = 0;
            short fontSize = 0;

            browser::elements::color color;
            browser::elements::color background;
        };
    }
}