DLIB_VERSION=19.13
DLIB_DIR_NAME=dlib-${DLIB_VERSION}
DLIB_TAR_NAME=dlib-${DLIB_VERSION}.tar.bz2
DLIB_DOWNLOAD_LINK=http://dlib.net/files/${DLIB_TAR_NAME}

# Quite awful, but seems to be the only dynamic solutions
QT_LIB_DIR=$(shell qtdiag | grep LibrariesPath | cut -d ":" -f 2)
QT_INCLUDE_DIR=$(shell qtdiag | grep HeadersPath | cut -d ":" -f 2)

# Directory for output files
OUTDIR = build

# Target file name (without extension).
TARGET = svm-multiclass-learner

CPPFLAGS += \
	-I include \
	-I $(DLIB_DIR_NAME) \
	-I $(QT_INCLUDE_DIR) \
	-I $(QT_INCLUDE_DIR)/QtCore \
	-I $(QT_INCLUDE_DIR)/QtWidgets \
	-I $(QT_INCLUDE_DIR)/QtGui \
	-I $(QT_INCLUDE_DIR)/QtPrintSupport

CPPFLAGS += \
	-O0 \
	-Wall \
	-std=c++11 \
	-fPIC
	
LDFLAGS +=  \
	-L $(QT_LIB_DIR)

LDFLAGS += \
	-lQt5Widgets -lQt5Gui -lQt5Core -lQt5PrintSupport -lGL \
	-lpthread \
	-lX11 

SOURCE += \
    src/svm-multiclass.cpp \
	$(DLIB_DIR_NAME)/dlib/all/source.cpp \
	src/moc_mainwindow.cpp \
	src/moc_qcustomplot.cpp \
	src/mainwindow.cpp \
	src/qcustomplot.cpp

HEADERS += \
	include/mainwindow.h \
	include/qcustomplot.h

UI += \
	forms/mainwindow.ui
	
UI_GENERATED_FILES += \
	include/ui_mainwindow.h

MOC_GENERATED_FILES += \
	src/moc_mainwindow.cpp \
	src/moc_qcustomplot.cpp

# List of all source files without directory and file-extension
ALLSRC = $(notdir $(basename $(SOURCE))) 

# Define all object files.
ALLOBJ = $(addprefix $(OUTDIR)/, $(addsuffix .o, $(ALLSRC)))

all: createdirs dlib build

bin: $(OUTDIR)/$(TARGET).bin
ui: $(UI_GENERATED_FILES)
moc: $(MOC_GENERATED_FILES)

include/ui_%.h: forms/%.ui
	QT_SELECT=qt5 uic $< -o $@

qrc_%.cpp: %.qrc
	QT_SELECT=qt5 qrc $< $< -o $@

src/moc_%.cpp: include/%.h
	QT_SELECT=qt5 moc $< -o $@
	
dlib:
	if [ ! -d "./${DLIB_DIR_NAME}" ]; then \
		wget ${DLIB_DOWNLOAD_LINK}; \
		tar xvfj ${DLIB_TAR_NAME}; \
		rm -r ${DLIB_TAR_NAME}; \
	fi

.SILENT: dlib


build: ui moc bin

.SECONDARY : $(TARGET).bin
.PRECIOUS : $(ALLOBJ)
%.bin:  $(ALLOBJ)
	g++ $(ALLOBJ) --output $@ $(LDFLAGS)

# Compile: create object files from C++ source files
define COMPILE_C_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1)
	g++ -c $$(CPPFLAGS) $$< -o $$@
endef
$(foreach src, $(SOURCE), $(eval $(call COMPILE_C_TEMPLATE, $(src))))

# Create output directories
createdirs:
	-@mkdir -p $(OUTDIR)
