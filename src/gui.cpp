#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"
#undef RAYGUI_IMPLEMENTATION
#include "gui.hpp"
#include "../include/style_cyber.h"

void GUI::init(const int font_size)
{
    GuiLoadStyleCyber();
    GuiSetStyle(DEFAULT, TEXT_SIZE, font_size);
}
