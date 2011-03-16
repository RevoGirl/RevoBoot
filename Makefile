#
#	Makefile for RevoBoot
#

export USE_APPLE_PB_SUPPORT = all

# CFLAGS	= -O $(MORECPP) -arch i386 -g 
DEFINES=
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
PAX = /bin/pax

OBJROOT = `pwd`/obj
SYMROOT = `pwd`/sym
DSTROOT = `pwd`/dst
SRCROOT = /tmp

#
# Export version number (picked up by i386/libsaio/Makefile)
#

export PRODUCT_VERSION_NUMBER = `cat ../../VERSION`

EXCLUDE = --exclude=.DS_Store --exclude=sym --exclude=obj --exclude=*.sh

ARCHLESS_RC_CFLAGS=`echo $(RC_CFLAGS) | sed 's/-arch [a-z0-9]*//g'`

VPATH = $(OBJROOT):$(SYMROOT)

GENERIC_SUBDIRS =

#
# Export target OS type (picked up by i386/libsaio/Makefile)
#

ifeq ($(MAKECMDGOALS), lion)
	TARGET_OS_TYPE = 3;
else
	TARGET_OS_TYPE = 2;
endif

export PRODUCT_OS_TARGET = `echo $(TARGET_OS_TYPE)`

lion: all

all: $(SYMROOT) $(OBJROOT)
	@if [ -z "$(RC_ARCHS)" ]; then					  \
		RC_ARCHS="i386";					  \
	fi;								  \
	SUBDIRS="$(GENERIC_SUBDIRS) $$RC_ARCHS";			  \
	for i in $$SUBDIRS; 						  \
	do \
	    if [ -d $$i ]; then						  \
		echo ================= make $@ for $$i =================; \
		( OBJROOT=$(OBJROOT)/$${i};				  \
		  SYMROOT=$(SYMROOT)/$${i};				  \
		  DSTROOT=$(DSTROOT);					  \
	          XCFLAGS=$(ARCHLESS_RC_CFLAGS);			  \
	          GENSUBDIRS="$(GENERIC_SUBDIRS)";			  \
	          for x in $$GENSUBDIRS;				  \
	          do							  \
	              if [ "$$x" == "$$i" ]; then			  \
	                  XCFLAGS="$(RC_CFLAGS)";			  \
	                  break;					  \
	              fi						  \
	          done;							  \
		  echo "$$OBJROOT $$SYMROOT $$DSTROOT"; \
		    cd $$i; ${MAKE}					  \
			"OBJROOT=$$OBJROOT"		 	  	  \
		  	"SYMROOT=$$SYMROOT"				  \
			"DSTROOT=$$DSTROOT"				  \
			"SRCROOT=$$SRCROOT"				  \
			"RC_ARCHS=$$RC_ARCHS"				  \
			"TARGET=$$i"					  \
			"RC_CFLAGS=$$XCFLAGS" $@			  \
		) || exit $$?; 						  \
	    else							  \
	    	echo "========= Nothing to build for $$i =========";	  \
	    fi;								  \
	done

clean:
	rm -rf sym obj dst out.log

$(SYMROOT) $(OBJROOT) $(DSTROOT):
	@$(MKDIRS) $@
