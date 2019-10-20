# $FreeBSD$

LIB=	  	lubheap
SHLIB_MAJOR=	1
SHLIB_MINOR=    0

CFLAGS=         -O -I../

LUBHEAP_POSIX!= ls ./posix/*.c

SRCS =		$(LUBHEAP_POSIX)

# 'make cleandir' to clean .depends files as well as object and libraries.


# NOTE: './vxworks' contains representative implementation for vxworks.
#       in the future it might be worthwhile to implement the abiliity
#       to set a variable to crosscompile onto vxworks.

.include <bsd.lib.mk>




