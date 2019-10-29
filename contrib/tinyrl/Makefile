# $FreeBSD$

LIB=	  	tinyrl
SHLIB_MAJOR=	0
SHLIB_MINOR=    7
SHLIB_TINY=	4

CFLAGS=         -O -I../

TINYRLHISTORY!= ls ./history/*.c
TINYRLVT100!=	ls ./vt100/*.c
TINYRL!=	ls ./*.c

SRCS =	$(TINYRLHISTORY)
SRCS+=	$(TINYRLVTOO)
SRCS+=	$(TINYRL)


# 'make cleandir' to clean .depends files as well as object and libraries.


.include <bsd.lib.mk>




