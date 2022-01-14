#ifndef SCREEN_H
#define SCREEN_H

#include "screen.h"

enum 
{
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    PURPLE,
    BROWN,
    GRAY,
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTPURPLE,
    YELLOW,
    WHITE
};

void clear_screen();
void putch(unsigned char c);
void puts(const char* text);
void set_text_color(unsigned char forecolor, unsigned char backcolor);
void init_screen();

#endif 