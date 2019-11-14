#include <stdio.h>
#include <string.h>
#include "bitpack.h"

/* replace any non-ascii chars, nulls or dels in read block with spaces */
void strip(char ascii[])
{
   int i;

   for (i = 0; i < strlen(ascii); ++i)
      if ( ascii[i] < 00 || ascii[i] > 0177 ) /* 00 = 0x0, 0177 = 0x7f */
         ascii[i] = ' ';
}

long long pack(char *block)
{
   int i, n = strlen(block);
   long long word = 0LL, work = 0LL;

   if (n > 8)
      n = 8;
   for (i = 0; i < n; ++i) {
      work = block[i];
      work <<= i*7;
      word ^= work;
   }
   return(word);
}

void unpack(char bytes[], long long word)
{
   int i;

   memset(bytes, '\0', 9);
   for (i = 0; i < 8; ++i)
      bytes[i] = (word >> i*7) & 0x7f;
}

void packarr(char *dst, char *src)
{
   int i;
   long long word;
   char bytes[9];

   strip(src);
   memset(dst, '\0', 71);
   for (i = 0; i < 10; ++i) {
      memset(bytes, '\0', sizeof(bytes));
      memcpy(bytes, src + i*8, sizeof(bytes)-1);
      word = pack(bytes);
      memcpy(dst + i*7, &word, 7);
   }
}

void unpackarr(char *dst, char *src)
{
   int i;
   long long word;
   char bytes[9];

   memset(dst, '\0', 81);
   for (i = 0; i < 10; ++i) {
      word = 0LL;
      memcpy(&word, src + i*7, 7);
      unpack(bytes, word);
      memcpy(dst + i*8, bytes, 8);
   }
}
