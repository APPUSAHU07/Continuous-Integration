@echo on
setlocal enabledelayedexpansion

echo === [ Pre-run DB unlock START ] ===
call "%TC_DATA%\tc_profilevars.bat"
call "%TC_BIN%\install" -unlock_db -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP%
echo PRE-RUN UNLOCK EXIT: %ERRORLEVEL%
echo === [ Pre-run DB unlock END ] ===

echo === [ Finding package folder START ] ===
pushd %ACTC_CUSTOM_CONFIG_DIR%
for /f "delims=" %%i IN ('dir /b /ad /t:c /od') DO (
    echo "Setting PACKFOLDER to %%i"
    SET PACKFOLDER=%%i
)
popd
echo Final PACKFOLDER: %PACKFOLDER%

echo === [ Copy template files START ] ===
set BMIDE_PKG=%TEMP%\mi8_deploy_data_model
if exist "%BMIDE_PKG%" rmdir /s /q "%BMIDE_PKG%"
mkdir "%BMIDE_PKG%" || exit /b !ERRORLEVEL!

echo n | "%TC_ROOT%\install\install\7za" x -aoa -bb2 -bd -o"%BMIDE_PKG%" "%ACTC_CUSTOM_CONFIG_DIR%\%PACKFOLDER%\artifacts\%PROJECT_TEMPLATE_NAME%_template.zip"

xcopy "%BMIDE_PKG%\install\%PROJECT_TEMPLATE_NAME%\%PROJECT_TEMPLATE_NAME%_template.xml" "%TC_DATA%\model\" /i /r /y /f || exit /b !ERRORLEVEL!
xcopy "%BMIDE_PKG%\install\%PROJECT_TEMPLATE_NAME%\%PROJECT_TEMPLATE_NAME%_dependency.xml" "%TC_DATA%\model\" /i /r /y /f || exit /b !ERRORLEVEL!
xcopy "%BMIDE_PKG%\install\%PROJECT_TEMPLATE_NAME%\lang\%PROJECT_TEMPLATE_NAME%_template_en_US.xml" "%TC_DATA%\model\lang\" /i /r /y /f || exit /b !ERRORLEVEL!
xcopy "%ACTC_CUSTOM_CONFIG_DIR%\%PACKFOLDER%\artifacts\client_%PROJECT_TEMPLATE_NAME%.properties" "%TC_DATA%\model\" /i /r /y /f || exit /b !ERRORLEVEL!
if not exist "%TC_DATA%\model\icons\" mkdir "%TC_DATA%\model\icons\"

set ICON_ZIP=%ACTC_CUSTOM_CONFIG_DIR%\%PACKFOLDER%\artifacts\%PROJECT_TEMPLATE_NAME%_icons.zip

echo ======================================
echo CHECKING ICON ZIP
echo ======================================

if exist "%ICON_ZIP%" (

    FOR %%F IN ("%ICON_ZIP%") DO (

        echo ICON ZIP FOUND
        echo FILE = %%F
        echo SIZE = %%~zF bytes

        REM Empty/broken zip check - threshold 500 bytes covers BMIDE empty zip overhead (~236 bytes)
        IF %%~zF LEQ 500 (

            echo INVALID ICON ZIP DETECTED - SIZE %%~zF bytes
            echo DELETING EMPTY ICON ZIP FROM PACKAGE...
            del /f /q "%ICON_ZIP%"

            IF EXIST "%ICON_ZIP%" (
                echo FAILED TO DELETE ICON ZIP
                exit /b 1
            )
            echo EMPTY ICON ZIP REMOVED FROM PACKAGE

            echo REMOVING STALE ICON ZIP FROM MODEL ICONS DIR IF PRESENT...
            del /f /q "%TC_DATA%\model\icons\%PROJECT_TEMPLATE_NAME%_icons.zip"
            echo STALE ICON ZIP CLEANUP DONE

        ) ELSE (

            echo VALID ICON ZIP FOUND
            echo COPYING ICON ZIP...

            xcopy "%%F" "%TC_DATA%\model\icons\" /i /r /y /f

            IF !ERRORLEVEL! NEQ 0 (
                echo ICON COPY FAILED
                exit /b 1
            )

            echo ICON ZIP COPIED
        )
    )

) ELSE (

    echo NO ICON ZIP FOUND
    echo REMOVING STALE ICON ZIP FROM MODEL ICONS DIR IF PRESENT...
    del /f /q "%TC_DATA%\model\icons\%PROJECT_TEMPLATE_NAME%_icons.zip"
    echo STALE ICON ZIP CLEANUP DONE
)

rmdir /s /q "%BMIDE_PKG%"
echo === [ Copy template files END ] ===

echo === [ Extract data model START ] ===
call "%TC_BIN%\business_model_extractor" -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% -outfile="%TC_DATA%\model\model.xml" || exit /b !ERRORLEVEL!
echo === [ Extract data model END ] ===

echo === [ Generate delta changes START ] ===
call "%TC_BIN%\bmide_processtemplates" -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% -mode=server -dir="%TC_DATA%\model" -templates="%PROJECT_TEMPLATE_NAME%" -deployvalidation=yes || exit /b !ERRORLEVEL!
echo === [ Generate delta changes END ] ===

echo === [ Pre-deploy START ] ===
call "%TC_BIN%\install" -regen_schema_file -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
call "%TC_BIN%\install" -lock_db -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
call "%TC_BIN%\clearlocks" -assert_all_dead -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
echo === [ Pre-deploy END ] ===

echo === [ Update data model START ] ===
call "%TC_BIN%\business_model_updater.exe" -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% -mode=upgrade -update=all -process=all -file="%TC_DATA%\model\delta.xml" -predelta -nolocalization || exit /b !ERRORLEVEL!
echo === [ Update data model END ] ===

echo === [ Update data model lang START ] ===
call "%TC_BIN%\business_model_updater.exe" -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% -mode=upgrade -update=localization -process=all -file="%TC_DATA%\model\lang\delta_lang.xml" || exit /b !ERRORLEVEL!
echo === [ Update data model lang END ] ===

echo Waiting 60 seconds for TC sessions to clear before post-deploy...
REM timeout /t 60 /nobreak
echo Wait complete.

echo === [ Post-deploy START ] ===
call "%TC_BIN%\install" -regen_schema_file -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
call "%TC_BIN%\install" -gen_xmit_file -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
echo call "%TC_BIN%\bmide_manage_templates" -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% -option=fullupdate -templates="%PROJECT_TEMPLATE_NAME%" || exit /b !ERRORLEVEL!
call "%TC_BIN%\bmide_manage_templates" -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% -option=fullupdate -templates="%PROJECT_TEMPLATE_NAME%" || exit /b !ERRORLEVEL!
echo call "%TC_BIN%\bmide_manage_templates" -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% -option=fullupdate -templates="%PROJECT_TEMPLATE_NAME%" || exit /b !ERRORLEVEL!
call "%TC_BIN%\clearlocks" -assert_all_dead -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
call "%TC_BIN%\install" -unlock_db -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
echo === [ Post-deploy END ] ===

echo ==[ Generate TCPLMML schema START ]==
call "%TC_BIN%\bmide_generatetcplmxmlschema.bat" -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% || exit /b !ERRORLEVEL!
echo ==[ Generate TCPLMML schema END ]==

echo ==[ Setup knowledgebase START ]==
call "%TC_BIN%\bmide_setupknowledgebase.bat" -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% -regen=false || exit /b !ERRORLEVEL!
echo ==[ Setup knowledgebase END ]==

echo ==[ Model tool START ]==
call "%TC_BIN%\bmide_modeltool.bat" -u=%ACTC_DATA_LOAD_USER% -p=%ACTC_DATA_LOAD_USER_PWD% -g=%ACTC_DATA_LOAD_USER_GRP% -tool=all -mode=update -target_dir="%TC_DATA%" -model_file="%TC_DATA%"\model\model.xml || exit /b !ERRORLEVEL!
echo ==[ Model tool END ]==

echo ==[ TcSchema To SolrSchema Transform START ]==
call "%TC_SOLR_ROOT%\TcSchemaToSolrSchemaTransform.bat" "%TC_DATA%\ftsi\solr_schema_files" || exit /b !ERRORLEVEL!
echo ==[ TcSchema To SolrSchema Transform END ]==