#pragma once

/* TODO: move into a config.h file */

// Debug-specific
#define DEBUG
#define DEBUG_CONSOLE_WIDTH 350
#define DEBUG_DRAW_DOM

// Feature-toggles
#define TODO true

#include <iostream>

#include "sdl_helper.h"
#include "tinyxml2.h"
#include "html.h"

extern Console console;
extern device_aspect DEVICE;
static std::string page = "";