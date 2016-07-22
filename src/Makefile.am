bin_PROGRAMS = alpharay

if WXWIDGETS
alpharay_SOURCES = guimain.cc \
                   mainframe.cc \
                   projectloadingdialog.cc \
                   renderframe.cc \
                   wxalpharaybitmap.cc \
                   wxalpharaycanvas.cc \
				   previewpanel.cc \
				   propertyeditor.cc
else
alpharay_SOURCES = climain.cc
endif

alpharay_LDADD = libalpharay/libalpharay.la

SUBDIRS = libalpharay
