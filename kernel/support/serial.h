#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef SERIAL_H
#define SERIAL_H

void init_serial() ;
int serial_recieved_yet();
char read_serial();
int able_to_transmit();
void write_serial(char a);

void serial_putchar(char c);
void serial_write(const char* data, size_t size);
void serial_writestring(const char* data);

#endif
