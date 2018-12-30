#pragma once

/* TODO: move into a config.h file */

// Debug-specific
#define DEBUG
#define DEBUG_CONSOLE_WIDTH 350
//#define DEBUG_DRAW_DOM

// Feature-toggles
#define TODO true

#include <iostream>
#include "console.h"

extern device_aspect DEVICE;
static std::string page = "";