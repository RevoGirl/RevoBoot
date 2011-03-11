
#	Makefile for i386 boot program
#	define FLOPPY and SMALL using DEFINES macro as necessary

#PNGCRUSH = `which pngcrush`
#PNGOPTIONS = -rem cHRM -rem gAMA -rem iCCP -rem sRGB

DIR = boot2
include ../MakePaths.dir

OPTIM = -Os -Oz
CFLAGS	= $(RC_CFLAGS) $(OPTIM) $(MORECPP) -arch i386 -g -Wmost -Werror \
		-fno-builtin -DSAIO_INTERNAL_USER -static $(OMIT_FRAME_POINTER_CFLAG) \
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

# The ordering is important;
# boot2.o must be first.
#OBJS = boot2.o boot.o graphics.o drivers.o prompt.o options.o lzss.o mboot.o \
#	ramdisk.o picopng.o resume.o bmdecompress.o graphic_utils.o gui.o
# button.o browser.o scrollbar.o == NOTYET	

OBJS = boot2.o boot.o bootlogo.o graphics.o drivers.o options.o lzss.o

UTILDIR = ../util
SFILES = boot2.s

#CFILES = boot.c graphics.c drivers.c prompt.c options.c mboot.c picopng.c
CFILES = boot.c bootlogo.c graphics.c drivers.c options.c

#HFILES = boot.h appleClut8.h appleboot.h mboot.h multiboot.h picopng.h
HFILES = boot.h bootlogo.h 

OTHERFILES = Makefile
ALLSRC = $(FOREIGNSRC) $(FOREIGNBIN) $(SFILES) $(CFILES) \
	$(HFILES) $(OTHERFILES)
DIRS_NEEDED = $(OBJROOT) $(SYMROOT)
BOOT2ADDR = 20200
MAXBOOTSIZE = 392704

all: $(DIRS_NEEDED) boot

embedtheme: CFLAGS += -DEMBED_THEME
#embedtheme: art.h embedded.h all

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

#prompt.o: vers.h
#vers.h:
#	@echo "#define I386BOOT_VERSION \"5.0.132\"" > $(SYMROOT)/vers.h
#	@echo "#define I386BOOT_BUILDDATE \"`date \"+%Y-%m-%d %H:%M:%S\"`\"" >> $(SYMROOT)/vers.h
#	@echo "#define I386BOOT_CHAMELEONVERSION \"`cat ../../version`\"" >> $(SYMROOT)/vers.h
#	@echo "#define I386BOOT_CHAMELEONREVISION \"`svnversion -n | tr -d [:alpha:]`\"" >> $(SYMROOT)/vers.h

#art.h:
#	@if [ "$(PNGCRUSH)" ]; then	\
#		echo "optimizing art files ...\n$(PNGCRUSH) $(PNGOPTIONS) artwork/$(THEME)"; \
#		cd $(THEMEDIR) && find . -name '*.png' -exec $(PNGCRUSH) -q $(PNGOPTIONS) -d $(SYMROOT)/embed {} \;; \
#		cp theme.plist $(SYMROOT)/embed; \
#	else \
#		cp -R $(THEMEDIR) $(SYMROOT)/embed; \
#	fi;
#	@echo "#ifndef __BOOT2_ART_H" >> $(SYMROOT)/art.h
#	@echo "#define __BOOT2_ART_H" >> $(SYMROOT)/art.h
#	@cd $(SYMROOT)/embed && find . -name 'theme.plist' -exec xxd -i >> $(SYMROOT)/art.h {} \;
#	@cd $(SYMROOT)/embed && find . -name '*.png' -exec xxd -i >> $(SYMROOT)/art.h {} \;
#	@echo "#endif /* !__BOOT2_ART_H */"  >> $(SYMROOT)/art.h ;\
#	rm -rf $(SYMROOT)/embed ;\

#embedded.h:
#	@cd $(SYMROOT)/../../doc && xxd -i BootHelp.txt > $(SYMROOT)/embedded.h

install_i386:: all $(INSTALLDIR)
	cp $(SYMROOT)/boot $(OTHER_FILES) $(INSTALLDIR)
	cd $(INSTALLDIR); chmod u+w boot $(OTHER_FILES)

include ../MakeInc.dir

#dependencies
-include $(OBJROOT)/Makedep
