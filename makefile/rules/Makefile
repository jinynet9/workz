

################################################################################
#
# 
#
pathsearch = $(firstword $(wildcard $(addsuffix /$(1),$(subst :, ,$(PATH)))))

LS := $(call pathsearch,ls)

echo:
	@echo "LS : $(LS)"

#$ make
#LS : /usr/bin/ls

################################################################################
#
# create dynamic rule
#

define onerule
$(target) : $(prerequisites)
	@echo target: $$@, prerequisites: $$^   # recipe 를 별도의 라인에 작성
endef

target = t1
prerequisites = p1 p2 p3
$(eval $(onerule))       # eval 함수를 사용해야 한다.

target = t2
prerequisites = r1 r2 r3
$(eval $(onerule))

.DEFAULT: ;

#$ make t1
#target: t1, prerequisites: p1 p2 p3
#
#$ make t2
#target: t2, prerequisites: r1 r2 r3

################################################################################
#
# 
#

#define AA
#foo := bar
#endef
#
#BB = $1 := $2
#
#$(eval $(AA))
#$(info $(foo))
#
#$(eval $(call BB,goo,gar))
#$(info $(goo))
#
#echo:
#	@echo

#$ make
#bar
#gar

################################################################################
#
# macro substitution
#

#SRCS = a.c b.c
#OBJS = $(SRCS:.c=.o)
#
#echo:
#	@echo "OBJS : $(OBJS)"

################################################################################
#
#

#X = hello
#Y = $(X)
#
#X = world
#
#echo:
#	@echo "X : $(X)"
#	@echo "Y : $(Y)"

################################################################################
#
#

#echo:
#	@echo "CC : $(CC)"

#$ CC="gcc -g" make
#CC : gcc -g
#
#$ make CC="gcc -x"
#CC : gcc -x

################################################################################
#
#

#FLAG1 = -1
#override FLAG2 = -2
#override FLAG3 += -3
#
#echo:
#	@echo "FLAG1 : $(FLAG1)"
#	@echo "FLAG2 : $(FLAG2)"
#	@echo "FLAG3 : $(FLAG3)"

#$ make FLAG1=-x FLAG2=-x FLAG3=-x
#FLAG1 : -x
#FLAG2 : -2
#FLAG3 : -x -3

################################################################################
#
#

#OBJS = a.o
#override CFLAGS += -g
#
#echo:
#	@echo "origin OBJS  : $(origin OBJS)"
#	@echo "origin HOME  : $(origin HOME)"
#	@echo "origin PARAM : $(origin PARAM)"
#	@echo "origin CC    : $(origin CC)"
#	@echo "origin @     : $(origin @)"
#	@echo "CFLAGS       : $(CFLAGS)"
#	@echo "origin CFLAGS: $(origin CFLAGS)"

#$ make PARAM=args CFLAGS="-I/work/include"
#origin OBJS  : file
#origin HOME  : environment
#origin PARAM : command line
#origin CC    : default
#origin @     : automatic
#CFLAGS       : -I/work/include -g
#origin CFLAGS: override

################################################################################
#
#

#comma := ,
#empty :=
#space := $(empty) $(empty)
#
#TEXT =   id, pass,  name
#
#STR1 = $(subst $(comma),_,$(TEXT))
#STR2 = $(subst $(space),_,$(TEXT))
#
#echo:
#	@echo "sust1: $(STR1)"
#	@echo "sust2: $(STR2)"
	
#$ make
#sust1: id_ pass_  name
#sust2: id,_pass,__name

################################################################################
#
#

#echo:
#	@echo "foreach: $(foreach str, a b c, $(str).c)"

#$ make
#foreach:  a.c  b.c  c.c

################################################################################
#
#

#WILDCARD = $(wildcard *.c)
#
#echo:
#	@echo "wildcard    : $(WILDCARD)"

#$ ls
#a.c  b.c  Makefile
#
#$ make
#wildcard    : a.c b.c

################################################################################
#
#

#FILES = src/test.c etc/init.d/header.h main.c test.o
#
#SUFFIX = $(suffix $(FILES))
#ADDSUFFIX = $(addsuffix .o, $(FILES))
#ADDPREFIX = $(addprefix /home/, $(FILES))
#
#echo:
#	@echo "suffix    : $(SUFFIX)"
#	@echo "addsuffix : $(ADDSUFFIX)"
#	@echo "addprefix : $(ADDPREFIX)"

#$ make
#suffix    : .c .h .c .o
#addsuffix : src/test.c.o etc/init.d/header.h.o main.c.o test.o.o
#addprefix : /home/src/test.c /home/etc/init.d/header.h /home/main.c /home/test.o

################################################################################
#
#

#PATHS = /etc/bin /user/src/main.c fun.c Makefile
#FILES = /src/test.c etc/init.d/header.h main.c test.o
#
#DIR = $(dir $(PATHS))
#NOTDIR = $(notdir $(PATHS))
#BASENAME = $(basename $(FILES))
#
#echo:
#	@echo "dir      : $(DIR)"
#	@echo "notdir   : $(NOTDIR)"
#	@echo "basename : $(BASENAME)"

#$ make
#dir      : /etc/ /user/src/ ./ ./
#notdir   : bin main.c fun.c Makefile
#basename : /src/test etc/init.d/header main test

################################################################################
#
#

#STR = What a wonderful world
#
#WORD = $(word 4, $(STR))
#WORDS = $(words $(STR))
#WORDLIST = $(wordlist 2, 3, $(STR))
#FIRSTWORD = $(firstword $(STR))
#
#echo:
#	@echo "word      : $(WORD)"
#	@echo "words     : $(WORDS)"
#	@echo "wordlist  : $(WORDLIST)"
#	@echo "firstword : $(FIRRSTWORD)"
#	@echo "join      : $(join main test, .c .h)"

#$ make
#word      : world
#words     : 4
#wordlist  : a wonderful
#firstword : 
#join      : main.c test.h

################################################################################
#
#

#FILES = main.c test.s header.h
#
#SRCS  = $(filter %.c %.s, $(FILES))
#HEADS = $(filter %.h, $(FILES))
#
#SRCSO  = $(filter-out %.c %.s, $(FILES))
#HEADSO = $(filter-out %.h, $(FILES))
#
#STR = What a wonderful world
#
#FINDSTRING  = $(findstring wonderful, $(STR))
#FINDSTRINGM = $(findstring wonderfl, $(STR))
#
#echo:
#	@echo "source      : $(SRCS)"
#	@echo "header      : $(HEADS)"
#	@echo "source out  : $(SRCSO)"
#	@echo "header out  : $(HEADSO)"
#	@echo "findstring      : $(FINDSTRING)"
#	@echo "findstring miss : $(FINDSTRINGM)"

#$ make
#source      : main.c test.s
#header      : header.h
#source out  : header.h
#header out  : main.c test.s
#findstring      : wonderful
#findstring miss : 

################################################################################
#
#

#MACRO_SORT = b c a e c b
#SORT = $(sort $(MACRO_SORT))
#
#MACRO_STRIP = It is	not   your     falut
#STRIP = $(strip $(MACRO_STRIP))
#
#echo:
#	@echo "sort  : $(SORT)"
#	@echo "strip : $(STRIP)"

#$ make
#sort    : a b c e
#strip   : It is not your falut

################################################################################
#
#

#SH = $(shell ls *.c)
#STR = $(subst love, LOVE, I love you)
#PAT = $(patsubst %.c, %.o, fun1.c fun2.c fun3.t)
#
#MACRO = test.c main.c TEST
#PATSTR = $(MACRO:%.c=%.o)
#
#echo:
#	@echo "shell    : $(SH)"
#	@echo "subst    : $(STR)"
#	@echo "patsubst : $(PAT)"
#	@echo "patstr   : $(PATSTR)"

#$ make
#shell    : a.c b.c
#subst    :  I  LOVE you
#patsubst :  fun1.o  fun2.o fun3.t
#patstr   : test.o main.o TEST

################################################################################
#
#

#CC = gcc
#
#CONFIG := $(findstring config, $(MAKECMDGOALS))
#
#ifeq ($(CONFIG), config)
#    CC = "gcc --sysroot=/"
#endif
#
#all:
#
#%.config:
#	@echo $(CONFIG)
#	@echo $(CC)
#
