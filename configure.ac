#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libmsgq], [1.0], [akhs3214@gmail.com])
AC_CONFIG_SRCDIR([inc/libmsgq.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_RANLIB

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
