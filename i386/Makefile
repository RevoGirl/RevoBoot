#
#	Makefile for RevoBoot
#

CFLAGS	= -O $(MORECPP) -arch i386 -g -static
CONFIG = hd
LIBDIR = libsa
INC = -I. -I$(LIBDIR)
ifneq "" "$(wildcard /bin/mkdirs)"
  MKDIRS = /bin/mkdirs
else
  MKDIRS = /bin/mkdir -p
endif
AS = as
LD = ld

#
# These paths are only valid in subdirectories of this directory.
#

OBJROOT=`pwd`/../../obj/i386
SYMROOT=`pwd`/../../sym/i386
DSTROOT=`pwd`/../../dst/i386
SRCROOT=/tmp

VPATH = $(OBJROOT):$(SYMROOT)

#
# The build over here is important.
#

SUBDIRS = util libsa libsaio boot2

lion: all
mountain-lion: all

all:
	@for i in ${SUBDIRS}; \
	do \
		echo ================= make $@ for $$i =================; \
		( cd $$i; ${MAKE}					  \
			"OBJROOT=$(OBJROOT)/$$i"			  \
		  	"SYMROOT=$(SYMROOT)"				  \
			"DSTROOT=$(DSTROOT)"				  \
			"SRCROOT=$(SRCROOT)"				  \
			"RC_ARCHS=$(RC_ARCHS)"				  \
			"RC_CFLAGS=$(RC_CFLAGS)" $@ 			  \
		) || exit $$?; 						  \
	done
