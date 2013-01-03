##
## Makefile for R-Tree library
##
## $Id: Makefile.Unix,v 1.7 2003/01/14 00:17:30 andrew Exp $

INCLUDEDIR	=	./include
LIBDIR		=	./lib

CC		=	gcc
RANLIB		=	ranlib
LINT		=	lint
MAKEFILE	= 	makefile
SHELL		= 	/bin/sh
DEFINES		=	$(RDEFINES) 
CFLAGS		=	-g -pedantic -Wall \
			$(CWARNFLAGS) $(DEFINES) \
                        -I$(INCLUDEDIR) -I../common/include


LIBNAME	=	$(LIBDIR)/librtree.a
TESTEXE	=	testrtree

.SUFFIXES: .ln .o .c

.c.o :
	$(CC) $(CFLAGS) -c $< -o $@

SUBDIRS		=  \
		src

OBJS		= \
		src/card.o \
		src/index.o \
		src/node.o \
		src/rectangle.o \
		src/split_q.o

TESTOBJS        = \
		test/test.o

$(LIBNAME) : $(OBJS)
	- rm -f $(LIBNAME)
	ar cr $(LIBNAME) $(OBJS)
	$(RANLIB) $(LIBNAME)

$(TESTEXE) test : $(LIBNAME) $(TESTOBJS)
	$(CC) $(CFLAGS) -o $(TESTEXE) $(TESTOBJS) $(LIBNAME) -lm

clean allclean: 
	- rm -f $(LIBNAME) *.o *core *.ln [Mm]akefile.bak
	@ for name in $(SUBDIRS) test; \
	do \
		echo "make clean in $$name" ; \
		(cd $$name ; rm -f *.o *core *.ln) ; \
	done
	rm -f $(TESTEXE)

all: $(LIBNAME) $(TESTEXE)


##
## $Log: Makefile.Unix,v $
## Revision 1.7  2003/01/14 00:17:30  andrew
## o Update for filtering bug Sheng found
##
## Revision 1.6  2002/07/13 19:00:08  andrew
## o Moved rect.c to rectangle.c
##
## Revision 1.5  2002/07/11 02:30:55  andrew
## o Fixed makefile to include common
## o Updated calculation of test point.
##
## Revision 1.4  2002/07/11 01:14:55  andrew
## o Moved compilation environment back to C from C++
## o Cleaned up comment structure
## o Converted Makefile to use spherical volume code
##
## Revision 1.3  2002/07/11 00:35:34  andrew
## o Working on R-Tree code on PC
##
## Revision 1.2  2002/07/10 03:02:57  andrew
## o Converted project over to C++ file extensions
## o Renamed and moved numerous files to keep internal header
##   files hidden.
## o Cleaned up exposed header file (rTreeIndex.h) to be better
##   laid out and better commented.
##
## Revision 1.1  2002/07/06 14:36:56  andrew
## o Initial check-in of the R-tree code
##
##
