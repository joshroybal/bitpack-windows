#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bitpack.h"

int main(int argc, char *argv[])
{
   int i, n, blksiz, nb;
   char byte_buf[81], ascii_buf[71];
   char *ibuf, *obuf;
   FILE *ifp, *ofp;

   if (argc < 4) {
      fprintf(stderr, "Usage: %s pack|unpack infile outfile\n", argv[0]);
      return(1);
   }

   if (strcmp(argv[1], "pack") == 0) {
      blksiz = 80;
      ibuf = byte_buf;
      obuf = ascii_buf;
   }
   else if (strcmp(argv[1], "unpack") == 0) {
      blksiz = 70;
      ibuf = ascii_buf;
      obuf = byte_buf;
   }
   else {
      fprintf(stderr, "Usage: %s pack|unpack infile outfile\n", argv[0]);
      return(1);
   }

   if ( (ifp = fopen(argv[2], "rb")) == (FILE *) NULL ) {
      fprintf(stderr, "error opening file %s\n", argv[2]);
      return(1);
   }

   if ( (ofp = fopen(argv[3], "wb")) == (FILE *) NULL ) {
      fprintf(stderr, "error opening file %s\n", argv[3]);
      return(1);
   }

   memset(ibuf, '\0', blksiz+1);
   n = fread(ibuf, 1, blksiz, ifp);
   while (n > 0) {
      if (blksiz == 80) {
         packarr(obuf, ibuf);
         nb = (strlen(ibuf) < 80) ? strlen(ibuf)*7/8+1 : 70;
      }
      else {
         unpackarr(obuf, ibuf);
         nb = strlen(obuf);
      }
      fwrite(obuf, nb, 1, ofp);
      memset(ibuf, '\0', blksiz+1);
      n = fread(ibuf, 1, blksiz, ifp);
   }

   fclose(ifp);
   fclose(ofp);

   if (strcmp(argv[1], "pack") == 0) {
      printf("%s packed to %s\n", argv[2], argv[3]);
   }
   else {
      printf("%s unpacked from %s\n", argv[3], argv[2]);
   }

   return(0);
}
