/*
 * @(#)encrypt.c
 *
 * Copyright 2001-2002, Aaron Ardiri     (mailto:aaron@ardiri.com)
 *                      Charles Kerchner (mailto:chip@ardiri.com)
 * All rights reserved.
 *
 * This file was generated as part of the  "lemmings" program developed 
 * for the Palm Computing Platform designed by Palm: 
 *
 *   http://www.palm.com/ 
 *
 * The contents of this file is confidential and proprietrary in nature 
 * ("Confidential Information"). Redistribution or modification without 
 * prior consent of the original author is prohibited. 
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
  printf("EnCrYpT v1.0\n");
  printf("  Copyright 2000-2002 Aaron Ardiri (aaron_ardiri@mobilewizardry.com)\n\n");

  // called correctly?
  if (argc > 2) {

    FILE           *file;
    unsigned char  key;
    unsigned char  regi[0x7FFF] = {}; // 32K of memory
    unsigned char  data[0x7FFF] = {}; // 32K of memory
    unsigned short i, j, index, regiLength, dataLength;
    unsigned short *ptr;
  
    // process all data files passed in
    for (i=1; i<argc; i+=2) {

      // read in the "encryption" key
      file       = fopen(argv[i],"rb");
      dataLength = fread(data,1,0x7fff,file);
      fclose(file);                           // MUST do, as it changes

      // read in the data to encrypt
      file       = fopen(argv[i+1],"rb");
      regiLength = fread(regi,1,0x7fff,file);
      fclose(file);
 
      // do the "encryption" (as would be done before execution)
      ptr = (unsigned short *)data;
      for (j=0; j<dataLength; j+=2) {
        if (*ptr == 0x784e) 
	  *ptr = 0x714e;                     // INTEL - byte ordering :P
                                             //         0x4e78 -> 0x4e71
        ptr++;
      }

      // starting key = checksum
      key = 0;
      for (j=0; j<dataLength; j++) {
        key ^= data[j];
      }

      // encrypt the chunk
      index = key;
      for (j=0; j<regiLength; j++) {

        // adjust the byte
        regi[j] ^= key;

        // dynamically update the key
        do {
          index = (index + key + 1) % dataLength;
          key   = data[index];
        } while (key == 0);
      }

      // write the new code into file
      file = fopen(argv[i+1],"wb");
      fwrite(regi,1,regiLength,file);
      fclose(file);

      printf("  processed %s (%d bytes)\n", argv[i+1], regiLength);
    }
    printf("\n");

    exit(0);
  }
  else {
    printf("USAGE:\n");
    printf("  encrypt keyfile { file1 file2 ... fileN }\n\n");

    exit(1);
  }
}
