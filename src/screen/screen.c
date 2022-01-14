#include "screen.h"
#include "memory/memory.h"
#include "io/io.h"
#include "string/string.h"

int pos_x, pos_y = 0;
int attrib = 0x0F;
unsigned short *text_ptr;


void scroll()
{
    unsigned blank, temp;

    blank = 0x20 | (attrib << 8);

    if (pos_y >= 25)
    {
        temp = pos_y - 25 + 1;
        memcpy(text_ptr, text_ptr + temp * 80, (25 - temp) * 80 * 2);

        memsetw(text_ptr + (25 - temp) * 80, blank, 80);
        pos_y = 25 - 1;
    }
}


void update_cursor()
{
    unsigned temp;

    temp = pos_y * 80 + pos_x;

    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}


void clear_screen()
{
    unsigned blank;
    int i;

    blank = 0x20 | (attrib << 8);

    for (i = 0; i < 25; i++)
        memsetw(text_ptr + i * 80, blank, 80);
    
    pos_x = 0;
    pos_y = 0;
    update_cursor();
}


void putch(unsigned char c)
{
    unsigned short* where;
    unsigned att = attrib << 8;

    if (c == 0x08)
    {
        if (pos_x != 0)
        {
            pos_x--;
            where = text_ptr + (pos_y * 80 + pos_x);
            *where = ' ' | att;
        }
        
        if (pos_y > 0 && pos_x == 0)
        {
            pos_y--;
            pos_x = 80;
        }
    }

    else if (c == 0x09)
    {
        pos_x = (pos_x + 8) & ~(8 - 1);
    }

    else if (c == '\r')
    {
        pos_x = 0;
    }

    else if (c == '\n')
    {
        pos_x = 0;
        pos_y++;
    }

    else if (c >= ' ')
    {
        where = text_ptr + (pos_y * 80 + pos_x);
        *where = c | att;
        pos_x++;
    }

    if (pos_x >= 80 && c != 0x08)
    {
        pos_x = 0;
        pos_y++;
    }
    
    scroll();
    update_cursor();
}


void puts(const char* text)
{
    int i;
    for (i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
}


void set_text_color(unsigned char forecolor, unsigned char backcolor)
{
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}


void init_screen()
{
    text_ptr = (unsigned short*) 0xB8000;
    set_text_color(WHITE, BLACK);
    clear_screen();
}