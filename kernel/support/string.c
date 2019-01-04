#include <stdint.h>
#include <stdarg.h>
#include "math.h"
#include "serial.h"
#include "console.h"

unsigned strlen(const char *s)
{
    unsigned len = 0;
    while(*s) {
        len++;
        s++;
    }
    return len;
}

void strtoupper(char *s)
{
    while(*s) {
        if(*s >= 'a' && *s <= 'z')
            *s -= 'a' - 'A';
        s++;
    }
}
void strtolower(char *s)
{
    while(*s) {
        if(*s >= 'A' && *s <= 'Z')
            *s += 'a' - 'A';
        s++;
    }
}
int str2int(const char *s, int *d) {
    for(; *s; ++s) {
        *d *= 10;
        if(*s >= '0' && *s <= '9')
            *d += (*s - '0');
        else
            return 0;
    }
    return 1;
}

int uint_to_string_based(uint32_t u, char *str, uint8_t base) {
  uint32_t t = 1;
  uint32_t q = 0;

  do {
    t *= base;
    q += 1;
  } while(t < u);

  t /= base;

  uint8_t digit;
  uint32_t i = 0;

  do {
    digit = u/t;
    if(digit < 10)
      str[i] = digit + '0';
    else
      str[i] = (digit-10) + 'A';
    u -= t*digit;
    t /= base;
    i++;
  } while(t != 1);

  digit = u/t;
  if(digit < 10)
    str[i] = digit + '0';
  else
    str[i] = (digit-10) + 'A';

  return q;
}

int uint_to_string(uint32_t u, char *str) {
  return uint_to_string_based(u, str, 10);
}

int uint_to_hex(uint32_t u, char *str) {
    return uint_to_string_based(u, str, 16);
}

int float_to_string(double x, char *str) {
    int current_str_index = 0;
    if(x < 0) {
        str[current_str_index] = '-';
        current_str_index++;
        x *= -1;
    }
    int whole;
    int dec = fp_mod(x, &whole);
    int used = uint_to_string(whole, str+current_str_index);
    current_str_index += used;
    str[current_str_index] = '.';
    current_str_index++;
    used += uint_to_string(dec, str+current_str_index);
    return used;
}


void memset(void *d, char value, uint32_t length) {
  for(uint32_t i=0; i < length; i++) {
    ((char*) d)[i] = value;
  }
}
void memcpy(void *d, const void *s, uint32_t length) {
  for(uint32_t i=0; i < length; i++) {
    ((char*) d)[i] = ((char*) s)[i];
  }
}

#define set_char(x, y) if(mode == 0) { str[x] = y; } else if (mode == 1) { terminal_putchar(y); } \
                        else if(mode == 2) { write_serial(y); }

int sprintf_processed(char *str, const char *format, va_list args, uint8_t mode) {
  uint32_t return_status = 0;
  uint32_t loop_status = 0;
  uint32_t current_index_str = 0;
  char buffer[64];
  while(*format) {
    if(loop_status == 1) {
      if(*format == 'd' || *format == 'i') {
          int num = va_arg(args, int);
          if(num < 0) {
            set_char(current_index_str, '-');
            current_index_str++;
            num *= -1;
          }
          memset(&buffer, 0x00, 64);
          uint_to_string((uint32_t) num, buffer);
          char* buffer_a = (char*) &buffer;
          while(*buffer_a) {
            set_char(current_index_str, *buffer_a);
            current_index_str++;
            buffer_a++;
          }
      }
      else if(*format == 'h') {
        int num = va_arg(args, int);
        if(num < 0) { num *= -1; }
        memset(&buffer, 0x00, 64);
        uint_to_hex((uint32_t) num, buffer);
        char* buffer_a = (char*) &buffer;
        while(*buffer_a) {
          set_char(current_index_str, *buffer_a);
          current_index_str++;
          buffer_a++;
        }
      }
      else if(*format == 'f') {
          double num = va_arg(args, double);
          memset(&buffer, 0x00, 64);
          float_to_string(num, buffer);
          char* buffer_a = (char*) &buffer;
          while(*buffer_a) {
              set_char(current_index_str, *buffer_a);
              current_index_str++;
              buffer_a++;
          }
      }
      else if(*format == 's') {
          char *s = va_arg(args, char*);
          while(*s) {
              set_char(current_index_str, *s);
              current_index_str++;
              s++;
          }
      }
      else if (*format == '%') {
        set_char(current_index_str, '%');
        current_index_str++;
      }
      else {
          return_status = 2;
          return return_status;
      }
      loop_status = 0;
    }
    else if(*format == '%') {
      loop_status = 1;
    }
    else {
      set_char(current_index_str, *format);
      current_index_str++;
    }
    format++;
  }
  return return_status;
}
int sprintf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    sprintf_processed(str, format, args, 0);
    va_end(args);
    return 0;

}
int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    sprintf_processed(0x0, format, args, 1);
    va_end(args);
    return 0;
}
int serial_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    sprintf_processed(0x0, format, args, 2);
    va_end(args);
    return 0;
}