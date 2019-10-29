# $FreeBSD$

PROG=		ucli
MAJOR=		0
MINOR=    	7
TINY=		3

PKGVER=		$(SHLIB_MAJOR).$(SHLIB_MINOR).$(SHLIB_TINY)

CFLAGS =        -O
CFLAGS+=	-I./contrib/
CFLAGS+=	-I./libucli/
#CFLAGS+=	-I../tcl8.6.8/
CFLAGS+=	-I/usr/local/include/tcl8.6/

CFLAGS+=	-L./libucli/
CFLAGS+=	-L/usr/local/lib

CFLAGS+=	-DPACKAGE_VERSION="\"$(PKGVER)\""


LDADD =		-ltcl86 -lucli

SRCS =		ucli.c 
MAN  =

DIRS = 		./contrib/tinyrl
DIRS+=		./contrib/lub
DIRS+=		./contrib/lubheap
DIRS+=		./libucli
DIRS+=		./module/mod_tclsh
DIRS+=		./module/mod_clish
DIRS+=		./module/mod_sh

# 'make cleandir' to clean .depends files as well as object and libraries.

test-bin:
	LD_LIBRARY_PATH=/usr/lib:/usr/local/lib:./contrib:./libucli ./ucli


.include <bsd.prog.mk>




