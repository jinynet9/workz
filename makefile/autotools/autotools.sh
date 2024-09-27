#!/bin/bash
#
# edit Makefile.am
#

CLEAN_FILES="COPYING INSTALL \
	       autom4te.cache \
	       autoscan.log \
	       aclocal.m4 \
	       compile \
	       missing \
	       configure \
	       configure.ac \
	       configure.scan \
	       config.h.in \
	       config.h \
	       config.log \
	       config.status \
	       stamp-h1 \
	       depcomp \
	       install-sh \
	       AUTHORS NEWS README ChangeLog"

function purge_conf() {
    echo -n "Purging all before autogen ... "
    [ -f Makefile ] && make clean
    /bin/rm -rf $CLEAN_FILES
    find . -name Makefile.in -exec rm -f {} \;
    find . -name Makefile -exec rm -f {} \;
    find . -name .deps -exec rm -rf {} \;
    echo "DONE"
}

function usage_exit() {
    echo "usage: $0 [-p] [-r]"
    echo "  -p  purge all before autogen"
    echo "  -r  reconfigure (purge & config)"
    exit 1
}

while getopts "pr" opt
do
    case "$opt" in
    	"p")
	    purge_conf
	    exit 0
	    ;;
	"r")
	    purge_conf
	    ;;
	*)
	    usage_exit
	    ;;
    esac
done

# STEP #0 : Prepare required files

touch AUTHORS NEWS README ChangeLog

# STEP #1 : RUN autoscan ---> configure.scan

echo "RUN autoscan ... "

autoscan

[ $? -ne 0 ] && { echo "autoscan: FAILED."; exit 1; }

# STEP #2 : MOVE configure.scan to configure.ac & EDIT configure.ac

echo "Generate configure.ac ... "

mv configure.scan configure.ac

sed -i 's/^AC_INIT.*/AC_INIT([hello-pkg], [1.0], [bugreports@foo.com])/' configure.ac
sed -i '/AC_INIT.*/a\AM_INIT_AUTOMAKE' configure.ac
sed -i '/AC_PROG_CC/a\AM_PROG_CC_C_O' configure.ac

# STEP #3 : RUN aclocal ---> aclocal.m4

echo "RUN aclocal ... "

aclocal

[ $? -ne 0 ] && { echo "aclocal: FAILED."; exit 1; }

# STEP #4 : RUN autoheader ---> autom4te.cache config.h.in

echo "RUN autoheader ... "

autoheader

[ $? -ne 0 ] && { echo "autoheader: FAILED."; exit 1; }

# STEP #5 : RUN autoconf ---> configure

echo "RUN autoconf ... "

autoconf

[ $? -ne 0 ] && { echo "autoconf: FAILED."; exit 1; }

# STEP #6 : RUN automake ---> depcomp, missing, Makefile.in, install-sh, mkinstalldirs

echo "RUN automake ... "

#automake --add-missing --copy --force-missing
automake --add-missing --force --gnu

[ $? -ne 0 ] && { echo "automake: FAILED."; exit 1; }

# STEP #7 : RUN configure ---> config.h, config.log, config.status, Makefile

echo "RUN configure ... "

./configure CFLAGS="-g -O0"

[ $? -ne 0 ] && { echo "FAILED."; exit 1; }

exit 0

