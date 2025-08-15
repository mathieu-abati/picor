noinst_LIBRARIES = libxcorr.a libavghash.a libimgtools.a
libxcorr_a_SOURCES = xcorr.c
libxcorr_a_CFLAGS = -I$(top_srcdir)/include
libavghash_a_SOURCES = avghash.c
libavghash_a_CFLAGS = -I$(top_srcdir)/include
libimgtools_a_SOURCES = img_tools.c
libimgtools_a_CFLAGS = -I$(top_srcdir)/include ${MAGICKCORE_CFLAGS}
libimgtools_a_LDFLAGS = ${MAGICKCORE_LIBS}
