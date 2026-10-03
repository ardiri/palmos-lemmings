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
MULTIGEN = $(PREFIX)m68k-palmos-multigen
ARMCC    = arm-elf-gcc
ARMCOPY  = arm-elf-objcopy
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
CCTOOLS =
CCFLAGS = -O2 -Wall -palmos4 -DLANG_$(LANG)
ACFLAGS = -mshort-load-bytes -ffixed-r9 -nostartfiles -O2 -Wall \
	  -Xlinker --script=callback.ld

# optional: the palmdev-macos toolchain (macOS, Apple Silicon), which needs
# no palmdev-prep - see tools/README.md
#   make PALMDEV=$(HOME)/tools/palmdev-macos
ifdef PALMDEV
TOOLS    = $(PALMDEV)/toolchain
SDK      = $(PALMDEV)/sdk/sdk-4
PILRC_DIR ?= $(HOME)/tools/pilrc-3.2-64bit/bin
export GCC_EXEC_PREFIX = $(TOOLS)/lib/gcc-lib/

PREFIX   = $(TOOLS)/bin/
ARMCC    = $(PREFIX)arm-palmos-gcc
ARMCOPY  = $(PREFIX)arm-palmos-objcopy
PILRC    = $(PILRC_DIR)/pilrc
PRC2BIN  = python3 ../tools/prc2bin.py

CCTOOLS  = -B$(TOOLS)/lib/gcc-lib/m68k-palmos/2.95.3-kgpd/ \
	   -B$(TOOLS)/m68k-palmos/bin/ -B$(TOOLS)/m68k-palmos/lib/
CCFLAGS  = -O2 -Wall -DLANG_$(LANG) $(CCTOOLS) \
	   -nostdinc -I$(TOOLS)/lib/gcc-lib/m68k-palmos/2.95.3-kgpd/include \
	   $(addprefix -I,$(shell find $(SDK)/include -type d)) -L$(SDK)/lib
ACFLAGS := -B$(TOOLS)/lib/gcc-lib/arm-palmos/3.3.1/ -B$(TOOLS)/arm-palmos/bin/ \
	   $(ACFLAGS)
endif

# compile requirements
all:
	LANG=en $(MAKE) lemmings

lemmings: $(EXEC) $(EXEC).def arm.midi.stamp arm.sfx.stamp bin.game.stamp $(ENCRYPT)
	$(BUILDPRC) $(EXEC).def $(BINFILE) *.bin
	$(PRC2BIN) $(EXEC).prc > $(TEMP)
	$(ENCRYPT) code0002.bin code0006.bin
	$(BUILDPRC) $(EXEC).def *.bin
	$(MAKE) clean
	cp $(EXEC).prc ../$(EXEC).prc

$(ENCRYPT): regcode/encrypt.c
	cc -O2 -Wall -o $@ $<

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(CCFLAGS) -o $(BINFILE)

$(MSEG): $(EXEC).def
#	multigen $(EXEC).def                     # pre prc-tools 2.2
	$(MULTIGEN) $(EXEC).def
	$(CC) $(CCTOOLS) -c -o $(EXEC)-sections.o $(EXEC)-sections.s

arm.sfx.stamp: sfx-callback.c
	$(ARMCC) -e callback -o sfx-callback.o $(ACFLAGS) sfx-callback.c
	$(ARMCOPY) -j .text -O binary sfx-callback.o sfx-callback.arm

arm.midi.stamp: midi-callback.c
	$(ARMCC) -e callback -o midi-callback.o $(ACFLAGS) midi-callback.c
	$(ARMCOPY) -O binary midi-callback.o midi-callback.arm

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
