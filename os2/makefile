# Makefile for OS/2 using Open Watcom compiler.
#
# wmake
# - builds wildmidi.dll and its import lib (wildmidi.lib)
#
# wmake target=static
# - builds the static library wildmidi_static.lib

!ifndef target
target = dll
!endif

CFLAGS = -bt=os2 -bm -fp5 -fpi87 -mf -oeatxh -w4 -ei -j -zp8 -zq
# -5s  :  Pentium stack calling conventions.
# -5r  :  Pentium register calling conventions.
CFLAGS+= -5s
DLLFLAGS=-bd

.SUFFIXES:
.SUFFIXES: .obj .c

DLLNAME=wildmidi.dll
EXPNAME=wildmidi.exp
LIBNAME=wildmidi.lib
LIBSTATIC=wildmidi_static.lib
PLAYER=wildmidi.exe
PLAYER_STATIC=wildmidi_static.exe
PLAYER_LIBS=mmpm2.lib

CFLAGS_LIB= $(CFLAGS) -DWILDMIDI_BUILD
CFLAGS_EXE= $(CFLAGS)
!ifeq target static
BLD_TARGET=$(LIBSTATIC) $(PLAYER_STATIC)
!else
CFLAGS_LIB+= $(DLLFLAGS)
BLD_TARGET=$(DLLNAME) $(PLAYER)
!endif
INCPATH=-I"$(%WATCOM)/h/os2" -I"$(%WATCOM)/h"
INCLUDES=$(INCPATH) -I. -I"../include"

OBJ=wm_error.obj file_io.obj lock.obj wildmidi_lib.obj reverb.obj gus_pat.obj f_xmidi.obj f_mus.obj f_hmp.obj f_midi.obj f_hmi.obj mus2mid.obj xmi2mid.obj internal_midi.obj patches.obj sample.obj
PLAYER_OBJ=wm_tty.obj msleep.obj getopt_long.obj out_none.obj out_wave.obj out_dart.obj wildmidi.obj

all: $(BLD_TARGET)

# rely on symbol name, not ordinal: -irn switch of wlib is default, but -inn is not.
$(DLLNAME): $(OBJ)
	wlink NAM $@ OP q SYSTEM os2v2_dll INITINSTANCE TERMINSTANCE OPTION MANYAUTODATA FIL {$(OBJ)} OPTION IMPF=$(EXPNAME)
	wlib -q -b -n -c -pa -s -t -zld -ii -io -inn $(LIBNAME) +$(DLLNAME)

$(LIBSTATIC): $(OBJ)
	wlib -q -b -n -c -pa -s -t -zld -ii -io $@ $(OBJ)

$(PLAYER): $(DLLNAME) $(PLAYER_OBJ)
	wlink N $@ SYS OS2V2 OP q LIBR {$(LIBNAME) $(PLAYER_LIBS)} F {$(PLAYER_OBJ)}

$(PLAYER_STATIC): $(LIBSTATIC) $(PLAYER_OBJ)
	wlink N $@ SYS OS2V2 OP q LIBR {$(LIBSTATIC) $(PLAYER_LIBS)} F {$(PLAYER_OBJ)}

# rules for library objs:
.c: ../src;../src/player
.c.obj:
	wcc386 $(CFLAGS_LIB) $(INCLUDES) -fo=$^@ $<
# rules for player objs:
getopt_long.obj: getopt_long.c
	wcc386 $(CFLAGS_EXE) $(INCLUDES) -fo=$^@ $<
wm_tty.obj: wm_tty.c
	wcc386 $(CFLAGS_EXE) $(INCLUDES) -fo=$^@ $<
wildmidi.obj: wildmidi.c
	wcc386 $(CFLAGS_EXE) $(INCLUDES) -fo=$^@ $<
out_none.obj: out_none.c
	wcc386 $(CFLAGS_EXE) $(INCLUDES) -fo=$^@ $<
out_wave.obj: out_wave.c
	wcc386 $(CFLAGS_EXE) $(INCLUDES) -fo=$^@ $<
out_dart.obj: out_dart.c
	wcc386 $(CFLAGS_EXE) $(INCLUDES) -fo=$^@ $<
msleep.obj: msleep.c
	wcc386 $(CFLAGS_EXE) $(INCLUDES) -fo=$^@ $<

distclean: clean .symbolic
	rm -f $(DLLNAME) $(EXPNAME) $(LIBNAME) $(LIBSTATIC) $(PLAYER) $(PLAYER_STATIC)
clean: .symbolic
	rm -f *.obj
