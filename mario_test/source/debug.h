#ifndef CRO_DEBUG_H
#define CRO_DEBUG_H

extern void __2snprintf(char *s, uint32_t num_chars, const char *format, ...);
int dumb_strlen(char *str);
void debug_print(char *str);

#define printf(s...) \
{\
   char temp[64+1]; \
   __2snprintf(temp, 64, s); \
   debug_print(temp); \
}

#endif

