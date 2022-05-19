/*
 * @(#)bmp2bin.c
 *
 * Copyright 2001, Aaron Ardiri     (mailto:aaron@ardiri.com)
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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define false (0)
#define true  (!0)

int
grayscale[4][3] =
{
  { 255, 255, 255 }, { 204, 204, 204 }, { 153, 153, 153 }, {   0,   0,   0 }
};

int
BMP_RGBToColorIndex(int r, int g, int b, int palette[][3], int paletteSize)
{
  int index, lowValue, i, *diffArray;

  // generate the color "differences" for all colors in the palette
  diffArray = (int *)malloc(paletteSize * sizeof(int));
  for (i = 0; i < paletteSize; i++)
  {
    diffArray[i] = ((palette[i][0] - r) * (palette[i][0] - r)) +
      ((palette[i][1] - g) * (palette[i][1] - g)) +
      ((palette[i][2] - b) * (palette[i][2] - b));
  }

  // find the palette index that has the smallest color "difference"
  index = paletteSize - 1;
  lowValue = diffArray[index];
  for (i = index - 1; i >= 0; i--)
  {
    if (diffArray[i] < lowValue)
    {
      lowValue = diffArray[i];
      index = i;
    }
  }

  // clean up
  free(diffArray);

  return index;
}

int
main(int argc, char *argv[])
{
  int result = 0;

  fprintf(stdout, "Lemmings Level Generator (bmp2lev)\n");
  fprintf(stdout, "  Copyright 2001 Aaron Ardiri\n\n");

  // called correctly?
  if (argc == 4) 
  {
    int  ok;
    FILE *fdata, *fmask, *fpalette, *fout;

    // initialize
    ok       = true;
    fdata    = NULL;
    fmask    = NULL;
    fpalette = NULL;

    // check parameters
    if (ok)
    {
      fdata    = fopen(argv[1], "rb"); 
      fmask    = fopen(argv[2], "rb"); 
      fpalette = fopen(argv[3], "r"); 
   
      if ((fdata == NULL) || (fmask == NULL) || (fpalette == NULL))
      {
        fprintf(stderr, "x - missing files\n");
        ok = false;
      }
    }

    // check input file sizes
    if (ok)
    {
      unsigned long sdata, smask;

      fseek(fdata, 0, SEEK_END); sdata = ftell(fdata); rewind(fdata);
      fseek(fmask, 0, SEEK_END); smask = ftell(fmask); rewind(fmask);

      if ((sdata != 245814) || (smask != 245814))
        ok = false;
    }

    // process :)
    if (ok)
    {
      unsigned char bdata[245760] = { 0 };
      unsigned char bmask[245760] = { 0 };

      unsigned char rdata[40960]  = { 0 }; // 16 color
      unsigned char rmask[20480]  = { 0 }; // grayscale
      unsigned char obuff[65536]  = { 0 }; 

      unsigned char data;
      int c, i, j, k, r, g, b, v, index;
      int palette[16][3];

      // load the pallete
      for (i=0; i<16; i++)
      {
        fscanf(fpalette, "%d %d %d\n", 
               &palette[i][0], &palette[i][1], &palette[i][2]);
      }
      
      // read in the files!
      fseek(fdata, 54, SEEK_SET); fread(bdata,1,245760,fdata);
      fseek(fmask, 54, SEEK_SET); fread(bmask,1,245760,fmask);

      // pixel @ (x,y) [r g b] offset is:
      //
      // i     = 245760 - (y * 1920) + (x * 3);
      // r,g,b = [i+2],[i+1],[i]

      // process the data (data) bitmap
      c     = 0;
      index = 0;
      for (j=0; j<128; j++)
      {
        index = 245760 - ((j+1) * 1920);

        for (i=0; i<320; i++)  // 640 / 2 = process 2 at a time
        {
          data = 0; 
          for (k=0; k<2; k++)
          {
            r = bdata[index + 2];
            g = bdata[index + 1];
            b = bdata[index + 0]; index += 3;
            v = BMP_RGBToColorIndex(r, g, b, palette, 16);
            data = (data << 4) | v; 
          }

          rdata[c++] = data;
        }
      }

      index = 0;
      for (i=0; i<c; i++)
      {
        // only encode 0xff (empty space)
        if (rdata[i] == 0xff) 
        {
          v = 1;
          while (((i+1) < c) && (v < 255) && (rdata[i+1] == 0xff)) 
          { 
            v++; i++; 
          }

          obuff[index++] = 0xff; 
          obuff[index++] = (v & 0xff); 
        }
        else
          obuff[index++] = rdata[i]; 
      }
      printf("data: <-- %s\n", argv[1]);
      printf("data: processed to %d bytes\n", index);
      sprintf(strstr(argv[1], ".")+1, "bin");
      fout = fopen(argv[1], "wb");
      fwrite(obuff,1,index,fout);
      fclose(fout);
      printf("data: --> %s\n", argv[1]);

      // process the mask bitmap
      c     = 0;
      index = 0;
      for (j=0; j<128; j++)
      {
        index = 245760 - ((j+1) * 1920);

        for (i=0; i<160; i++)  // 640 / 4 = process 4 at a time
        {
          data = 0; 
          for (k=0; k<4; k++)
          {
            r = bmask[index + 2];
            g = bmask[index + 1];
            b = bmask[index + 0]; index += 3;
            v = BMP_RGBToColorIndex(r, g, b, grayscale, 4);
            data = (data << 2) | v; 
          }

          rmask[c++] = data;
        }
      }

      index = 0;
      for (i=0; i<c; i++)
      {
        // only encode 0x00 (empty space)
        if (rmask[i] == 0x00) 
        {
          v = 1;
          while (((i+1) < c) && (v < 255) && (rmask[i+1] == 0x00)) 
          { 
            v++; i++; 
          }

          obuff[index++] = 0x00; 
          obuff[index++] = (v & 0xff); 
        }
        else
          obuff[index++] = rmask[i]; 
      }
      printf("mask: <-- %s\n", argv[2]);
      printf("mask: processed to %d bytes\n", index);
      sprintf(strstr(argv[2], ".")+1, "bin");
      fout = fopen(argv[2], "wb");
      fwrite(obuff,1,index,fout);
      fclose(fout);
      printf("mask: --> %s\n", argv[2]);
    }

    if (!ok)
      fprintf(stderr, "- [%s %s %s] bad param\n", 
                      argv[1], argv[2], argv[3]);

    if (fdata != NULL) fclose(fdata);
    if (fmask != NULL) fclose(fmask);
    if (fpalette != NULL) fclose(fpalette);
  }
  else 
  {
    fprintf(stderr, "USAGE:\n");
    fprintf(stderr, "  bmp2bin lev-data.bmp lev-mask.bmp lev.pal\n\n");

    result = 1;
  }

  return result;
}
