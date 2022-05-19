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
	bmp2bin level-01-data.bmp level-01-mask.bmp ../palette-earth.pal
	bmp2bin level-02-data.bmp level-02-mask.bmp ../palette-golden.pal
	bmp2bin level-03-data.bmp level-03-mask.bmp ../palette-golden.pal
	bmp2bin level-04-data.bmp level-04-mask.bmp ../palette-pink.pal
	bmp2bin level-05-data.bmp level-05-mask.bmp ../palette-blue.pal
	bmp2bin level-06-data.bmp level-06-mask.bmp ../palette-hell.pal
	bmp2bin level-07-data.bmp level-07-mask.bmp ../palette-pink.pal
	bmp2bin level-08-data.bmp level-08-mask.bmp ../palette-golden.pal
	bmp2bin level-09-data.bmp level-09-mask.bmp ../palette-pink.pal
	bmp2bin level-10-data.bmp level-10-mask.bmp ../palette-golden.pal
	bmp2bin level-11-data.bmp level-11-mask.bmp ../palette-pink.pal
	bmp2bin level-12-data.bmp level-12-mask.bmp ../palette-golden.pal
	bmp2bin level-13-data.bmp level-13-mask.bmp ../palette-hell.pal
	bmp2bin level-14-data.bmp level-14-mask.bmp ../palette-golden.pal
	bmp2bin level-15-data.bmp level-15-mask.bmp ../palette-golden.pal
	bmp2bin level-16-data.bmp level-16-mask.bmp ../palette-hell.pal
	bmp2bin level-17-data.bmp level-17-mask.bmp ../palette-pink.pal
	bmp2bin level-18-data.bmp level-18-mask.bmp ../palette-hell.pal
	bmp2bin level-19-data.bmp level-19-mask.bmp ../palette-pink.pal
	bmp2bin level-20-data.bmp level-20-mask.bmp ../palette-pink.pal
	bmp2bin level-21-data.bmp level-21-mask.bmp ../palette-earth.pal
	bmp2bin level-22-data.bmp level-22-mask.bmp ../palette-trees.pal
	bmp2bin level-23-data.bmp level-23-mask.bmp ../palette-hell.pal
	bmp2bin level-24-data.bmp level-24-mask.bmp ../palette-pink.pal
	bmp2bin level-25-data.bmp level-25-mask.bmp ../palette-blue.pal
	bmp2bin level-26-data.bmp level-26-mask.bmp ../palette-golden.pal
	bmp2bin level-27-data.bmp level-27-mask.bmp ../palette-pink.pal
	bmp2bin level-28-data.bmp level-28-mask.bmp ../palette-hell.pal
	bmp2bin level-29-data.bmp level-29-mask.bmp ../palette-golden.pal
	bmp2bin level-30-data.bmp level-30-mask.bmp ../palette-hell.pal
	cp *.bin ..

	bmp2raw level-01-data.bmp level-01-mask.bmp ../palette-earth.pal
	raw2cmp level-01-data.raw level-01-data.cmp -gfx  -lz77
	raw2cmp level-01-mask.raw level-01-mask.cmp -mask -huff
	bmp2raw level-02-data.bmp level-02-mask.bmp ../palette-golden.pal
	raw2cmp level-02-data.raw level-02-data.cmp -gfx  -lz77
	raw2cmp level-02-mask.raw level-02-mask.cmp -mask -huff
	bmp2raw level-03-data.bmp level-03-mask.bmp ../palette-golden.pal
	raw2cmp level-03-data.raw level-03-data.cmp -gfx  -huff
	raw2cmp level-03-mask.raw level-03-mask.cmp -mask -huff
	bmp2raw level-04-data.bmp level-04-mask.bmp ../palette-pink.pal
	raw2cmp level-04-data.raw level-04-data.cmp -gfx  -lz77
	raw2cmp level-04-mask.raw level-04-mask.cmp -mask -huff
	bmp2raw level-05-data.bmp level-05-mask.bmp ../palette-blue.pal
	raw2cmp level-05-data.raw level-05-data.cmp -gfx  -lz77
	raw2cmp level-05-mask.raw level-05-mask.cmp -mask -lz77
	bmp2raw level-06-data.bmp level-06-mask.bmp ../palette-hell.pal
	raw2cmp level-06-data.raw level-06-data.cmp -gfx  -lz77
	raw2cmp level-06-mask.raw level-06-mask.cmp -mask -huff
	bmp2raw level-07-data.bmp level-07-mask.bmp ../palette-pink.pal
	raw2cmp level-07-data.raw level-07-data.cmp -gfx  -lz77
	raw2cmp level-07-mask.raw level-07-mask.cmp -mask -huff
	bmp2raw level-08-data.bmp level-08-mask.bmp ../palette-golden.pal
	raw2cmp level-08-data.raw level-08-data.cmp -gfx  -lz77
	raw2cmp level-08-mask.raw level-08-mask.cmp -mask -huff
	bmp2raw level-09-data.bmp level-09-mask.bmp ../palette-pink.pal
	raw2cmp level-09-data.raw level-09-data.cmp -gfx  -lz77
	raw2cmp level-09-mask.raw level-09-mask.cmp -mask -huff
	bmp2raw level-10-data.bmp level-10-mask.bmp ../palette-golden.pal
	raw2cmp level-10-data.raw level-10-data.cmp -gfx  -lz77
	raw2cmp level-10-mask.raw level-10-mask.cmp -mask -huff
	bmp2raw level-11-data.bmp level-11-mask.bmp ../palette-pink.pal
	raw2cmp level-11-data.raw level-11-data.cmp -gfx  -lz77
	raw2cmp level-11-mask.raw level-11-mask.cmp -mask -huff
	bmp2raw level-12-data.bmp level-12-mask.bmp ../palette-golden.pal
	raw2cmp level-12-data.raw level-12-data.cmp -gfx  -lz77
	raw2cmp level-12-mask.raw level-12-mask.cmp -mask -huff
	bmp2raw level-13-data.bmp level-13-mask.bmp ../palette-hell.pal
	raw2cmp level-13-data.raw level-13-data.cmp -gfx  -lz77
	raw2cmp level-13-mask.raw level-13-mask.cmp -mask -huff
	bmp2raw level-14-data.bmp level-14-mask.bmp ../palette-golden.pal
	raw2cmp level-14-data.raw level-14-data.cmp -gfx  -huff
	raw2cmp level-14-mask.raw level-14-mask.cmp -mask -huff
	bmp2raw level-15-data.bmp level-15-mask.bmp ../palette-golden.pal
	raw2cmp level-15-data.raw level-15-data.cmp -gfx  -lz77
	raw2cmp level-15-mask.raw level-15-mask.cmp -mask -huff
	bmp2raw level-16-data.bmp level-16-mask.bmp ../palette-hell.pal
	raw2cmp level-16-data.raw level-16-data.cmp -gfx  -huff
	raw2cmp level-16-mask.raw level-16-mask.cmp -mask -huff
	bmp2raw level-17-data.bmp level-17-mask.bmp ../palette-pink.pal
	raw2cmp level-17-data.raw level-17-data.cmp -gfx  -lz77
	raw2cmp level-17-mask.raw level-17-mask.cmp -mask -huff
	bmp2raw level-18-data.bmp level-18-mask.bmp ../palette-hell.pal
	raw2cmp level-18-data.raw level-18-data.cmp -gfx  -lz77
	raw2cmp level-18-mask.raw level-18-mask.cmp -mask -huff
	bmp2raw level-19-data.bmp level-19-mask.bmp ../palette-pink.pal
	raw2cmp level-19-data.raw level-19-data.cmp -gfx  -lz77
	raw2cmp level-19-mask.raw level-19-mask.cmp -mask -huff
	bmp2raw level-20-data.bmp level-20-mask.bmp ../palette-pink.pal
	raw2cmp level-20-data.raw level-20-data.cmp -gfx  -lz77
	raw2cmp level-20-mask.raw level-20-mask.cmp -mask -huff
	bmp2raw level-21-data.bmp level-21-mask.bmp ../palette-earth.pal
	raw2cmp level-21-data.raw level-21-data.cmp -gfx  -lz77
	raw2cmp level-21-mask.raw level-21-mask.cmp -mask -huff
	bmp2raw level-22-data.bmp level-22-mask.bmp ../palette-trees.pal
	raw2cmp level-22-data.raw level-22-data.cmp -gfx  -huff
	raw2cmp level-22-mask.raw level-22-mask.cmp -mask -huff
	bmp2raw level-23-data.bmp level-23-mask.bmp ../palette-hell.pal
	raw2cmp level-23-data.raw level-23-data.cmp -gfx  -lz77
	raw2cmp level-23-mask.raw level-23-mask.cmp -mask -lz77
	bmp2raw level-24-data.bmp level-24-mask.bmp ../palette-pink.pal
	raw2cmp level-24-data.raw level-24-data.cmp -gfx  -lz77
	raw2cmp level-24-mask.raw level-24-mask.cmp -mask -huff
	bmp2raw level-25-data.bmp level-25-mask.bmp ../palette-blue.pal
	raw2cmp level-25-data.raw level-25-data.cmp -gfx  -lz77
	raw2cmp level-25-mask.raw level-25-mask.cmp -mask -lz77
	bmp2raw level-26-data.bmp level-26-mask.bmp ../palette-golden.pal
	raw2cmp level-26-data.raw level-26-data.cmp -gfx  -lz77
	raw2cmp level-26-mask.raw level-26-mask.cmp -mask -huff
	bmp2raw level-27-data.bmp level-27-mask.bmp ../palette-pink.pal
	raw2cmp level-27-data.raw level-27-data.cmp -gfx  -lz77
	raw2cmp level-27-mask.raw level-27-mask.cmp -mask -huff
	bmp2raw level-28-data.bmp level-28-mask.bmp ../palette-hell.pal
	raw2cmp level-28-data.raw level-28-data.cmp -gfx  -lz77
	raw2cmp level-28-mask.raw level-28-mask.cmp -mask -huff
	bmp2raw level-29-data.bmp level-29-mask.bmp ../palette-golden.pal
	raw2cmp level-29-data.raw level-29-data.cmp -gfx  -lz77
	raw2cmp level-29-mask.raw level-29-mask.cmp -mask -huff
	bmp2raw level-30-data.bmp level-30-mask.bmp ../palette-hell.pal
	raw2cmp level-30-data.raw level-30-data.cmp -gfx  -lz77
	raw2cmp level-30-mask.raw level-30-mask.cmp -mask -huff

	cp *.cmp ..

cmp_report:

	bmp2raw level-01-data.bmp level-01-mask.bmp ../palette-earth.pal
	raw2cmp level-01-data.raw level-01-data.cmp -gfx  -lz77
	mv level-01-data.cmp level-01-data-lz77.cmp
	raw2cmp level-01-data.raw level-01-data.cmp -gfx  -huff
	mv level-01-data.cmp level-01-data-huff.cmp
	raw2cmp level-01-mask.raw level-01-mask.cmp -mask -lz77
	mv level-01-mask.cmp level-01-mask-lz77.cmp
	raw2cmp level-01-mask.raw level-01-mask.cmp -mask -huff
	mv level-01-mask.cmp level-01-mask-huff.cmp

	bmp2raw level-02-data.bmp level-02-mask.bmp ../palette-golden.pal
	raw2cmp level-02-data.raw level-02-data.cmp -gfx  -lz77
	mv level-02-data.cmp level-02-data-lz77.cmp
	raw2cmp level-02-data.raw level-02-data.cmp -gfx  -huff
	mv level-02-data.cmp level-02-data-huff.cmp
	raw2cmp level-02-mask.raw level-02-mask.cmp -mask -lz77
	mv level-02-mask.cmp level-02-mask-lz77.cmp
	raw2cmp level-02-mask.raw level-02-mask.cmp -mask -huff
	mv level-02-mask.cmp level-02-mask-huff.cmp

	bmp2raw level-03-data.bmp level-03-mask.bmp ../palette-golden.pal
	raw2cmp level-03-data.raw level-03-data.cmp -gfx  -lz77
	mv level-03-data.cmp level-03-data-lz77.cmp
	raw2cmp level-03-data.raw level-03-data.cmp -gfx  -huff
	mv level-03-data.cmp level-03-data-huff.cmp
	raw2cmp level-03-mask.raw level-03-mask.cmp -mask -lz77
	mv level-03-mask.cmp level-03-mask-lz77.cmp
	raw2cmp level-03-mask.raw level-03-mask.cmp -mask -huff
	mv level-03-mask.cmp level-03-mask-huff.cmp

	bmp2raw level-04-data.bmp level-04-mask.bmp ../palette-pink.pal
	raw2cmp level-04-data.raw level-04-data.cmp -gfx  -lz77
	mv level-04-data.cmp level-04-data-lz77.cmp
	raw2cmp level-04-data.raw level-04-data.cmp -gfx  -huff
	mv level-04-data.cmp level-04-data-huff.cmp
	raw2cmp level-04-mask.raw level-04-mask.cmp -mask -lz77
	mv level-04-mask.cmp level-04-mask-lz77.cmp
	raw2cmp level-04-mask.raw level-04-mask.cmp -mask -huff
	mv level-04-mask.cmp level-04-mask-huff.cmp

	bmp2raw level-05-data.bmp level-05-mask.bmp ../palette-blue.pal
	raw2cmp level-05-data.raw level-05-data.cmp -gfx  -lz77
	mv level-05-data.cmp level-05-data-lz77.cmp
	raw2cmp level-05-data.raw level-05-data.cmp -gfx  -huff
	mv level-05-data.cmp level-05-data-huff.cmp
	raw2cmp level-05-mask.raw level-05-mask.cmp -mask -lz77
	mv level-05-mask.cmp level-05-mask-lz77.cmp
	raw2cmp level-05-mask.raw level-05-mask.cmp -mask -huff
	mv level-05-mask.cmp level-05-mask-huff.cmp

	bmp2raw level-06-data.bmp level-06-mask.bmp ../palette-hell.pal
	raw2cmp level-06-data.raw level-06-data.cmp -gfx  -lz77
	mv level-06-data.cmp level-06-data-lz77.cmp
	raw2cmp level-06-data.raw level-06-data.cmp -gfx  -huff
	mv level-06-data.cmp level-06-data-huff.cmp
	raw2cmp level-06-mask.raw level-06-mask.cmp -mask -lz77
	mv level-06-mask.cmp level-06-mask-lz77.cmp
	raw2cmp level-06-mask.raw level-06-mask.cmp -mask -huff
	mv level-06-mask.cmp level-06-mask-huff.cmp

	bmp2raw level-07-data.bmp level-07-mask.bmp ../palette-pink.pal
	raw2cmp level-07-data.raw level-07-data.cmp -gfx  -lz77
	mv level-07-data.cmp level-07-data-lz77.cmp
	raw2cmp level-07-data.raw level-07-data.cmp -gfx  -huff
	mv level-07-data.cmp level-07-data-huff.cmp
	raw2cmp level-07-mask.raw level-07-mask.cmp -mask -lz77
	mv level-07-mask.cmp level-07-mask-lz77.cmp
	raw2cmp level-07-mask.raw level-07-mask.cmp -mask -huff
	mv level-07-mask.cmp level-07-mask-huff.cmp

	bmp2raw level-08-data.bmp level-08-mask.bmp ../palette-golden.pal
	raw2cmp level-08-data.raw level-08-data.cmp -gfx  -lz77
	mv level-08-data.cmp level-08-data-lz77.cmp
	raw2cmp level-08-data.raw level-08-data.cmp -gfx  -huff
	mv level-08-data.cmp level-08-data-huff.cmp
	raw2cmp level-08-mask.raw level-08-mask.cmp -mask -lz77
	mv level-08-mask.cmp level-08-mask-lz77.cmp
	raw2cmp level-08-mask.raw level-08-mask.cmp -mask -huff
	mv level-08-mask.cmp level-08-mask-huff.cmp

	bmp2raw level-09-data.bmp level-09-mask.bmp ../palette-pink.pal
	raw2cmp level-09-data.raw level-09-data.cmp -gfx  -lz77
	mv level-09-data.cmp level-09-data-lz77.cmp
	raw2cmp level-09-data.raw level-09-data.cmp -gfx  -huff
	mv level-09-data.cmp level-09-data-huff.cmp
	raw2cmp level-09-mask.raw level-09-mask.cmp -mask -lz77
	mv level-09-mask.cmp level-09-mask-lz77.cmp
	raw2cmp level-09-mask.raw level-09-mask.cmp -mask -huff
	mv level-09-mask.cmp level-09-mask-huff.cmp

	bmp2raw level-10-data.bmp level-10-mask.bmp ../palette-golden.pal
	raw2cmp level-10-data.raw level-10-data.cmp -gfx  -lz77
	mv level-10-data.cmp level-10-data-lz77.cmp
	raw2cmp level-10-data.raw level-10-data.cmp -gfx  -huff
	mv level-10-data.cmp level-10-data-huff.cmp
	raw2cmp level-10-mask.raw level-10-mask.cmp -mask -lz77
	mv level-10-mask.cmp level-10-mask-lz77.cmp
	raw2cmp level-10-mask.raw level-10-mask.cmp -mask -huff
	mv level-10-mask.cmp level-10-mask-huff.cmp

	bmp2raw level-11-data.bmp level-11-mask.bmp ../palette-pink.pal
	raw2cmp level-11-data.raw level-11-data.cmp -gfx  -lz77
	mv level-11-data.cmp level-11-data-lz77.cmp
	raw2cmp level-11-data.raw level-11-data.cmp -gfx  -huff
	mv level-11-data.cmp level-11-data-huff.cmp
	raw2cmp level-11-mask.raw level-11-mask.cmp -mask -lz77
	mv level-11-mask.cmp level-11-mask-lz77.cmp
	raw2cmp level-11-mask.raw level-11-mask.cmp -mask -huff
	mv level-11-mask.cmp level-11-mask-huff.cmp

	bmp2raw level-12-data.bmp level-12-mask.bmp ../palette-golden.pal
	raw2cmp level-12-data.raw level-12-data.cmp -gfx  -lz77
	mv level-12-data.cmp level-12-data-lz77.cmp
	raw2cmp level-12-data.raw level-12-data.cmp -gfx  -huff
	mv level-12-data.cmp level-12-data-huff.cmp
	raw2cmp level-12-mask.raw level-12-mask.cmp -mask -lz77
	mv level-12-mask.cmp level-12-mask-lz77.cmp
	raw2cmp level-12-mask.raw level-12-mask.cmp -mask -huff
	mv level-12-mask.cmp level-12-mask-huff.cmp

	bmp2raw level-13-data.bmp level-13-mask.bmp ../palette-hell.pal
	raw2cmp level-13-data.raw level-13-data.cmp -gfx  -lz77
	mv level-13-data.cmp level-13-data-lz77.cmp
	raw2cmp level-13-data.raw level-13-data.cmp -gfx  -huff
	mv level-13-data.cmp level-13-data-huff.cmp
	raw2cmp level-13-mask.raw level-13-mask.cmp -mask -lz77
	mv level-13-mask.cmp level-13-mask-lz77.cmp
	raw2cmp level-13-mask.raw level-13-mask.cmp -mask -huff
	mv level-13-mask.cmp level-13-mask-huff.cmp

	bmp2raw level-14-data.bmp level-14-mask.bmp ../palette-golden.pal
	raw2cmp level-14-data.raw level-14-data.cmp -gfx  -lz77
	mv level-14-data.cmp level-14-data-lz77.cmp
	raw2cmp level-14-data.raw level-14-data.cmp -gfx  -huff
	mv level-14-data.cmp level-14-data-huff.cmp
	raw2cmp level-14-mask.raw level-14-mask.cmp -mask -lz77
	mv level-14-mask.cmp level-14-mask-lz77.cmp
	raw2cmp level-14-mask.raw level-14-mask.cmp -mask -huff
	mv level-14-mask.cmp level-14-mask-huff.cmp

	bmp2raw level-15-data.bmp level-15-mask.bmp ../palette-golden.pal
	raw2cmp level-15-data.raw level-15-data.cmp -gfx  -lz77
	mv level-15-data.cmp level-15-data-lz77.cmp
	raw2cmp level-15-data.raw level-15-data.cmp -gfx  -huff
	mv level-15-data.cmp level-15-data-huff.cmp
	raw2cmp level-15-mask.raw level-15-mask.cmp -mask -lz77
	mv level-15-mask.cmp level-15-mask-lz77.cmp
	raw2cmp level-15-mask.raw level-15-mask.cmp -mask -huff
	mv level-15-mask.cmp level-15-mask-huff.cmp

	bmp2raw level-16-data.bmp level-16-mask.bmp ../palette-hell.pal
	raw2cmp level-16-data.raw level-16-data.cmp -gfx  -lz77
	mv level-16-data.cmp level-16-data-lz77.cmp
	raw2cmp level-16-data.raw level-16-data.cmp -gfx  -huff
	mv level-16-data.cmp level-16-data-huff.cmp
	raw2cmp level-16-mask.raw level-16-mask.cmp -mask -lz77
	mv level-16-mask.cmp level-16-mask-lz77.cmp
	raw2cmp level-16-mask.raw level-16-mask.cmp -mask -huff
	mv level-16-mask.cmp level-16-mask-huff.cmp

	bmp2raw level-17-data.bmp level-17-mask.bmp ../palette-pink.pal
	raw2cmp level-17-data.raw level-17-data.cmp -gfx  -lz77
	mv level-17-data.cmp level-17-data-lz77.cmp
	raw2cmp level-17-data.raw level-17-data.cmp -gfx  -huff
	mv level-17-data.cmp level-17-data-huff.cmp
	raw2cmp level-17-mask.raw level-17-mask.cmp -mask -lz77
	mv level-17-mask.cmp level-17-mask-lz77.cmp
	raw2cmp level-17-mask.raw level-17-mask.cmp -mask -huff
	mv level-17-mask.cmp level-17-mask-huff.cmp

	bmp2raw level-18-data.bmp level-18-mask.bmp ../palette-hell.pal
	raw2cmp level-18-data.raw level-18-data.cmp -gfx  -lz77
	mv level-18-data.cmp level-18-data-lz77.cmp
	raw2cmp level-18-data.raw level-18-data.cmp -gfx  -huff
	mv level-18-data.cmp level-18-data-huff.cmp
	raw2cmp level-18-mask.raw level-18-mask.cmp -mask -lz77
	mv level-18-mask.cmp level-18-mask-lz77.cmp
	raw2cmp level-18-mask.raw level-18-mask.cmp -mask -huff
	mv level-18-mask.cmp level-18-mask-huff.cmp

	bmp2raw level-19-data.bmp level-19-mask.bmp ../palette-pink.pal
	raw2cmp level-19-data.raw level-19-data.cmp -gfx  -lz77
	mv level-19-data.cmp level-19-data-lz77.cmp
	raw2cmp level-19-data.raw level-19-data.cmp -gfx  -huff
	mv level-19-data.cmp level-19-data-huff.cmp
	raw2cmp level-19-mask.raw level-19-mask.cmp -mask -lz77
	mv level-19-mask.cmp level-19-mask-lz77.cmp
	raw2cmp level-19-mask.raw level-19-mask.cmp -mask -huff
	mv level-19-mask.cmp level-19-mask-huff.cmp

	bmp2raw level-20-data.bmp level-20-mask.bmp ../palette-pink.pal
	raw2cmp level-20-data.raw level-20-data.cmp -gfx  -lz77
	mv level-20-data.cmp level-20-data-lz77.cmp
	raw2cmp level-20-data.raw level-20-data.cmp -gfx  -huff
	mv level-20-data.cmp level-20-data-huff.cmp
	raw2cmp level-20-mask.raw level-20-mask.cmp -mask -lz77
	mv level-20-mask.cmp level-20-mask-lz77.cmp
	raw2cmp level-20-mask.raw level-20-mask.cmp -mask -huff
	mv level-20-mask.cmp level-20-mask-huff.cmp

	bmp2raw level-21-data.bmp level-21-mask.bmp ../palette-earth.pal
	raw2cmp level-21-data.raw level-21-data.cmp -gfx  -lz77
	mv level-21-data.cmp level-21-data-lz77.cmp
	raw2cmp level-21-data.raw level-21-data.cmp -gfx  -huff
	mv level-21-data.cmp level-21-data-huff.cmp
	raw2cmp level-21-mask.raw level-21-mask.cmp -mask -lz77
	mv level-21-mask.cmp level-21-mask-lz77.cmp
	raw2cmp level-21-mask.raw level-21-mask.cmp -mask -huff
	mv level-21-mask.cmp level-21-mask-huff.cmp

	bmp2raw level-22-data.bmp level-22-mask.bmp ../palette-trees.pal
	raw2cmp level-22-data.raw level-22-data.cmp -gfx  -lz77
	mv level-22-data.cmp level-22-data-lz77.cmp
	raw2cmp level-22-data.raw level-22-data.cmp -gfx  -huff
	mv level-22-data.cmp level-22-data-huff.cmp
	raw2cmp level-22-mask.raw level-22-mask.cmp -mask -lz77
	mv level-22-mask.cmp level-22-mask-lz77.cmp
	raw2cmp level-22-mask.raw level-22-mask.cmp -mask -huff
	mv level-22-mask.cmp level-22-mask-huff.cmp

	bmp2raw level-23-data.bmp level-23-mask.bmp ../palette-hell.pal
	raw2cmp level-23-data.raw level-23-data.cmp -gfx  -lz77
	mv level-23-data.cmp level-23-data-lz77.cmp
	raw2cmp level-23-data.raw level-23-data.cmp -gfx  -huff
	mv level-23-data.cmp level-23-data-huff.cmp
	raw2cmp level-23-mask.raw level-23-mask.cmp -mask -lz77
	mv level-23-mask.cmp level-23-mask-lz77.cmp
	raw2cmp level-23-mask.raw level-23-mask.cmp -mask -huff
	mv level-23-mask.cmp level-23-mask-huff.cmp

	bmp2raw level-24-data.bmp level-24-mask.bmp ../palette-pink.pal
	raw2cmp level-24-data.raw level-24-data.cmp -gfx  -lz77
	mv level-24-data.cmp level-24-data-lz77.cmp
	raw2cmp level-24-data.raw level-24-data.cmp -gfx  -huff
	mv level-24-data.cmp level-24-data-huff.cmp
	raw2cmp level-24-mask.raw level-24-mask.cmp -mask -lz77
	mv level-24-mask.cmp level-24-mask-lz77.cmp
	raw2cmp level-24-mask.raw level-24-mask.cmp -mask -huff
	mv level-24-mask.cmp level-24-mask-huff.cmp

	bmp2raw level-25-data.bmp level-25-mask.bmp ../palette-blue.pal
	raw2cmp level-25-data.raw level-25-data.cmp -gfx  -lz77
	mv level-25-data.cmp level-25-data-lz77.cmp
	raw2cmp level-25-data.raw level-25-data.cmp -gfx  -huff
	mv level-25-data.cmp level-25-data-huff.cmp
	raw2cmp level-25-mask.raw level-25-mask.cmp -mask -lz77
	mv level-25-mask.cmp level-25-mask-lz77.cmp
	raw2cmp level-25-mask.raw level-25-mask.cmp -mask -huff
	mv level-25-mask.cmp level-25-mask-huff.cmp

	bmp2raw level-26-data.bmp level-26-mask.bmp ../palette-golden.pal
	raw2cmp level-26-data.raw level-26-data.cmp -gfx  -lz77
	mv level-26-data.cmp level-26-data-lz77.cmp
	raw2cmp level-26-data.raw level-26-data.cmp -gfx  -huff
	mv level-26-data.cmp level-26-data-huff.cmp
	raw2cmp level-26-mask.raw level-26-mask.cmp -mask -lz77
	mv level-26-mask.cmp level-26-mask-lz77.cmp
	raw2cmp level-26-mask.raw level-26-mask.cmp -mask -huff
	mv level-26-mask.cmp level-26-mask-huff.cmp

	bmp2raw level-27-data.bmp level-27-mask.bmp ../palette-pink.pal
	raw2cmp level-27-data.raw level-27-data.cmp -gfx  -lz77
	mv level-27-data.cmp level-27-data-lz77.cmp
	raw2cmp level-27-data.raw level-27-data.cmp -gfx  -huff
	mv level-27-data.cmp level-27-data-huff.cmp
	raw2cmp level-27-mask.raw level-27-mask.cmp -mask -lz77
	mv level-27-mask.cmp level-27-mask-lz77.cmp
	raw2cmp level-27-mask.raw level-27-mask.cmp -mask -huff
	mv level-27-mask.cmp level-27-mask-huff.cmp

	bmp2raw level-28-data.bmp level-28-mask.bmp ../palette-hell.pal
	raw2cmp level-28-data.raw level-28-data.cmp -gfx  -lz77
	mv level-28-data.cmp level-28-data-lz77.cmp
	raw2cmp level-28-data.raw level-28-data.cmp -gfx  -huff
	mv level-28-data.cmp level-28-data-huff.cmp
	raw2cmp level-28-mask.raw level-28-mask.cmp -mask -lz77
	mv level-28-mask.cmp level-28-mask-lz77.cmp
	raw2cmp level-28-mask.raw level-28-mask.cmp -mask -huff
	mv level-28-mask.cmp level-28-mask-huff.cmp

	bmp2raw level-29-data.bmp level-29-mask.bmp ../palette-golden.pal
	raw2cmp level-29-data.raw level-29-data.cmp -gfx  -lz77
	mv level-29-data.cmp level-29-data-lz77.cmp
	raw2cmp level-29-data.raw level-29-data.cmp -gfx  -huff
	mv level-29-data.cmp level-29-data-huff.cmp
	raw2cmp level-29-mask.raw level-29-mask.cmp -mask -lz77
	mv level-29-mask.cmp level-29-mask-lz77.cmp
	raw2cmp level-29-mask.raw level-29-mask.cmp -mask -huff
	mv level-29-mask.cmp level-29-mask-huff.cmp

	bmp2raw level-30-data.bmp level-30-mask.bmp ../palette-hell.pal
	raw2cmp level-30-data.raw level-30-data.cmp -gfx  -lz77
	mv level-30-data.cmp level-30-data-lz77.cmp
	raw2cmp level-30-data.raw level-30-data.cmp -gfx  -huff
	mv level-30-data.cmp level-30-data-huff.cmp
	raw2cmp level-30-mask.raw level-30-mask.cmp -mask -lz77
	mv level-30-mask.cmp level-30-mask-lz77.cmp
	raw2cmp level-30-mask.raw level-30-mask.cmp -mask -huff
	mv level-30-mask.cmp level-30-mask-huff.cmp

	ls -l *.cmp > level-cmp_sizes.txt
	rm *.cmp
	
# clean-up funtions
clean:
	rm *.bin *.cmp *.raw
