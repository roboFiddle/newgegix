#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#ifndef STRING_H
#define STRING_H

unsigned strlen(const char *s);
void strtoupper(char *s);
void strtolower(char *s);
int str2int(const char *s, int *d);
int uint_to_string(uint32_t u, char *str);
int uint_to_hex(uint32_t u, char *str);

void memset(void *d, char value, uint32_t length);
void memcpy(void *d, const void *s, uint32_t length);
int sprintf_processed(char *str, const char *format, va_list args, uint8_t mode);
int sprintf(char *str, const char *format, ...);
int printf(const char *format, ...);
int serial_printf(const char *format, ...);

#endif
