DLIB_VERSION=19.13
DLIB_DIR_NAME=dlib-${DLIB_VERSION}
DLIB_TAR_NAME=dlib-${DLIB_VERSION}.tar.bz2
DLIB_DOWNLOAD_LINK=http://dlib.net/files/${DLIB_TAR_NAME}

# Directory for output files
OUTDIR = build

# Target file name (without extension).
TARGET = svm-multiclass-learner

CPPFLAGS += \
	-I include \
	-I $(DLIB_DIR_NAME)

CPPFLAGS += \
	-O0 \
	-Wall \
	-std=c++11
	
LDFLAGS += -lpthread -lX11

SOURCE = \
    src/svm-multiclass.cpp \
	$(DLIB_DIR_NAME)/dlib/all/source.cpp

HEADERS = \


# List of all source files without directory and file-extension
ALLSRC = $(notdir $(basename $(SOURCE)))

# Define all object files.
ALLOBJ = $(addprefix $(OUTDIR)/, $(addsuffix .o, $(ALLSRC)))

all: createdirs dlib build

bin: $(OUTDIR)/$(TARGET).bin

dlib:
	if [ ! -d "./${DLIB_DIR_NAME}" ]; then \
		wget ${DLIB_DOWNLOAD_LINK}; \
		tar xvfj ${DLIB_TAR_NAME}; \
		rm -r ${DLIB_TAR_NAME}; \
	fi

.SILENT: dlib

build: bin

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
	-@mkdir $(OUTDIR)
