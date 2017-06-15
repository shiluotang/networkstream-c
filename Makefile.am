ACLOCAL_AMFLAGS = -Wall

lib_LTLIBRARIES = libnetworkstream-c.la

libnetworkstream_c_la_SOURCES = \
								./src/debug.c \
								./src/stringbuilder.c \
								./src/networkstream.c

check_PROGRAMS = test1 test2

test1_SOURCES	= ./tests/test1.c
test1_LDADD		= ./libnetworkstream-c.la

test2_SOURCES	= ./tests/test2.c
test2_LDADD		= ./libnetworkstream-c.la

TESTS = $(check_PROGRAMS)
