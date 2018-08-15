#pragma once
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#include <iostream>

#include "../main.h"
#include "../console.h"

struct font {
    TTF_Font* font;
    std::string fontPath;
    int fontSize;
};

extern Console console;

static std::vector<font> fontCache;
namespace browser {
    namespace utils {
        TTF_Font* get_font_from_cache(std::string path, int size) {
            /*if ((int)fontCache.size() > 5) {
                TTF_CloseFont(fontCache.begin()->font);
                fontCache.erase(fontCache.begin());
            }*/

            for (int i = 0; i < (int)fontCache.size(); i++) {
                if (fontCache[i].fontSize == size && fontCache[i].fontPath == path) {
                    if (fontCache[i].font == NULL) {
                        fontCache.erase(fontCache.begin() + i);
                        continue;
                    }

                    return fontCache[i].font;
                }
            }

            TTF_Font *font = TTF_OpenFont(path.c_str(), size);
            fontCache.push_back({ font, path, size });
            return font;
        }
    }
}