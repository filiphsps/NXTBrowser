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
        class GenericTextElement: public browser::elements::GenericElement {
            private:
                TTF_Font *font;
                std::string content;
            public:
                GenericTextElement(browser::elements::properties *props, std::string content) : browser::elements::GenericElement(props) {
                    this->content = content;
                }

                //Temp
                void SetFont(TTF_Font *font) {
                    this->font = font;
                }

                virtual browser::elements::renderQueueItem getRenderQueueItem () {
                    if (this->properties->width && this->properties->height)
                        return browser::elements::GenericElement::getRenderQueueItem();
                    
                    int width = this->properties->maxWidth - (this->properties->margin.left + this->properties->margin.right +
                        this->properties->padding.left + this->properties->padding.right);

                    // Calculate text width & height
                    // TODO: Separate this into GUI lib
                    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(this->font, this->content.c_str(), {0, 0, 0, 0}, width);
                    if (!this->properties->height)
                        this->properties->height = surface->h;
                    if (!this->properties->width)
                        this->properties->width = surface->w;
                    
                    SDL_FreeSurface(surface);

                    console.printf("DOM->PARSER->GenericTextElement->width: " + std::to_string((this->properties->margin.left + this->properties->margin.right +
                        this->properties->padding.left + this->properties->padding.right)));

                    return browser::elements::GenericElement::getRenderQueueItem();
                }
        };
    }
}