#
# Until I can remove the dependency on the appkit,
# we'll just keep the generated files in this directory
# and install them directly, rather than generating them again.
#

DIR = util
include ../MakePaths.dir

VPATH = $(OBJROOT):$(SYMROOT)

INSTALLDIR = $(DSTROOT)/usr/standalone/i386
LOCALBIN = $(DSTROOT)/usr/local/bin

OPTIM = -Os -Oz
CFLAGS = $(RC_CFLAGS) $(OPTIM) -Wmost -Werror -g
LDFLAGS =
CFILES = md.c machOconv.c
ALLSRC = $(CFILES) $(MFILES) $(HFILES) $(EXPORT_HFILES)

PROGRAMS = md machOconv

OUTFILES = $(PROGRAMS)

DIRS_NEEDED = $(OBJROOT) $(SYMROOT) $(LANGDIR)

lion: all

mountain-lion: all

all: $(DIRS_NEEDED) $(PROGRAMS)

machOconv: machOconv.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(DEFINES) -o $(SYMROOT)/$(@F) machOconv.o
md:
	$(CC) -mdynamic-no-pic -Wall -dead_strip -arch i386 -mmacosx-version-min=10.5 md.c -o $(SYMROOT)/md

include ../MakeInc.dir

#dependencies
-include $(OBJROOT)/Makedep

