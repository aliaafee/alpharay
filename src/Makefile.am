if WXWIDGETS

bin_PROGRAMS = alpharay alpharaycli

alpharay_SOURCES = guimain.cc \
                   mainframe.cc \
                   projectloadingdialog.cc \
                   renderframe.cc \
                   wxalpharaybitmap.cc \
                   wxalpharaycanvas.cc \
				   previewpanel.cc \
				   propertyeditor.cc
alpharay_LDADD = libalpharay/libalpharay.la

alpharaycli_SOURCES = climain.cc
alpharaycli_LDADD = libalpharay/libalpharay.la

else

bin_PROGRAMS = alpharay

alpharay_SOURCES = climain.cc
alpharay_LDADD = libalpharay/libalpharay.la

endif

SUBDIRS = libalpharay
