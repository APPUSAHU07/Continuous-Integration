# ==================================================
# Copyright 2015
# Siemens Product Lifecycle Management Software Inc.
# All Rights Reserved.
# ==================================================
# ==================================================
#             Auto-generated source
#                 DO NOT EDIT
# ==================================================



# ==================================================
# Define macros for C++ Server Library
# ==================================================
C9TESTLIB_SRC        = $(BASE_PATH)$(PS)src$(PS)server$(PS)C9testlib
C9TESTLIB_DRV        = $(PROJECT_DRV)$(PS)C9testlib
C9TESTLIB_OBJ        = $(PROJECT_OBJ)$(PS)libc9testlib
C9TESTLIB_CXXS       = $(C9TESTLIB_SRC)$(PS)*.c* $(C9TESTLIB_DRV)$(PS)*.cxx
C9TESTLIB_CXX_FLAGS  = $(CXX_FLAGS) -DIPLIB=c9testlib
C9TESTLIB_LINK_FLAGS = $(LINK_FLAGS)
C9TESTLIB_INCLUDES   = -I$(BASE_PATH)$(PS)src$(PS)server -I$(PROJECT_DRV) \
                                  -I$(TC_ROOT)$(PS)include -I$(TC_ROOT)$(PS)include_cpp \
                                  
C9TESTLIB_LIB_PATH   = $(LP)$(PROJECT_BIN)  $(LIB_PATH)
C9TESTLIB_LIBS       = $(LIB_PREFIX)metaframework$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)sa$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)tccore$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)tc$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)cxpom$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)epm$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)ae$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)grm$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)cfm$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)bom$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)ps$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)pom$(LIB_SUFFIX)  $(LIB_PREFIX)fclasses$(LIB_SUFFIX)  $(LIB_PREFIX)itk$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)extensionframework$(LIB_SUFFIX) $(LIB_PREFIX)mld$(LIB_SUFFIX)  $(LIB_PREFIX)base_utils$(LIB_SUFFIX) $(SYSLIBS)

C9TESTLIB_LD_FLAGS   = $(C9TESTLIB_LINK_FLAGS) $(C9TESTLIB_LIB_PATH)
C9TESTLIB_TARGET     = $(PROJECT_BIN)$(PS)libC9testlib
C9TESTLIB_AUTO_TARG  = $(C9TESTLIB_DRV)$(PS)initializemodule.cxx
C9TESTLIB_AUTO_SRC   = $(PROJECT_HOME)$(PS)extensions$(PS)*.xml

C9testlib_all:   $(C9TESTLIB_AUTO_TARG) $(C9TESTLIB_OBJ)  $(C9TESTLIB_TARGET).$(LIB_EXT)

$(C9TESTLIB_AUTO_TARG) : $(C9TESTLIB_AUTO_SRC)
	@echo ________________________________________________________________________________
	@echo Auto Generation of business object classes for C9testlib
	$(TC_ROOT)$(PS)bin$(PS)bmide_generatecode$(SCRIPT_EXT) -templateProj=$(PROJECT_HOME) -templateDeps=$(TEMPLATE_HOME) -gensrcDir=$(PROJECT_DRV) -srcDir=$(BASE_PATH)$(PS)src$(PS)server -log=$(BASE_PATH)$(PS)output$(PS)generate.log
	@echo ________________________________________________________________________________

$(C9TESTLIB_OBJ) :
	@echo Creating: $(C9TESTLIB_OBJ)
	-@$(MAKE_DIR) $(C9TESTLIB_OBJ)

$(C9TESTLIB_CXXS) :


# ==================================================
# Windows Build Target
# ==================================================
$(C9TESTLIB_TARGET).dll : $(C9TESTLIB_CXXS)
	@echo ________________________________________________________________________________
	@echo Building C++ Server C9testlib
	@echo Compiling Source...
	@$(COMPILE)  $(C9TESTLIB_CXX_FLAGS) $(C9TESTLIB_INCLUDES) -c $(**)   $(CARG_OUT)$(C9TESTLIB_OBJ)$(PS)
	@echo.
	@$(LINK) $(C9TESTLIB_LD_FLAGS) $(C9TESTLIB_OBJ)$(PS)*.$(OBJ_EXT)  $(C9TESTLIB_LIBS)  $(LARG_OUT)$(C9TESTLIB_TARGET).dll
	$(COPY_FILE)  $(PROJECT_BIN)$(PS)libC9testlib.dll $(PROJECT_TC)$(PS)bin
	$(COPY_FILE)  $(PROJECT_BIN)$(PS)libC9testlib.lib $(PROJECT_TC)$(PS)lib
	@echo ________________________________________________________________________________



# ==================================================
# UNIX Build Target
# ==================================================
$(C9TESTLIB_TARGET).so : $(C9TESTLIB_CXXS)
	@echo ________________________________________________________________________________
	@echo Building C++ Server C9testlib
	@# The compile output goes to the current directory (project root), so remove any .o files
	@# that may have been left here from previous failed builds. Then after the compile move all .o
	@# files to the the desired obj folder.
	@-rm *.$(OBJ_EXT)
	@echo Compiling $(C9TESTLIB_CXXS)
	@$(COMPILE)  $(C9TESTLIB_CXX_FLAGS) $(C9TESTLIB_INCLUDES) -c $(C9TESTLIB_CXXS)
	@mv *.$(OBJ_EXT) $(C9TESTLIB_OBJ)
	@echo Linking $(C9TESTLIB_TARGET).so
	@$(LINK) $(C9TESTLIB_LD_FLAGS) $(C9TESTLIB_OBJ)$(PS)*.$(OBJ_EXT)  $(C9TESTLIB_LIBS)  $(LARG_OUT) $(C9TESTLIB_TARGET).so
	$(COPY_FILE)  $(PROJECT_BIN)$(PS)libC9testlib.so $(PROJECT_TC)$(PS)lib
	@echo ________________________________________________________________________________


C9testlib_clean:
	@echo ________________________________________________________________________________
	@echo Cleaning C++ Server C9testlib
	-$(DELETE_DIR)  $(C9TESTLIB_OBJ)
	-$(DELETE_DIR)  $(C9TESTLIB_DRV)
	-$(DELETE_FILE) $(PROJECT_BIN)$(PS)libC9testlib.*
	-$(DELETE_FILE) $(PROJECT_TC)$(PS)bin$(PS)libC9testlib.dll
	-$(DELETE_FILE) $(PROJECT_TC)$(PS)lib$(PS)libC9testlib.lib
	-$(DELETE_FILE) $(PROJECT_TC)$(PS)lib$(PS)libC9testlib.so
	@echo ________________________________________________________________________________
