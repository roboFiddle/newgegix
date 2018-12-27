#include "console.h"

int kernel_main(void) {
    terminal_initialize();
    terminal_writestring("Welcome to GEGIX\n", vga_entry_color(15, 0));
}
