#
# @(#)Makefile
#
# Copyright 2001, Aaron Ardiri     (mailto:aaron@ardiri.com)
#                 Charles Kerchner (mailto:chip@ardiri.com)
# All rights reserved.
# 
# The contents of this file is confidential and proprietrary in nature 
# ("Confidential Information"). Redistribution or modification without 
# prior consent of the original author(s) is prohibited.
#

# compile requirements
all:
	make level1
	make level2
	make level3
	make level4
	make level5
	make level6
	make level7
	make level8
	make level9
	make level10

level1:
	bmp2bin level-01-data.bmp level-01-mask.bmp ../level-01.pal
	bmp2raw level-01-data.bmp level-01-mask.bmp ../level-01.pal
	raw2cmp level-01-data.raw level-01-data.cmp -gfx  -lz77
	raw2cmp level-01-mask.raw level-01-mask.cmp -mask -huff
	cp level-01*.bin ..
	cp level-01*.cmp ..

level2:
	bmp2bin level-02-data.bmp level-02-mask.bmp ../level-02.pal
	bmp2raw level-02-data.bmp level-02-mask.bmp ../level-02.pal
	raw2cmp level-02-data.raw level-02-data.cmp -gfx  -lz77
	raw2cmp level-02-mask.raw level-02-mask.cmp -mask -huff
	cp level-02*.bin ..
	cp level-02*.cmp ..

level3:
	bmp2bin level-03-data.bmp level-03-mask.bmp ../level-03.pal
	bmp2raw level-03-data.bmp level-03-mask.bmp ../level-03.pal
	raw2cmp level-03-data.raw level-03-data.cmp -gfx  -lz77
	raw2cmp level-03-mask.raw level-03-mask.cmp -mask -huff
	cp level-03*.bin ..
	cp level-03*.cmp ..

level4:
	bmp2bin level-04-data.bmp level-04-mask.bmp ../level-04.pal
	bmp2raw level-04-data.bmp level-04-mask.bmp ../level-04.pal
	raw2cmp level-04-data.raw level-04-data.cmp -gfx  -lz77
	raw2cmp level-04-mask.raw level-04-mask.cmp -mask -huff
	cp level-04*.bin ..
	cp level-04*.cmp ..

level5:
	bmp2bin level-05-data.bmp level-05-mask.bmp ../level-05.pal
	bmp2raw level-05-data.bmp level-05-mask.bmp ../level-05.pal
	raw2cmp level-05-data.raw level-05-data.cmp -gfx  -lz77
	raw2cmp level-05-mask.raw level-05-mask.cmp -mask -huff
	cp level-05*.bin ..
	cp level-05*.cmp ..

level6:
	bmp2bin level-06-data.bmp level-06-mask.bmp ../level-06.pal
	bmp2raw level-06-data.bmp level-06-mask.bmp ../level-06.pal
	raw2cmp level-06-data.raw level-06-data.cmp -gfx  -lz77
	raw2cmp level-06-mask.raw level-06-mask.cmp -mask -huff
	cp level-06*.bin ..
	cp level-06*.cmp ..

level7:
	bmp2bin level-07-data.bmp level-07-mask.bmp ../level-07.pal
	bmp2raw level-07-data.bmp level-07-mask.bmp ../level-07.pal
	raw2cmp level-07-data.raw level-07-data.cmp -gfx  -lz77
	raw2cmp level-07-mask.raw level-07-mask.cmp -mask -huff
	cp level-07*.bin ..
	cp level-07*.cmp ..

level8:
	bmp2bin level-08-data.bmp level-08-mask.bmp ../level-08.pal
	bmp2raw level-08-data.bmp level-08-mask.bmp ../level-08.pal
	raw2cmp level-08-data.raw level-08-data.cmp -gfx  -lz77
	raw2cmp level-08-mask.raw level-08-mask.cmp -mask -huff
	cp level-08*.bin ..
	cp level-08*.cmp ..

level9:
	bmp2bin level-09-data.bmp level-09-mask.bmp ../level-09.pal
	bmp2raw level-09-data.bmp level-09-mask.bmp ../level-09.pal
	raw2cmp level-09-data.raw level-09-data.cmp -gfx  -lz77
	raw2cmp level-09-mask.raw level-09-mask.cmp -mask -huff
	cp level-09*.bin ..
	cp level-09*.cmp ..

level10:
	bmp2bin level-10-data.bmp level-10-mask.bmp ../level-10.pal
	bmp2raw level-10-data.bmp level-10-mask.bmp ../level-10.pal
	raw2cmp level-10-data.raw level-10-data.cmp -gfx  -lz77
	raw2cmp level-10-mask.raw level-10-mask.cmp -mask -huff
	cp level-10*.bin ..
	cp level-10*.cmp ..

# clean-up funtions
clean:
	rm *.bin *.cmp *.raw
