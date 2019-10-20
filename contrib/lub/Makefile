# $FreeBSD$

LIB=	  	lub
SHLIB_MAJOR=	0
SHLIB_MINOR=    7
SHLIB_TINY=	4

CFLAGS=         -O -I../

LUBARGV!=	ls ./argv/*.c
LUBBINTREE!=	ls ./bintree/*.c
LUBBLOCKPOOL!=	ls ./blockpool/*.c
LUBCTYPE!=	ls ./ctype/*.c
LUBDBLOCKPOOL!=	ls ./dblockpool/*.c
LUBDUMP!=	ls ./dump/*.c
LUBFIFO!=	ls ./fifo/*.c
LUBSCHEDULER!=	ls ./scheduler/*.c
LUBSTRING!=	ls ./string/*.c

LUBHEAP!=		ls ./heap/*.c
LUBHEAP_POSIX!= 	ls ./heap/posix/*.c
LUBHEAP_VXWORKS!=	ls ./heap/vxworks/*.c

LUBPARTITION!=		ls ./partition/*.c
LUBPARTITION_POSIX!=	ls ./posix/*.c
LUBPARTITION_VXWORKS!=	ls ./vxworks/*.c


SRCS =	$(LUBARGV)
SRCS+=	$(LUBBINTREE)
SRCS+=	$(LUBBLOCKPOOL)
SRCS+=	$(LUBCTYPE)
SRCS+=	$(LUBDBLOCKPOOL)
SRCS+=	$(LUBDUMP)
SRCS+=	$(LUBFIFO)
SRCS+=	$(LUBSCHEDULER)
SRCS+=	$(LUBSTRING)

#TODO: fix vxworks check...

#.if for vxworks...
#SRCS+=	$(LUBHEAP) 	$(LUBHEAP_VXWORKS)
#SRCS+=	$(LUBPARTITION)	$(LUBPARTITION_VXWORKS)
#.else # assume posix compliant OS

SRCS+=	$(LUBHEAP) 	$(LUBHEAP_POSIX)
SRCS+=	$(LUBPARTITION)	$(LUBPARTITION_POSIX)

#.end for vxworks

# 'make cleandir' to clean .depends files as well as object and libraries.


# NOTE: './vxworks' contains representative implementation for vxworks.
#       in the future it might be worthwhile to implement the abiliity
#       to set a variable to crosscompile onto vxworks.

.include <bsd.lib.mk>




