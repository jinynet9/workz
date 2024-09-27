#
# all
# lib or liball
# target or targetall
# clean or cleanall
# distclean or distcleanall
#

SUBDIRS = sub1 sub2

TOP_DIR = $(PWD)

export TOP_DIR


TARGET = a.out

#
# $1	SUBDIRS
# $2	TARGET
#
define subdirdo
	@for DIR in $(1); do \
		$(MAKE) -C $$DIR $(2); \
		if [ $$? != 0 ]; then exit 1; fi; \
	done
endef

all : subdir lib target

subdir :
	$(call subdirdo,$(SUBDIRS),all)

lib : $(LIB_TARGET)

.PHONY : $(SUBDIRS)



include Makefile.skel

