/*
 * @(#)music.c
 *
 * Copyright 2001, Aaron Ardiri     (mailto:aaron@ardiri.com)
 *                 Charles Kerchner (mailto:chip@ardiri.com)
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

//
// frequency reference table (MIDI notes)
//

static int freqNote[] = {
                            0,     8,     9,     9,    10,    10,    11,    12,
                           12,    13,    14,    15,    16,    17,    18,    19, 
                           20,    21,    23,    24,    25,    27,    29,    30,
                           32,    34,    36,    38,    41,    43,    46,    48,
                           51,    55,    58,    61,    65,    69,    73,    77,
                           82,    87,    92,    97,   103,   110,   116,   123,
                          130,   138,   146,   155,   164,   174,   184,   195,
                          207,   220,   233,   246,   261,   277,   293,   311,
                          329,   349,   369,   391,   415,   440,   466,   493,
                          523,   554,   587,   622,   659,   698,   739,   783,
                          830,   880,   932,   987,  1046,  1108,  1174,  1244,
                         1318,  1396,  1479,  1567,  1661,  1760,  1864,  1975,
                         2093,  2217,  2349,  2489,  2637,  2793,  2959,  3135,
                         3322,  3520,  3729,  3951,  4186,  4434,  4698,  4978,
                         5274,  5587,  5919,  6271,  6644,  7040,  7458,  7902,
                         8372,  8869,  9297,  9956, 10548, 11175, 11839, 12543
                        };

int 
main(int argc, char *argv[]) 
{
  int result = 0;

  // convert the music data into ardiri.com MUSIC format :)
  if (argc == 2)
  {
    FILE           *fp = NULL;
    unsigned short freq, time;
    unsigned char  buffer[32768] = { 0 };
    int            count;
    unsigned int   nt, dur;

    // open the output file
    fp = fopen(argv[1],"wb");

    // write out the music data
    //
    // single note = 32 bits, 0xDDDDFFFF where, DDDD = end time, FFFF = freq
    
    count    = 2;
    while (nt != -1)
    {
      scanf("%d %d\n", &nt, &dur);

      if ((dur != 0) && (dur != -1))
      {
        // get the frequency and time for the note
        freq     = freqNote[nt];
        time     = dur;

        // write the data (remember, endian) :P
        buffer[count++] = (freq & 0xff00) >> 8;
        buffer[count++] =  freq & 0x00ff;
        buffer[count++] = (time & 0xff00) >> 8;
        buffer[count++] =  time & 0x00ff;
      }
    }

    count -= 2;
    buffer[0] = ((count >> 2) & 0xff00) >> 8;
    buffer[1] =  (count >> 2) & 0x00ff;
    fwrite(buffer,1,count+2,fp);

    // close the output file
    fclose(fp);
  }
  else
    fprintf(stdout, "USAGE: music file.mus < file.dat\n");

  return result;
}
