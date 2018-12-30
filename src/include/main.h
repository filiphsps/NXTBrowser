#pragma once

/* TODO: move into a config.h file */

// Debug-specific
#define DEBUG
#define DEBUG_CONSOLE_WIDTH 350
//#define DEBUG_DRAW_DOM

// Feature-toggles
#define TODO true
#define ___NATIVE_GUI___ true

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "libs/tinyxml2/tinyxml2.h"

struct device_aspect {
    short scaling = 1;
    int w = 0;
    int h = 0;
};

extern device_aspect DEVICE;
static std::string page = "";