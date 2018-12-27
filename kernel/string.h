#ifndef STRING_H
#define STRING_H

unsigned strlen(const char *s);
void strtoupper(char *s);
void strtolower(char *s);
int str2int(const char *s, int *d);
char *uint_to_string(uint32_t u, char *str);
void memset(void *d, char value, unsigned length);
void memcpy(void *d, const void *s, unsigned length);
void printf(const char *s, ...);

#endif