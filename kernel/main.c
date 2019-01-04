#include "support/console.h"
#include "support/serial.h"
#include "support/string.h"
#include "support/math.h"
#include "boot/kernelmain.h"

int kernel_main(void) {
    //load_gdt();
    terminal_initialize();
    init_serial();

    printf("Welcome to GEGIX\n");
    printf("The newest (and coolest) operating system\n");
    printf("hi %f hi", 12.3456789);
    return 0;
}
