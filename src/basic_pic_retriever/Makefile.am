bin_PROGRAMS = picor-basic-pic-retriever
picor_basic_pic_retriever_SOURCES = basic_pic_retriever.c
picor_basic_pic_retriever_CFLAGS = -I$(top_srcdir)/include ${MAGICKCORE_CFLAGS}
picor_basic_pic_retriever_LDFLAGS = -static ${MAGICKCORE_LIBS}
picor_basic_pic_retriever_LDADD = -lm $(top_builddir)/lib/libxcorr.a $(top_builddir)/lib/libavghash.a $(top_builddir)/lib/libimgtools.a

bin_SCRIPTS = picor-basic-pic-retriever-stats

pkgdatadir = $(datadir)/picor
install-data-local:
	mkdir -p $(DESTDIR)$(pkgdatadir)/
	cp -r $(top_srcdir)/src/basic_pic_retriever/stats_resources $(DESTDIR)$(pkgdatadir)/
