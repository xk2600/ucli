# $FreeBSD$

LIB=	  	lubheap
SHLIB_MAJOR=	1
SHLIB_MINOR=    0

LUBHEAP_POSIX!= ls ./posix/*.c
LUBHEAP!=       ls ./*.c

SRCS =	  	$(LUBHEAP)
SRCS+=		$(LUBHEAP_POSIX)

# NOTE: './vxworks' contains representative implementation for vxworks.
#       in the future it might be worthwhile to implement the abiliity
#       to set a variable to crosscompile onto vxworks.

.include <bsd.lib.mk>




