
DIR = libsaio
include ../MakePaths.dir

UTILDIR = ../util
LIBSADIR = ../libsa
INSTALLDIR = $(DSTROOT)/System/Library/Frameworks/System.framework/Versions/B/PrivateHeaders/standalone
#SYMROOT=

OPTIM = -Os -Oz
DEBUG = -DNOTHING
CFLAGS	= $(RC_CFLAGS) $(OPTIM) $(MORECPP) -arch i386 -g -Wmost \
    -D__ARCHITECTURE__=\"i386\" -DSAIO_INTERNAL_USER \
    -DRCZ_COMPRESSED_FILE_SUPPORT $(DEBUG) \
    -fno-builtin -static $(OMIT_FRAME_POINTER_CFLAG) \
    -mpreferred-stack-boundary=2 -fno-align-functions -fno-stack-protector \
    -march=pentium4 -msse2 -mfpmath=sse -msoft-float

DEFINES=
CONFIG = hd
INC = -I. -I$(SYMROOT) -I$(UTILDIR) -I$(LIBSADIR)
ifneq "" "$(wildcard /bin/mkdirs)"
  MKDIRS = /bin/mkdirs
else
  MKDIRS = /bin/mkdir -p
endif
AS = as
LD = ld
# LIBS= -lc_static
LIBS=

VPATH = $(OBJROOT):$(SYMROOT)

SAIO_OBJS = table.o asm.o bios.o biosfn.o \
	disk.o sys.o cache.o bootstruct.o \
	stringTable.o load.o pci.o allocate.o \
	vbe.o hfs.o hfs_compare.o \
	xml.o md5c.o device_tree.o \
	cpu.o platform.o acpi.o \
	smbios.o efi.o

SAIO_EXTERN_OBJS = console.o

SFILES =
CFILES = 
HFILES = 
EXPORTED_HFILES =
INSTALLED_HFILES =
OTHERFILES = Makefile
ALLSRC =  $(SFILES) $(CFILES) \
	$(HFILES) $(OTHERFILES)
LIBS = libsaio.a
DIRS_NEEDED = $(OBJROOT) $(SYMROOT)

all: $(DIRS_NEEDED) libsaio.h $(LIBS)

libsaio.a: $(SAIO_EXTERN_OBJS) $(SAIO_OBJS)
	rm -f $(SYMROOT)/$(@F)
	ar q $(SYMROOT)/$(@F) $^
	ranlib $(SYMROOT)/$(@F)

include ../MakeInc.dir

# dependencies
-include $(OBJROOT)/Makedep
