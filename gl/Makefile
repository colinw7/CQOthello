.SILENT:

CDEBUG = -g

PROG    = CGLOthello
CPP_SRC = CGLOthello.cpp

EXTRA_CFLAGS = \
-I$(INC_DIR)/CGLApp \
-I$(INC_DIR)/CGLWidget \
-I$(INC_DIR)/CGLControl \
-I$(INC_DIR)/CGLRenderer3D \
-I$(INC_DIR)/CGLPixelRenderer \
-I$(INC_DIR)/CPixelRenderer \
-I$(INC_DIR)/CGeometry3D \
-I$(INC_DIR)/CMaterial \
-I$(INC_DIR)/CImageLib \
-I$(INC_DIR)/CFont \
-I$(INC_DIR)/CPen \
-I$(INC_DIR)/CBrush \
-I$(INC_DIR)/CSymbol2D \
-I$(INC_DIR)/CPixelClip \
-I$(INC_DIR)/CClipSide \
-I$(INC_DIR)/CDisplayRange2D \
-I$(INC_DIR)/CMath \
-I$(INC_DIR)/CEvent \
-I$(INC_DIR)/CKeyType \
-I$(INC_DIR)/CFile \
-I$(INC_DIR)/CFileType \
-I$(INC_DIR)/CAlignType \
-I$(INC_DIR)/CStrUtil \
-I$(INC_DIR)/CUtil \

LIBS = \
-lCGLApp -lCGLWidget -lCGLControl -lCWindow -lCGLRenderer3D \
-lCGeometry3D -lCRenderer2D -lCGLPixelRenderer \
-lCGLApp -lCGLTexture -lCGLUtil -lCImageLib -lCPixelRenderer \
-lCImageRenderer2D -lCImagePixelRenderer -lCSymbol2D \
-lCImageDraw -lCPath2D -lCAxis2D -lCPixelClip \
-lCOthelloLib -lCFont -lCTimer -lCMath -lCFileParse -lCFile -lCOSUser -lCOSRand -lCOSTimer \
-lCEvent -lCOSNaN -lCOSFile -lCTempFile -lCFreeType -lCConfig -lCEnv \
-lCRGBUtil -lCFileUtil -lCUtil -lCStrUtil -lCRegExp \
-lCAssert -lCGlob -lCPrintF -lCStrNCase \
$(GL_LIBS) $(X11_LIBS) $(IMAGE_LIBS) \
-ltre -lfreetype -lboost_signals -lz

EXTRA_DEPS = \
$(LIB_DIR)/libCGLApp.a \
$(LIB_DIR)/libCGLPixelRenderer.a \
$(LIB_DIR)/libCGLRenderer3D.a \
$(LIB_DIR)/libCGLWidget.a \

include $(MAKE_DIR)/bin.mk
