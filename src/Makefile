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
CC       = $(PREFIX)m68k-palmos-gcc
PILRC    = $(PREFIX)pilrc
BUILDPRC = $(PREFIX)build-prc
PRC2BIN  = $(PREFIX)prc2bin
ENCRYPT  = regcode/encrypt

# source / outputs
BINFILE = lemmings
EXEC    = $(BINFILE)_$(LANG)
MSEG    = $(EXEC)-sections.o $(EXEC)-sections.ld
OBJS    = palm.o device.o graphics.o game.o help_$(LANG).o register.o levelpack.o $(MSEG)
TEMP    = tmp.out

# compiler flags
CCFLAGS = -O2 -Wall -palmos4 -DLANG_$(LANG)
ACFLAGS = -mshort-load-bytes -ffixed-r9 -nostartfiles -O2 -Wall \
	  -Xlinker --script=callback.ld

# compile requirements
all:
	LANG=en make lemmings

lemmings: $(EXEC) $(EXEC).def arm.midi.stamp arm.sfx.stamp bin.game.stamp
	$(BUILDPRC) $(EXEC).def $(BINFILE) *.bin
	$(PRC2BIN) $(EXEC).prc > $(TEMP)
	$(ENCRYPT) code0002.bin code0006.bin
	$(BUILDPRC) $(EXEC).def *.bin
	make clean
	cp $(EXEC).prc ../$(EXEC).prc

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(CCFLAGS) -o $(BINFILE)

$(MSEG): $(EXEC).def
#	multigen $(EXEC).def                     # pre prc-tools 2.2
	m68k-palmos-multigen $(EXEC).def
	$(CC) -c -o $(EXEC)-sections.o $(EXEC)-sections.s

arm.sfx.stamp: sfx-callback.c
	arm-elf-gcc -e callback -o sfx-callback.o $(ACFLAGS) sfx-callback.c
	arm-elf-objcopy -j .text -O binary sfx-callback.o sfx-callback.arm

arm.midi.stamp: midi-callback.c
	arm-elf-gcc -e callback -o midi-callback.o $(ACFLAGS) midi-callback.c
	arm-elf-objcopy -O binary midi-callback.o midi-callback.arm

bin.levl.stamp: $(EXEC)_$(PACK).rcp arm.midi.stamp
	$(PILRC) -q -ro -creator LEMM -type levl -name "LEMM_$(PACK)" \
	$(EXEC)_$(PACK).rcp $(EXEC)_$(PACK).pdb

bin.game.stamp: $(EXEC).rcp $(EXEC)_TRAINER.rcp
	$(PILRC) -q $(EXEC).rcp
	$(PILRC) -q $(EXEC)_TRAINER.rcp

# source dependancies
palm.o: 	resource.h palm.h palm.c
device.o: 	resource.h palm.h device.h device.c
graphics.o: 	resource.h palm.h graphics.h graphics.c
game.o: 	resource.h palm.h game.h game.c \
		game_spr.inc game_gfx.inc game_msk.inc game_cmp.inc
help_$(LANG).o:	resource.h palm.h help.h help_$(LANG).c
register.o:	resource.h palm.h register.h register.c
gccfix.o:	resource.h palm.h gccfix.h gccfix.c

# compile rules
.SUFFIXES: .c .o

.c.o:
	$(CC) -c $(CCFLAGS) $<

# clean-up funtions
clean:
	rm -f *.[oa] *.arm *sections.* *.bin *.hdr bin.res *.grc *~ $(TEMP) $(BINFILE)

distclean:
	rm -f *.[oa] *.arm *sections.* *.bin *.hdr bin.res *.pdb *.[pg]rc *~ $(TEMP) $(BINFILE)
