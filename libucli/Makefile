# $FreeBSD$

LIB=	  	ucli
SHLIB_MAJOR=	0
SHLIB_MINOR=    7
SHLIB_TINY=	3

PKGVER=		$(SHLIB_MAJOR).$(SHLIB_MINOR).$(SHLIB_TINY)

CFLAGS =        -O 

CFLAGS+=	-I../contrib/

CFLAGS+=	-DPACKAGE_VERSION="\"$(PKGVER)\"" 

#DPADD =		-
#LDADD =		-ltinyrl -llub -llubheap

# TINYXML=	tinyxml_read.cpp
UCLI!=		find . -type f -name "*.c"

SRCS =	$(UCLI)
SRCS+=	$(TINYXML)

# 'make cleandir' to clean .depends files as well as object and libraries.

.include <bsd.lib.mk>




