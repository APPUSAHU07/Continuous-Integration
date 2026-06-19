@echo on
setlocal enabledelayedexpansion

:: =============================================================================
::  bmide_deploy_only.bat
::  Deploys a BMIDE template to Teamcenter.
::  Variables expected from caller (CallBmideDeploy.bat):
::    TC_ROOT, TC_DATA, TC_BIN, TC_SOLR_ROOT (optional - auto-detected below)
::    ACTC_DATA_LOAD_USER, ACTC_DATA_LOAD_USER_PWD, ACTC_DATA_LOAD_USER_GRP
::    ACTC_CUSTOM_CONFIG_DIR, PROJECT_TEMPLATE_NAME, softwareVersion
:: =============================================================================

:: ── Auto-detect TC_SOLR_ROOT (handles solr, solr-9.6.0, solr-8.x, etc.) ─────
for /f "delims=" %%D in ('dir /b /ad "%TC_ROOT%\solr*" 2^>nul') do set "TC_SOLR_ROOT=%TC_ROOT%\%%D"
echo Resolved TC_SOLR_ROOT = [%TC_SOLR_ROOT%]
if not defined TC_SOLR_ROOT (
    echo ERROR: No solr* directory found under %TC_ROOT%
    exit /b 1
)

:: =============================================================================
echo.
echo ============================================================
echo  BMIDE DEPLOY  ^|  Template: %PROJECT_TEMPLATE_NAME%  ^|  Version: %softwareVersion%
echo ============================================================
echo.

:: =============================================================================
:: SECTION 1 — PRE-RUN DB UNLOCK
:: =============================================================================
echo === [ Pre-run DB unlock START ] ===
call "%TC_DATA%\tc_profilevars.bat"
call "%TC_BIN%\install" -unlock_db -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP%
echo PRE-RUN UNLOCK EXIT: %ERRORLEVEL%
echo === [ Pre-run DB unlock END ] ===

:: =============================================================================
:: SECTION 2 — FIND PACKAGE FOLDER (latest by creation time)
:: =============================================================================
echo === [ Finding package folder START ] ===
pushd %ACTC_CUSTOM_CONFIG_DIR%
for /f "delims=" %%i in ('dir /b /ad /t:c /od') do (
    echo Setting PACKFOLDER to %%i
    set "PACKFOLDER=%%i"
)
popd
echo Final PACKFOLDER: %PACKFOLDER%
if not defined PACKFOLDER (
    echo ERROR: No package folder found under %ACTC_CUSTOM_CONFIG_DIR%
    exit /b 1
)
echo === [ Finding package folder END ] ===

:: =============================================================================
:: SECTION 3 — COPY TEMPLATE FILES
:: =============================================================================
echo === [ Copy template files START ] ===

set "BMIDE_PKG=%TEMP%\mi8_deploy_data_model"
if exist "%BMIDE_PKG%" rmdir /s /q "%BMIDE_PKG%"
mkdir "%BMIDE_PKG%" || exit /b !ERRORLEVEL!

echo Extracting template zip...
echo n | "%TC_ROOT%\install\install\7za" x -aoa -bb2 -bd ^
    -o"%BMIDE_PKG%" ^
    "%ACTC_CUSTOM_CONFIG_DIR%\%PACKFOLDER%\artifacts\%PROJECT_TEMPLATE_NAME%_template.zip"

echo Copying template XML files to TC_DATA\model...
xcopy "%BMIDE_PKG%\install\%PROJECT_TEMPLATE_NAME%\%PROJECT_TEMPLATE_NAME%_template.xml"       "%TC_DATA%\model\"      /i /r /y /f || exit /b !ERRORLEVEL!
xcopy "%BMIDE_PKG%\install\%PROJECT_TEMPLATE_NAME%\%PROJECT_TEMPLATE_NAME%_dependency.xml"     "%TC_DATA%\model\"      /i /r /y /f || exit /b !ERRORLEVEL!
xcopy "%BMIDE_PKG%\install\%PROJECT_TEMPLATE_NAME%\lang\%PROJECT_TEMPLATE_NAME%_template_en_US.xml" "%TC_DATA%\model\lang\" /i /r /y /f || exit /b !ERRORLEVEL!
xcopy "%ACTC_CUSTOM_CONFIG_DIR%\%PACKFOLDER%\artifacts\client_%PROJECT_TEMPLATE_NAME%.properties" "%TC_DATA%\model\"    /i /r /y /f || exit /b !ERRORLEVEL!

if not exist "%TC_DATA%\model\icons\" mkdir "%TC_DATA%\model\icons\"

:: ── Icon zip handling ────────────────────────────────────────────────────────
set "ICON_ZIP=%ACTC_CUSTOM_CONFIG_DIR%\%PACKFOLDER%\artifacts\%PROJECT_TEMPLATE_NAME%_icons.zip"

echo ======================================
echo CHECKING ICON ZIP
echo ======================================

if exist "%ICON_ZIP%" (
    echo ICON ZIP FOUND - COPYING...
    xcopy "%ICON_ZIP%" "%TC_DATA%\model\icons\" /i /r /y /f
    if !ERRORLEVEL! NEQ 0 (
        echo ICON COPY FAILED
        exit /b 1
    )
    echo ICON ZIP COPIED
) else (
    echo NO ICON ZIP FOUND IN PACKAGE - SKIPPING
)

rmdir /s /q "%BMIDE_PKG%"
echo === [ Copy template files END ] ===

:: =============================================================================
:: SECTION 4 — EXTRACT DATA MODEL
:: =============================================================================
echo === [ Extract data model START ] ===
call "%TC_BIN%\business_model_extractor" ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% ^
    -outfile="%TC_DATA%\model\model.xml" || exit /b !ERRORLEVEL!
echo === [ Extract data model END ] ===

:: =============================================================================
:: SECTION 5 — GENERATE DELTA CHANGES
:: =============================================================================
echo === [ Generate delta changes START ] ===
call "%TC_BIN%\bmide_processtemplates" ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% ^
    -mode=server ^
    -dir="%TC_DATA%\model" ^
    -templates="%PROJECT_TEMPLATE_NAME%" ^
    -deployvalidation=yes || exit /b !ERRORLEVEL!
echo === [ Generate delta changes END ] ===

:: =============================================================================
:: SECTION 6 — PRE-DEPLOY  (schema regen + lock DB + clear locks)
:: =============================================================================
echo === [ Pre-deploy START ] ===
call "%TC_BIN%\install" -regen_schema_file ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
call "%TC_BIN%\install" -lock_db ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
call "%TC_BIN%\clearlocks" -assert_all_dead ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
echo === [ Pre-deploy END ] ===

:: =============================================================================
:: SECTION 7 — UPDATE DATA MODEL  (schema + localization)
:: =============================================================================
echo === [ Update data model START ] ===
call "%TC_BIN%\business_model_updater.exe" ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% ^
    -mode=upgrade -update=all -process=all ^
    -file="%TC_DATA%\model\delta.xml" ^
    -predelta -nolocalization || exit /b !ERRORLEVEL!
echo === [ Update data model END ] ===

echo === [ Update data model lang START ] ===
call "%TC_BIN%\business_model_updater.exe" ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% ^
    -mode=upgrade -update=localization -process=all ^
    -file="%TC_DATA%\model\lang\delta_lang.xml" || exit /b !ERRORLEVEL!
echo === [ Update data model lang END ] ===

:: =============================================================================
:: SECTION 8 — POST-DEPLOY
:: =============================================================================
echo === [ Post-deploy START ] ===

call "%TC_BIN%\install" -regen_schema_file ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
call "%TC_BIN%\install" -gen_xmit_file ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!

:: ── Smart add-vs-fullupdate: check if template is already registered ──────────
echo Checking if template [%PROJECT_TEMPLATE_NAME%] is already registered...
"%TC_BIN%\bmide_manage_templates" ^
    -u=%ACTC_DATA_LOAD_USER% ^
    -p=%ACTC_DATA_LOAD_USER_PWD% ^
    -g=%ACTC_DATA_LOAD_USER_GRP% ^
    -option=list > "%TEMP%\tc_tmpl_list.txt" 2>&1

findstr /I /C:"%PROJECT_TEMPLATE_NAME%" "%TEMP%\tc_tmpl_list.txt" >nul 2>&1
if !ERRORLEVEL!==0 (
    set "BMIDE_OPTION=fullupdate"
    echo Template [%PROJECT_TEMPLATE_NAME%] found in registry -^> using fullupdate
) else (
    set "BMIDE_OPTION=add"
    echo Template [%PROJECT_TEMPLATE_NAME%] NOT in registry -^> using add
)
del /f /q "%TEMP%\tc_tmpl_list.txt" 2>nul

call "%TC_BIN%\bmide_manage_templates" ^
    -u=%ACTC_DATA_LOAD_USER% ^
    -p=%ACTC_DATA_LOAD_USER_PWD% ^
    -g=%ACTC_DATA_LOAD_USER_GRP% ^
    -option=!BMIDE_OPTION! ^
    -templates="%PROJECT_TEMPLATE_NAME%" || exit /b !ERRORLEVEL!

call "%TC_BIN%\clearlocks" -assert_all_dead ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
call "%TC_BIN%\install" -unlock_db ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!

echo === [ Post-deploy END ] ===

:: =============================================================================
:: SECTION 9 — GENERATE TCPLMXML SCHEMA
:: =============================================================================
echo ==[ Generate TCPLMXML schema START ]==
call "%TC_BIN%\bmide_generatetcplmxmlschema.bat" ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
echo ==[ Generate TCPLMXML schema END ]==

:: =============================================================================
:: SECTION 10 — SETUP KNOWLEDGEBASE
:: =============================================================================
echo ==[ Setup knowledgebase START ]==
call "%TC_BIN%\bmide_setupknowledgebase.bat" ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% ^
    -regen=false || exit /b !ERRORLEVEL!
echo ==[ Setup knowledgebase END ]==

:: =============================================================================
:: SECTION 11 — MODEL TOOL
:: =============================================================================
echo ==[ Model tool START ]==
call "%TC_BIN%\bmide_modeltool.bat" ^
    -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% ^
    -tool=all -mode=update ^
    -target_dir="%TC_DATA%" ^
    -model_file="%TC_DATA%\model\model.xml" || exit /b !ERRORLEVEL!
echo ==[ Model tool END ]==

:: =============================================================================
:: SECTION 12 — TCSCHEMA TO SOLR SCHEMA TRANSFORM
:: =============================================================================
echo ==[ TcSchema To SolrSchema Transform START ]==
echo Resolved TC_SOLR_ROOT = [%TC_SOLR_ROOT%]
if not exist "%TC_SOLR_ROOT%\TcSchemaToSolrSchemaTransform.bat" (
    echo ERROR: TcSchemaToSolrSchemaTransform.bat not found at: %TC_SOLR_ROOT%
    exit /b 1
)
call "%TC_SOLR_ROOT%\TcSchemaToSolrSchemaTransform.bat" "%TC_DATA%\ftsi\solr_schema_files" || exit /b !ERRORLEVEL!
echo ==[ TcSchema To SolrSchema Transform END ]==

echo.
echo ============================================================
echo  BMIDE DEPLOY COMPLETE  ^|  Template: %PROJECT_TEMPLATE_NAME%
echo ============================================================
echo.

endlocal
exit /b 0