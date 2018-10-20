CDEBUG = -g
LDEBUG = -g

LIBS = \
-lCTclApp -lCOthelloLib -lCFile -lCEnv -lCStrUtil \
-lCUtil -lCGlob -lCRegExp -lCReadLine -lCOSRand \
-lCHistory -lCOSTime -lCOSUser -lCOSFile -lCPrintF -lCStrNCase \
-lreadline -lcurses -ltix -ltk -ltcl \
-lXt -lX11 -lXft -lXext -lXss -lfontconfig -lpthread -lm -lc -ldl

PROG    = CTclOthello
CPP_SRC = CTclOthello.cpp

EXTRA_DEPS = $(LIB_DIR)/libCTclApp.a

TCL_FILES =  \
CTclOthello.tcl

include $(MAKE_DIR)/bin.mk
