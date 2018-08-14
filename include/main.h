#pragma once

// Debug-specific
#define DEBUG
//#define DEBUG_DRAW_DOM
//#define DEBUG_DRAW_DOM_UPDATES // FIXME:

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