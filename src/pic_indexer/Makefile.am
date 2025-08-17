noinst_HEADERS = pic_indexer.h

bin_PROGRAMS = picor-index-folder picor-retrieve-pic
picor_index_folder_SOURCES = index_folder.c
picor_index_folder_CFLAGS = -I$(top_srcdir)/include ${MAGICKCORE_CFLAGS}
picor_index_folder_LDFLAGS = -static ${MAGICKCORE_LIBS}
picor_index_folder_LDADD = -lm $(top_builddir)/lib/libavghash.a $(top_builddir)/lib/libimgtools.a
picor_retrieve_pic_SOURCES = retrieve_pic.c
picor_retrieve_pic_CFLAGS = -I$(top_srcdir)/include ${MAGICKCORE_CFLAGS}
picor_retrieve_pic_LDFLAGS = -static ${MAGICKCORE_LIBS}
picor_retrieve_pic_LDADD = -lm $(top_builddir)/lib/libavghash.a $(top_builddir)/lib/libimgtools.a
