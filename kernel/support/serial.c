#include "string.h"
#include "io.h"


#define PORT 0x3f8   /* COM1 */

void init_serial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int serial_recieved_yet() {
   return inb(PORT + 5) & 1;
}

char read_serial() {
   while (serial_recieved_yet() == 0);
   return inb(PORT);
}

int able_to_transmit() {
   return inb(PORT + 5) & 0x20;
}

void write_serial(char a) {
   while (able_to_transmit() == 0);
   outb(PORT,a);
}

void serial_putchar(char c) {
  write_serial(c);
}
void serial_write(const char* data, size_t size) {
  for (size_t i = 0; i < size; i++)
      serial_putchar(data[i]);
}
void serial_writestring(const char* data) {
  serial_write(data, strlen(data));
}
