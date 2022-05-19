#
# @(#)Makefile
#
# Copyright 2003, Aaron Ardiri     (mailto:aaron@ardiri.com)
#                 Charles Kerchner (mailto:chip@ardiri.com)
# All rights reserved.
# 
# The contents of this file is confidential and proprietrary in nature 
# ("Confidential Information"). Redistribution or modification without 
# prior consent of the original author(s) is prohibited.
#

# compile requirements
all:
	./bmp2bin level-01-data.bmp level-01-mask.bmp ../level-01.pal
	./bmp2bin level-02-data.bmp level-02-mask.bmp ../level-02.pal
	./bmp2bin level-03-data.bmp level-03-mask.bmp ../level-03.pal
	./bmp2bin level-04-data.bmp level-04-mask.bmp ../level-04.pal
	./bmp2bin level-05-data.bmp level-05-mask.bmp ../level-05.pal
	./bmp2bin level-06-data.bmp level-06-mask.bmp ../level-06.pal
	./bmp2bin level-07-data.bmp level-07-mask.bmp ../level-07.pal
	./bmp2bin level-08-data.bmp level-08-mask.bmp ../level-08.pal
	./bmp2bin level-09-data.bmp level-09-mask.bmp ../level-09.pal
	./bmp2bin level-10-data.bmp level-10-mask.bmp ../level-10.pal
	./bmp2bin level-11-data.bmp level-11-mask.bmp ../level-11.pal
	./bmp2bin level-12-data.bmp level-12-mask.bmp ../level-12.pal
	cp *.bin ..

	./bmp2raw level-01-data.bmp level-01-mask.bmp ../level-01.pal
	./raw2cmp level-01-data.raw level-01-data.cmp -gfx  -huff
	./raw2cmp level-01-mask.raw level-01-mask.cmp -mask -huff
	./bmp2raw level-02-data.bmp level-02-mask.bmp ../level-02.pal
	./raw2cmp level-02-data.raw level-02-data.cmp -gfx  -huff
	./raw2cmp level-02-mask.raw level-02-mask.cmp -mask -huff
	./bmp2raw level-03-data.bmp level-03-mask.bmp ../level-03.pal
	./raw2cmp level-03-data.raw level-03-data.cmp -gfx  -huff
	./raw2cmp level-03-mask.raw level-03-mask.cmp -mask -huff
	./bmp2raw level-04-data.bmp level-04-mask.bmp ../level-04.pal
	./raw2cmp level-04-data.raw level-04-data.cmp -gfx  -huff
	./raw2cmp level-04-mask.raw level-04-mask.cmp -mask -huff
	./bmp2raw level-05-data.bmp level-05-mask.bmp ../level-05.pal
	./raw2cmp level-05-data.raw level-05-data.cmp -gfx  -huff
	./raw2cmp level-05-mask.raw level-05-mask.cmp -mask -huff
	./bmp2raw level-06-data.bmp level-06-mask.bmp ../level-06.pal
	./raw2cmp level-06-data.raw level-06-data.cmp -gfx  -huff
	./raw2cmp level-06-mask.raw level-06-mask.cmp -mask -huff
	./bmp2raw level-07-data.bmp level-07-mask.bmp ../level-07.pal
	./raw2cmp level-07-data.raw level-07-data.cmp -gfx  -huff
	./raw2cmp level-07-mask.raw level-07-mask.cmp -mask -huff
	./bmp2raw level-08-data.bmp level-08-mask.bmp ../level-08.pal
	./raw2cmp level-08-data.raw level-08-data.cmp -gfx  -huff
	./raw2cmp level-08-mask.raw level-08-mask.cmp -mask -huff
	./bmp2raw level-09-data.bmp level-09-mask.bmp ../level-09.pal
	./raw2cmp level-09-data.raw level-09-data.cmp -gfx  -huff
	./raw2cmp level-09-mask.raw level-09-mask.cmp -mask -huff
	./bmp2raw level-10-data.bmp level-10-mask.bmp ../level-10.pal
	./raw2cmp level-10-data.raw level-10-data.cmp -gfx  -huff
	./raw2cmp level-10-mask.raw level-10-mask.cmp -mask -huff
	./bmp2raw level-11-data.bmp level-11-mask.bmp ../level-11.pal
	./raw2cmp level-11-data.raw level-11-data.cmp -gfx  -huff
	./raw2cmp level-11-mask.raw level-11-mask.cmp -mask -huff
	./bmp2raw level-12-data.bmp level-12-mask.bmp ../level-12.pal
	./raw2cmp level-12-data.raw level-12-data.cmp -gfx  -huff
	./raw2cmp level-12-mask.raw level-12-mask.cmp -mask -huff

	cp *.cmp ..

# clean-up funtions
clean:
	rm *.bin *.cmp *.raw
