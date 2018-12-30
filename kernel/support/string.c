#include <stdint.h>
#include <stdarg.h>
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

void uint_to_string_based(uint32_t u, char *str, uint8_t base) {
  uint32_t t = 1;

  do {
    t *= base;
  } while(t < u);

  t /= 10;

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
}

void uint_to_string(uint32_t u, char *str) {
  uint_to_string_based(u, str, 10);
}

int uint_to_hex(uint32_t u, char *str) {
  uint_to_string_based(u, str, 16);
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

int sprintf(char *str, const char *format, ...) {
  va_list args;
  uint32_t return_status = 0;
  uint32_t loop_status = 0;
  uint32_t current_index_str = 0;
  char buffer[64];
  va_start(args, format);
  while(*format) {
    if(loop_status == 1) {
      if(*format == 'd' || *format == 'i') {
          int num = va_arg(args, int);
          if(num < 0) {
            str[current_index_str] = '-';
            current_index_str++;
            num *= -1;
          }
          memset(&buffer, 0x00, 64);
          uint_to_string((uint32_t) num, buffer);
          char* buffer_a = (char*) &buffer;
          while(*buffer_a) {
            str[current_index_str] = *buffer_a;
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
          str[current_index_str] = *buffer_a;
          current_index_str++;
          buffer_a++;
        }
      }
      else if (*format == '%') {
        str[current_index_str] = '%';
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
      str[current_index_str] = *format;
      current_index_str++;
    }
    format++;
  }
  va_end(args);
  return return_status;
}
