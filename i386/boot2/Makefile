
#	Makefile for i386 boot program
#	define FLOPPY and SMALL using DEFINES macro as necessary


DIR = boot2
include ../MakePaths.dir

OPTIM = -Os -Oz
CFLAGS	= $(RC_CFLAGS) $(OPTIM) $(MORECPP) -arch i386 -g -Wmost -Werror \
	-fno-builtin -DSAIO_INTERNAL_USER -static $(OMIT_FRAME_POINTER_CFLAG) \
	-DMAKE_TARGET_OS=$(PRODUCT_OS_TARGET) \
	-mpreferred-stack-boundary=2 -fno-align-functions -fno-stack-protector \
	-march=pentium4 -msse2 -mfpmath=sse -msoft-float

DEFINES=
CONFIG = hd
SYMDIR = $(SYMROOT)
LIBSADIR = ../libsa
LIBSAIODIR = ../libsaio
UTILDIR = ../util
#THEME = default
#THEMEDIR = ../../artwork/themes/$(THEME)
INC = -I. -I.. -I$(SYMDIR) -I$(LIBSADIR) -I$(LIBSAIODIR) -I$(UTILDIR)
ifneq "" "$(wildcard /bin/mkdirs)"
  MKDIRS = /bin/mkdirs
else
  MKDIRS = /bin/mkdir -p
endif
AS = as
LD = gcc
# LIBS= -lc_static
LIBS= -L$(SYMDIR) -lsaio -lsa
LIBDEP= $(SYMDIR)/libsaio.a $(SYMDIR)/libsa.a

OTHER_FILES =

INSTALLDIR = $(DSTROOT)/usr/standalone/i386
VPATH = $(OBJROOT):$(SYMROOT)

#
# The ordering is important;
#	

OBJS = boot2.o boot.o bootlogo.o graphics.o drivers.o options.o lzss.o

UTILDIR = ../util
SFILES = boot2.s
CFILES = boot.c bootlogo.c graphics.c drivers.c options.c
HFILES = boot.h bootlogo.h 

OTHERFILES = Makefile
ALLSRC = $(FOREIGNSRC) $(FOREIGNBIN) $(SFILES) $(CFILES) $(HFILES) $(OTHERFILES)
DIRS_NEEDED = $(OBJROOT) $(SYMROOT)
BOOT2ADDR = 20200
MAXBOOTSIZE = 392704

lion: all

all: $(DIRS_NEEDED) boot

boot: machOconv $(OBJS) $(LIBDEP)
	$(LD) -static -Wl,-preload -Wl,-segaddr,__INIT,$(BOOT2ADDR) \
	        -nostdlib -arch i386 -Wl,-segalign,20 \
		-o $(SYMROOT)/boot.sys $(filter %.o,$^) $(LIBS) -lcc_kext
	machOconv $(SYMROOT)/boot.sys $(SYMROOT)/boot
	size $(SYMROOT)/boot.sys
	ls -l $(SYMROOT)/boot
	@( size=`ls -l $(SYMROOT)/boot | awk '{ print $$5}'` ; \
	  if expr "$$size" ">" "$(MAXBOOTSIZE)" > /dev/null ;\
	  then \
	    echo "Booter executable larger than $(MAXBOOTSIZE) bytes" ;\
	    rm $(SYMROOT)/boot ;\
	    exit 1;\
	  fi)

include ../MakeInc.dir

#dependencies
-include $(OBJROOT)/Makedep
