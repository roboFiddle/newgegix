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