dnl Process this file with autoconf to produce a configure script.
AC_INIT([mpeg4-2000], 1.2.3)
AC_CONFIG_HEADER(mpeg4-2000.h)
dnl Detect the canonical host and target build environment
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.5 no-define foreign])

AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_MSG_CHECKING([libtool tag=C for nasm])
if test -z "`./libtool --help | grep \"\\-\\-tag\"`"; then
  AC_MSG_RESULT([no])
else
  LIBTOOL_C_TAG="--tag=CXX"
  AC_SUBST(LIBTOOL_C_TAG)
  AC_MSG_RESULT([yes])
fi 
AC_PROG_MAKE_SET
AC_PROG_CC
OBJC="???"
AC_SUBST(OBJC)
AC_PROG_CXX(c++)
AC_C_INLINE
AC_C_CONST
AC_PROG_INSTALL
AC_PATH_XTRA
AC_C_BIGENDIAN

AC_SUBST(ACLOCAL_AMFLAGS)

AS=gcc
AC_SUBST(AS)
ASFLAGS="$ASFLAGS"
AC_SUBST(ASFLAGS)
CCAS=gcc
AC_SUBST(CCAS)
CCASFLAGS="$ASFLAGS"
AC_SUBST(CCASFLAGS)

dnl Checks for libraries.

dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_SIZEOF(long)
AC_LANG_PUSH(C++)
AC_CHECK_SIZEOF(bool)
AC_LANG_POP(C++)

AC_CHECK_HEADERS(fcntl.h unistd.h stdint.h inttypes.h getopt.h byteswap.h)
AC_CHECK_HEADERS(sys/time.h)

dnl Checks for library functions.
AC_CHECK_FUNCS(strerror strcasestr poll getopt getopt_long getopt_long_only getrusage socketpair strsep inet_ntoa)


dnl @author  Guido Draheim <guidod@gmx.de>
dnl heavilly modified
AC_DEFUN([MY_CHECK_CC_OPT],
[
save_CFLAG="$CFLAGS"
save_CXXFLAG="$CXXFLAGS"
CFLAGS="$CFLAGS -Werror $1"
CXXFLAGS="$CXXFLAGS -Werror $1"
   AC_MSG_CHECKING([whether $3 accepts $1])
   AC_COMPILE_IFELSE(
[extern void f(void); void f(void){}],
[$2=yes
 AC_MSG_RESULT([yes])],
[$2=no
 AC_MSG_RESULT([no])])
CFLAGS="$save_CFLAG"
CXXFLAGS="$save_CXXFLAG"
])

AC_DEFUN([MY_CHECK_CC_OPT_SET],
[
   MY_CHECK_CC_OPT($1, mpeg4ip_ok, $3)
   if test $mpeg4ip_ok = yes; then
     $2="$$2 $1"
   fi
])

AC_DEFUN([MY_CHECK_CC_OPT_F_OR_M],
[
  temp_val=-m$1
  MY_CHECK_CC_OPT($temp_val,mpeg4ip_ok, [c compiler])
  if test $mpeg4ip_ok = yes; then
     AC_SUBST($2, $temp_val)
     AM_CONDITIONAL(HAVE_$2, true)
  else
     temp_val=-f$1
     MY_CHECK_CC_OPT($temp_val, mpeg4ip_ok, [c compiler])
     if test $mpeg4ip_ok = yes; then
        AC_SUBST($2, $temp_val)
     AM_CONDITIONAL(HAVE_$2, true)
     else
        temp_val=
        AC_SUBST($2, $temp_val)
     AM_CONDITIONAL(HAVE_$2, false)
     fi
  fi
])

dnl
BASE_ERR="-Wall"
BILLS_CWARNINGS="$BASE_ERR"

AM_CONDITIONAL(HAVE_ALIGN_LOOPS, false)
MY_CHECK_CC_OPT_F_OR_M(align-loops, ALIGN_LOOPS)
AM_CONDITIONAL(HAVE_ALIGN_FUNCS, false)
MY_CHECK_CC_OPT_F_OR_M(align-functions, ALIGN_FUNCS)
AM_CONDITIONAL(HAVE_ALIGN_JUMPS, false)
MY_CHECK_CC_OPT_F_OR_M(align-jumps, ALIGN_JUMPS)

MY_CHECK_CC_OPT_SET([-Wmissing-prototypes], BILLS_CWARNINGS, [c compiler])
MY_CHECK_CC_OPT_SET([-Wmissing-declarations], BILLS_CWARNINGS, [c compiler])
MY_CHECK_CC_OPT_SET([-Wno-char-subscripts], BILLS_CWARNINGS, [c compiler])
MY_CHECK_CC_OPT_SET([-Wno-unknown-pragmas],BILLS_CWARNINGS, [c compiler])
MY_CHECK_CC_OPT_SET([-Wformat=2],BILLS_CWARNINGS, [c compiler])
BILLS_CPPWARNINGS="$BASE_ERR"
AC_LANG_PUSH(C++)
MY_CHECK_CC_OPT_SET([-Wmissing-prototypes], BILLS_CPPWARNINGS,[c++ compiler])
MY_CHECK_CC_OPT_SET([-Wno-char-subscripts], BILLS_CPPWARNINGS,[c++ compiler])
MY_CHECK_CC_OPT_SET([-Woverloaded-virtual], BILLS_CPPWARNINGS,[c++ compiler])
MY_CHECK_CC_OPT_SET([-Wno-unknown-pragmas], BILLS_CPPWARNINGS,[c++ compiler])
MY_CHECK_CC_OPT_SET([-Wno-deprecated], BILLS_CPPWARNINGS,[c++ compiler])
MY_CHECK_CC_OPT_SET([-Wformat=2], BILLS_CPPWARNINGS,[c++ compiler])
AC_LANG_POP(C++)
AC_SUBST(BILLS_CWARNINGS)
AC_SUBST(BILLS_CPPWARNINGS)

SDL_CFLAGS=`sdl-config --cflags`
CFLAGS="$CFLAGS -DMPEG4IP $SDL_CFLAGS"
AC_SUBST(CFLAGS)
CXXFLAGS="$CXXFLAGS -DMPEG4IP $SDL_CFLAGS"
AC_SUBST(CXXFLAGS)

dnl Now output the Makefile's
AC_OUTPUT( \
	Makefile \
	src/Makefile \
)
