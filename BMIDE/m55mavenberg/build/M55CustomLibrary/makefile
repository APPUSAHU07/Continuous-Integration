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
M55CUSTOMLIBRARY_SRC        = $(BASE_PATH)$(PS)src$(PS)server$(PS)M55CustomLibrary
M55CUSTOMLIBRARY_DRV        = $(PROJECT_DRV)$(PS)M55CustomLibrary
M55CUSTOMLIBRARY_OBJ        = $(PROJECT_OBJ)$(PS)libm55customlibrary
M55CUSTOMLIBRARY_CXXS       = $(M55CUSTOMLIBRARY_SRC)$(PS)*.c* $(M55CUSTOMLIBRARY_DRV)$(PS)*.cxx
M55CUSTOMLIBRARY_CXX_FLAGS  = $(CXX_FLAGS) -DIPLIB=m55customlibrary
M55CUSTOMLIBRARY_LINK_FLAGS = $(LINK_FLAGS)
M55CUSTOMLIBRARY_INCLUDES   = -I$(BASE_PATH)$(PS)src$(PS)server -I$(PROJECT_DRV) \
                                  -I$(TC_ROOT)$(PS)include -I$(TC_ROOT)$(PS)include_cpp \
                                  
M55CUSTOMLIBRARY_LIB_PATH   = $(LP)$(PROJECT_BIN)  $(LIB_PATH)
M55CUSTOMLIBRARY_LIBS       = $(LIB_PREFIX)metaframework$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)sa$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)tccore$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)tc$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)cxpom$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)epm$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)ae$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)pom$(LIB_SUFFIX)  $(LIB_PREFIX)fclasses$(LIB_SUFFIX)  $(LIB_PREFIX)itk$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)extensionframework$(LIB_SUFFIX) $(LIB_PREFIX)mld$(LIB_SUFFIX)  $(LIB_PREFIX)base_utils$(LIB_SUFFIX) $(SYSLIBS)

M55CUSTOMLIBRARY_LD_FLAGS   = $(M55CUSTOMLIBRARY_LINK_FLAGS) $(M55CUSTOMLIBRARY_LIB_PATH)
M55CUSTOMLIBRARY_TARGET     = $(PROJECT_BIN)$(PS)libM55CustomLibrary
M55CUSTOMLIBRARY_AUTO_TARG  = $(M55CUSTOMLIBRARY_DRV)$(PS)initializemodule.cxx
M55CUSTOMLIBRARY_AUTO_SRC   = $(PROJECT_HOME)$(PS)extensions$(PS)*.xml

M55CustomLibrary_all:   $(M55CUSTOMLIBRARY_AUTO_TARG) $(M55CUSTOMLIBRARY_OBJ)  $(M55CUSTOMLIBRARY_TARGET).$(LIB_EXT)

$(M55CUSTOMLIBRARY_AUTO_TARG) : $(M55CUSTOMLIBRARY_AUTO_SRC)
	@echo ________________________________________________________________________________
	@echo Auto Generation of business object classes for M55CustomLibrary
	$(TC_ROOT)$(PS)bin$(PS)bmide_generatecode$(SCRIPT_EXT) -templateProj=$(PROJECT_HOME) -templateDeps=$(TEMPLATE_HOME) -gensrcDir=$(PROJECT_DRV) -srcDir=$(BASE_PATH)$(PS)src$(PS)server -log=$(BASE_PATH)$(PS)output$(PS)generate.log
	@echo ________________________________________________________________________________

$(M55CUSTOMLIBRARY_OBJ) :
	@echo Creating: $(M55CUSTOMLIBRARY_OBJ)
	-@$(MAKE_DIR) $(M55CUSTOMLIBRARY_OBJ)

$(M55CUSTOMLIBRARY_CXXS) :


# ==================================================
# Windows Build Target
# ==================================================
$(M55CUSTOMLIBRARY_TARGET).dll : $(M55CUSTOMLIBRARY_CXXS)
	@echo ________________________________________________________________________________
	@echo Building C++ Server M55CustomLibrary
	@echo Compiling Source...
	@$(COMPILE)  $(M55CUSTOMLIBRARY_CXX_FLAGS) $(M55CUSTOMLIBRARY_INCLUDES) -c $(**)   $(CARG_OUT)$(M55CUSTOMLIBRARY_OBJ)$(PS)
	@echo.
	@$(LINK) $(M55CUSTOMLIBRARY_LD_FLAGS) $(M55CUSTOMLIBRARY_OBJ)$(PS)*.$(OBJ_EXT)  $(M55CUSTOMLIBRARY_LIBS)  $(LARG_OUT)$(M55CUSTOMLIBRARY_TARGET).dll
	$(COPY_FILE)  $(PROJECT_BIN)$(PS)libM55CustomLibrary.dll $(PROJECT_TC)$(PS)bin
	$(COPY_FILE)  $(PROJECT_BIN)$(PS)libM55CustomLibrary.lib $(PROJECT_TC)$(PS)lib
	@echo ________________________________________________________________________________



# ==================================================
# UNIX Build Target
# ==================================================
$(M55CUSTOMLIBRARY_TARGET).so : $(M55CUSTOMLIBRARY_CXXS)
	@echo ________________________________________________________________________________
	@echo Building C++ Server M55CustomLibrary
	@# The compile output goes to the current directory (project root), so remove any .o files
	@# that may have been left here from previous failed builds. Then after the compile move all .o
	@# files to the the desired obj folder.
	@-rm *.$(OBJ_EXT)
	@echo Compiling $(M55CUSTOMLIBRARY_CXXS)
	@$(COMPILE)  $(M55CUSTOMLIBRARY_CXX_FLAGS) $(M55CUSTOMLIBRARY_INCLUDES) -c $(M55CUSTOMLIBRARY_CXXS)
	@mv *.$(OBJ_EXT) $(M55CUSTOMLIBRARY_OBJ)
	@echo Linking $(M55CUSTOMLIBRARY_TARGET).so
	@$(LINK) $(M55CUSTOMLIBRARY_LD_FLAGS) $(M55CUSTOMLIBRARY_OBJ)$(PS)*.$(OBJ_EXT)  $(M55CUSTOMLIBRARY_LIBS)  $(LARG_OUT) $(M55CUSTOMLIBRARY_TARGET).so
	$(COPY_FILE)  $(PROJECT_BIN)$(PS)libM55CustomLibrary.so $(PROJECT_TC)$(PS)lib
	@echo ________________________________________________________________________________


M55CustomLibrary_clean:
	@echo ________________________________________________________________________________
	@echo Cleaning C++ Server M55CustomLibrary
	-$(DELETE_DIR)  $(M55CUSTOMLIBRARY_OBJ)
	-$(DELETE_DIR)  $(M55CUSTOMLIBRARY_DRV)
	-$(DELETE_FILE) $(PROJECT_BIN)$(PS)libM55CustomLibrary.*
	-$(DELETE_FILE) $(PROJECT_TC)$(PS)bin$(PS)libM55CustomLibrary.dll
	-$(DELETE_FILE) $(PROJECT_TC)$(PS)lib$(PS)libM55CustomLibrary.lib
	-$(DELETE_FILE) $(PROJECT_TC)$(PS)lib$(PS)libM55CustomLibrary.so
	@echo ________________________________________________________________________________
