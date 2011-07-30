#/* 
# * uri escaping polyglot (C, Makefile)
# *
# * Copyright (c) 2007  A. Carl Douglas
# *
# * To compile as a test program:
# *   make -f uri.c
# *
# * Otherwise, link into uri.c into your program.
# *
# *   void uri_encode (char *src, char *dst, int src_len);
# *   void uri_decode (char *src, char *dst, int src_len);
# */

# /* Makefile

uri: uri.c
	gcc -Wall -ansi -o uri uri.c -DTEST

clean:
	rm uri

define c_source
# */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

void uri_encode (char *src, char *dst, int len);
void uri_decode (char *src, char *dst, int len);

void 
uri_decode(char *src, char *dst, int len)
{
  char           *ptr;

  for (ptr = src; *ptr && len > 0;) {
    if (*ptr == '%') {
      unsigned int    ch;
      ptr++;
      ch  = (*ptr >= '0' && *ptr <= '9') ? *ptr - '0' : *ptr - 'A' + 10;
      ch *= 16;
      ptr++;
      ch += (*ptr >= '0' && *ptr <= '9') ? *ptr - '0' : *ptr - 'A' + 10;
      ptr++;
      *dst = ch;
      dst++;
      len -= 3;
    } else {
      *dst = *ptr;
      ptr++;
      dst++;
      len--;
    }
  }
  *dst = '\0';
}

void 
uri_encode(char *src, char *dst, int len)
{
  static const char * const hex_char = "0123456789ABCDEF";

  char           *ptr;

  for (ptr = src; *ptr && len > 0; ptr++) {
    if (isalnum(*ptr)) {
      *dst = *ptr;
      dst++;
      len--;
    } else {
      *(dst++) = '%';
      *(dst++) = *(hex_char + ((unsigned char)*ptr >> 4));
      *(dst++) = *(hex_char + ((unsigned char)*ptr & 0x0F));
      len -= 3;
    }
  }
  *dst = '\0';
}

#ifdef TEST
int main () {
  char a[255];
  char b[255];
  char c[255];
  int i;
  sprintf(a, "HELLO, world '12$$%%2131@@#(*^*(&<> \" //\\ ?:;");
  printf("%s\n", a);
  uri_encode(a, b, 255);
  printf("%s\n", b);
  uri_decode(b, c, 255);
  printf("%s\n", c);
  i = strcmp(a,c);
  printf("%s\n", i == 0 ? "PASSED" : "FAILED");
  return i;
}
#endif

/* back to makefile
endef
#*/

