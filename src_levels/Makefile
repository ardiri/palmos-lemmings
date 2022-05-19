#
# @(#)Makefile
#
# Copyright 2001-2002, Aaron Ardiri     (mailto:aaron@ardiri.com)
#                      Charles Kerchner (mailto:chip@ardiri.com)
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
PILRC    = $(PREFIX)pilrc

# source / outputs
BINFILE = lemmings
EXEC    = $(BINFILE)_$(LANG)

# compile requirements
all:
#	LANG=en PACK=GENESIS        make lemmings_level   # shipping already :P
#	LANG=en PACK=XMAS           make lemmings_level   # shipping already :P
#	LANG=en PACK=INFINITY       make lemmings_level   # shipping already :P
#	LANG=en PACK=NEWAGE         make lemmings_level   # shipping already :P
#	LANG=en PACK=CLASSIC_FUN    make lemmings_level   # shipping already :P
#	LANG=en PACK=CLASSIC_TRICKY make lemmings_level   # shipping already :P
#	LANG=en PACK=CLASSIC_TAXING make lemmings_level   # shipping already :P
#	LANG=en PACK=CALENDAR       make lemmings_level   # shipping already :P
#	LANG=en PACK=CLASSIC_MAYHEM make lemmings_level   # shipping already :P

#	LANG=en PACK=OHNO_TAME      make lemmings_level

lemmings_level: bin.levl.stamp
	cp $(EXEC)_$(PACK).pdb ..

bin.levl.stamp: $(EXEC)_$(PACK).rcp
	$(PILRC) -q -ro -creator LEMM -type levl -name "LEMM_$(PACK)" \
	$(EXEC)_$(PACK).rcp $(EXEC)_$(PACK).pdb

# clean-up funtions
clean:
	rm -f *.pdb

distclean:
	make clean
