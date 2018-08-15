#pragma once

/* TODO: move into a config.h file */

// Debug-specific
#define DEBUG
#define DEBUG_CONSOLE_WIDTH 400
#define DEBUG_DRAW_DOM
//#define DEBUG_DRAW_DOM_UPDATES

// Device-specific
#define DEVICE_HEIGHT 720
#define DEVICE_WIDTH 1280

// Feature-toggles

#include <iostream>

#include "sdl_helper.h"
#include "tinyxml2.h"
#include "html.h"

extern Console console;
static std::string page = "";