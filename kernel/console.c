#include "console.h"
#include "string.h"
#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

#define VGA_HEIGHT 24
#define VGA_WIDTH 80

uint32_t terminal_row = 0;
uint32_t terminal_column = 0;
uint16_t terminal_color = 0;
uint16_t* terminal_buffer;
uint8_t terminal_contents[80][24];
uint8_t terminal_colors[80][24];


void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

void set_cursor_current_position()
{
    fb_move_cursor(terminal_row * 80 + terminal_column);
}

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
            terminal_contents[x][y] = ' ';
            terminal_colors[x][y] = terminal_color;
        }
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_clear(void)
{
    for(size_t i=0; i<80; i++)
    {
        for(size_t j=0; j<25; j++)
        {
            terminal_putentryat(' ', vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK), i, j);
        }
    }
}

void terminal_scroll(void)
{
    for(int row=1; row < 25; row++)
    {
        for(int col=0; col<80; col++)
        {
            terminal_contents[col][row-1] = terminal_contents[col][row];
            terminal_colors[col][row-1] = terminal_colors[col][row];
        }
    }
    for(int col=0; col<80; col++) // Clear Last Row too
    {
        terminal_contents[col][24] = ' ';
    }
}

void terminal_redraw(void)
{
    for(int i=0; i<80; i++)
    {
        for(int j=0; j<25; j++)
        {
            terminal_putentryat(terminal_contents[i][j], terminal_colors[i][j], i, j);
        }
    }
}

void terminal_putchar_color(char c, uint8_t color)
{
    set_cursor_current_position();
    if(c == '\n') // Don't Print Anything on Newline
    {
        terminal_row += 1;
        terminal_column = 0;
        if(terminal_row == VGA_HEIGHT)
        {
            terminal_row = VGA_HEIGHT - 1;
            terminal_clear();
            terminal_scroll();
            terminal_redraw();
        }
    }
    else
    {
        terminal_putentryat(c, color, terminal_column, terminal_row);
        terminal_contents[terminal_column][terminal_row] = c;
        terminal_colors[terminal_column][terminal_row] = color;
        terminal_column += 1;
        if(terminal_column == VGA_WIDTH)
        {
            terminal_putchar_color('\n', 0);
        }
    }
}

void terminal_putchar_2color(char c, enum vga_color text_color, enum vga_color background_color )
{
    terminal_putchar_color(c, vga_entry_color(text_color, background_color));
}

void terminal_putchar(char c)
{
    terminal_putchar_color(c, terminal_color);
}

void terminal_write(const char* data, size_t size, uint8_t color)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar_color(data[i], color);
}

void terminal_writestring(const char* data, uint8_t color)
{
    terminal_write(data, strlen(data), color);
}
