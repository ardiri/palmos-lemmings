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

# compiler commands 
PREFIX   = 
CC       = $(PREFIX)gcc

# source / outputs
SRCS     = bmp2$(OUT).c
EXEC     = bmp2$(OUT)

# compiler flags 
CCFLAGS = -Wall 

# compile requirements
all:
	OUT=raw make bmp2XXX
	OUT=bin make bmp2XXX
	strip *.exe
	cp *.exe ..

bmp2XXX:
	$(CC) $(CCFLAGS) -o $(EXEC) $(SRCS)

# clean-up funtions
clean:
	rm -f *.[oa] *~

distclean: 
	rm -f *.[oa] *~ $(EXEC)
