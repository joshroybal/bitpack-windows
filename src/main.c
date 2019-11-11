/* copyright (c) 2019 by Josh Roybal */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bitpack.h"

int main(int argc, char *argv[])
{
   int i, n, blksiz;
   long long word = 0LL;
   char bytes[9];
   char *ascii_text;
   FILE *fp;

   if (argc < 4) {
      fprintf(stderr, "Usage: %s pack|unpack infile outfile\n", argv[0]);
      return(1);
   }

   if (strcmp(argv[1], "pack") != 0 && strcmp(argv[1], "unpack") != 0) {
      fprintf(stderr, "Usage: %s pack|unpack infile outfile\n", argv[0]);
      return(1);
   }

   if ( (fp = fopen(argv[2], "rb")) == (FILE *) NULL ) {
      fprintf(stderr, "error opening file %s\n", argv[2]);
      return(1);
   }
   fseek(fp, 0, SEEK_END);
   n = ftell(fp);
   fseek(fp, 0, SEEK_SET);
   ascii_text = (char *) malloc((n+1) * sizeof(char));
   fread(ascii_text, n, 1, fp);
   fclose(fp);

   if ( (fp = fopen(argv[3], "wb")) == (FILE *) NULL ) {
      fprintf(stderr, "error opening file %s\n", argv[3]);
      return(1);
   }

   if (strcmp(argv[1], "pack") == 0) { /* write blocks of packed bits to file */
      memset(bytes, '\n', 9);
      for (i = 0; i < n; i += 8) {
         strncpy(bytes, ascii_text + i, 8);
         word = pack(bytes);
         blksiz = (strlen(bytes) < 7) ? strlen(bytes) : 7;
         fwrite(&word, blksiz, 1, fp);
      }
      printf("%s packed into %s\n", argv[2], argv[3]);
   }
   else {   /* convert packed ascii back to byte sized chars */
      for (i = 0; i < n; i += 7) {
         memcpy(&word, ascii_text + i, 7);
         unpack(bytes, word);
         blksiz = strlen(bytes);
         fwrite(bytes, 1, blksiz, fp);
      }
      printf("%s unpacked from %s\n", argv[3], argv[2]);
   }

   fclose(fp);
   free(ascii_text);

   return(0);
}
