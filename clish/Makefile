# $FreeBSD$

LIB=	  	ucli
SHLIB_MAJOR=	0
SHLIB_MINOR=    7
SHLIB_TINY=	3

PKGVER=		$(SHLIB_MAJOR).$(SHLIB_MINOR).$(SHLIB_TINY)

CFLAGS=         -O -I../ -DPACKAGE_VERSION="\"$(PKGVER)\""

TINYXML=	tinyxml_read.cpp
UCLI!=		ls *.c

SRCS =	$(UCLI)
SRCS+=	$(TINYXML)

# 'make cleandir' to clean .depends files as well as object and libraries.

.include <bsd.lib.mk>




