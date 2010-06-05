################################################################################
# This is the main make file for wxWidgets/wxx. 
# It has the following commands and functionality:
#   make help - Display a list of available commands.
#   make - produce a debug version of all the target directories
#   make clean - cleans the top level binary directory
#   make subclean - cleans only the target applications' binary directory
#   make distclean - performs clean and subclean.
#
#   All of the above options have an equivalent release option that performs 
#   the same operation as the debug version but with the targets being the 
#   release versions in the release directories. (Note: I tried using such
#   command line targets such as "clean release" but then clean and release 
#   became targets; each of which caused make to run the script. The script
#   became more complex than desired. So it was decided to use a command line
#   variable definition instead.)
#
#   make b=r
#   make clean b=r
#   make subclean b=r
#   make distclean b=r
# 
# Author:
#   David Russak, with much help from Paul Cannon's make system and 
#   http://make.paulandlesley.org/multi-arch.html.
#
# Version: $Id$
#
################################################################################

.SUFFIXES:

SHARED_DIR := ../../Shared
MAKE_DIR := $(SHARED_DIR)/make
ifeq ($(SDL_EXTERN),)
  SDL_EXTERN := ../../sdlExtern
endif

# This include file sets important make variables such as the build type, 
# CFLAGS, etc.
include $(MAKE_DIR)/exports.mk
include $(MAKE_DIR)/cleaning.mk

TARGET_DIRS := wxIFM \
               wxthings \
               wxplotctrl \
               wxTreeListCtrl \
               wxTreeMultiCtrl

################################################################################
################################################################################
# Make section
#
ifneq ($(CLEANING), 1)

# A macro to start a new make in a child directory.
MAKETARGET = $(MAKE) --no-print-directory -C $@ -f Makefile $(MAKECMDGOALS)

.PHONY: all

all: $(TARGET_DIRS) 

$(TARGET_DIRS): make_locals
	+@$(MAKETARGET)

make_locals: $(LIB_DIR)/locals.mk

# If needed create the locals.mk file. This also includes creating $LIB_DIR
# and making config.h if necessary.
$(LIB_DIR)/locals.mk:
	+@[ -d $@ ] || mkdir -p $(LIB_DIR)
	+@[ -f $@ ] || $(MAKE_DIR)/generate_locals $(LIB_DIR) locals.mk $(MAKE_DIR) $(BLD_TYPE) $(SDL_EXTERN)

help:
	@echo "-------- Build options --------"
	@echo "make                Produce a debug version of all libraries."
	@echo "make b=r            Produce a release version of all libraries." 
	@echo
	@echo "-------- Cleaning options --------"
	@echo "make clean          Clean the top level debug binary directory."
	@echo "make clean b=r      Clean the top level release binary directory."
	@echo "make subclean       Clean only the target applications' debug binary directory."
	@echo "make subclean b=r   Clean only the target applications' release binary directory."
	@echo "make distclean      Performs debug clean and subclean.
	@echo "make distclean b=r  Performs release clean and subclean.


else 
################################################################################
################################################################################
# cleaning section
#
# clean - only remove executables - not config.h (which causes many files to
#   rebuild).
# subclean - cleans only the libraries' objects
# distclean - performs library clean and cleans all subdirectories
#

# Clean subdirectories macro 
CLEANTARGET = $(MAKE) clean -C $@ -f Makefile

.PHONY: clean subclean distclean $(TARGET_DIRS)

distclean: subclean
	rm -rf $(LIB_DIR)

clean:
	rm -f $(LIB_DIR)/*.a

subclean: $(TARGET_DIRS)

$(TARGET_DIRS):
	+@$(CLEANTARGET)

endif

# These rules keep make from trying to remake Makefile or *.mk files. 
Makefile : ;
%.mk :: ;

