#include "support/console.h"
#include "support/serial.h"
#include "support/string.h"

int kernel_main(void) {
    terminal_initialize();
    init_serial();

    terminal_writestring("Welcome to GEGIX\n", vga_entry_color(15, 0));
    terminal_writestring("The newest (and coolest) operating system\n", vga_entry_color(15, 0));
    char haha[80];
    sprintf(haha, "hi testing %d %i %h %%%% hiiiiiii", -4321, 9876, 0xDEAD);
    terminal_writestring(haha, vga_entry_color(15, 0));
    serial_writestring(haha);
    return 0;
}
