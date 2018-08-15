#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../../console.h"

#include "../utils.h"
#include "../properties.h"
#include "genericElement.h"

extern Console console;

namespace browser {
    namespace elements {
        class GenericTextElement: public browser::elements::GenericElement {
            private:
            public:
                TTF_Font *font;
                std::string content;

                GenericTextElement(std::string content) : browser::elements::GenericElement() {
                    this->elementType = browser::elements::elementTypes::Text;
                    this->content = content;
                }

                virtual TTF_Font* getFont() { return this->font; };
                virtual browser::elements::renderQueueItem getRenderQueueItem (SDL_Surface* _surface) {
                    if (this->properties.width > 0 && this->properties.height > 0)
                        return browser::elements::GenericElement::getRenderQueueItem(_surface);
                    
                    // Update maxwidth & maxheight
                    if(this->properties.maxWidth <= 0)
                        this->properties.maxWidth = _surface->w;
                    if(this->properties.maxHeight <= 0)
                        this->properties.maxHeight = _surface->h;
                    
                    // Calculate width 
                    int width = this->properties.maxWidth - (this->properties.margin.left + this->properties.margin.right +
                        this->properties.padding.left + this->properties.padding.right);

                    // Set font style
                    switch (this->properties.fontStyle) {
                        case Bold:
                            #ifdef __SWITCH__
                                this->font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", (int)this->properties.fontSize);
                                TTF_SetFontStyle(this->font, TTF_STYLE_BOLD);
                            #elif __MACOS__
                                this->font = browser::utils::get_font_from_cache("/Library/Fonts/Arial Bold.ttf", (int)this->properties.fontSize);
                            #else
                                this->font = browser::utils::get_font_from_cache("../../resources/fonts/NintendoStandard.ttf", (int)this->properties.fontSize);
                                TTF_SetFontStyle(this->font, TTF_STYLE_BOLD);
                            #endif
                            break;
                        case Normal:
                        default:
                            #ifdef __SWITCH__
                                this->font = browser::utils::get_font_from_cache("romfs:/fonts/NintendoStandard.ttf", (int)this->properties.fontSize);
                            #elif __MACOS__
                                this->font = browser::utils::get_font_from_cache("/Library/Fonts/Arial.ttf", (int)this->properties.fontSize);
                            #else
                                this->font = browser::utils::get_font_from_cache("../../resources/fonts/NintendoStandard.ttf", (int)this->properties.fontSize);
                            #endif
                            TTF_SetFontStyle(this->font, TTF_STYLE_NORMAL);
                            break;
                    }

                    // Calculate text width & height
                    // TODO: Separate this into GUI lib
                    SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(this->font, this->content.c_str(), {0, 0, 0, 0}, width);

                    if (this->properties.height <= 0)
                        this->properties.height = surface->h;
                    if (this->properties.width <= 0)
                        this->properties.width = surface->w;

                    SDL_FreeSurface(surface);

                    //console.printf("DOM->Parser->GenericTextElement->width: " + std::to_string(width));
                    
                    return browser::elements::GenericElement::getRenderQueueItem(_surface);
                }
        };
    }
}