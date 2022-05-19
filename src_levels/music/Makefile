#
# @(#)Makefile
#
# Copyright 2001, Aaron Ardiri     (mailto:aaron@ardiri.com)
#                 Charles Kerchner (mailto:chip@ardiri.com)
# All rights reserved.
#
# This file was generated as part of the  "Lemmings" program developed 
# for the Palm Computing Platform designed by Palm: 
#
#   http://www.palm.com/ 
#
# The contents of this file is confidential and proprietrary in nature 
# ("Confidential Information"). Redistribution or modification without 
# prior consent of the original author is prohibited. 
#

# compiler commands 
PREFIX   = 
CC       = $(PREFIX)gcc

# source / outputs
SRCS     = music.c
EXEC     = music

# compiler flags 
CCFLAGS = -Wall 

# compile requirements
all:
	$(CC) $(CCFLAGS) -o $(EXEC) $(SRCS)
	strip $(EXEC).exe
	$(EXEC) lemm_cancan.mus     < lemm_cancan.dat
	$(EXEC) lemm_mountain.mus   < lemm_mountain.dat
	$(EXEC) lemm_tenbottles.mus < lemm_tenbottles.dat

	$(EXEC) lemm_adeste_fidelis.mus         < lemm_adeste_fidelis.dat
	$(EXEC) lemm_here_comes_santa_claus.mus < lemm_here_comes_santa_claus.dat
	$(EXEC) lemm_jingle_bells.mus           < lemm_jingle_bells.dat
	$(EXEC) lemm_noel.mus                   < lemm_noel.dat

# clean-up funtions
clean:
	rm -f *.[oa] *~

distclean: 
	rm -f *.[oa] *~ $(EXEC)
