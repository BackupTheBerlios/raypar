@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by CLIENT.HPJ. >"hlp\CLIENT.hm"
echo. >>"hlp\CLIENT.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\CLIENT.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\CLIENT.hm"
echo. >>"hlp\CLIENT.hm"
echo // Prompts (IDP_*) >>"hlp\CLIENT.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\CLIENT.hm"
echo. >>"hlp\CLIENT.hm"
echo // Resources (IDR_*) >>"hlp\CLIENT.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\CLIENT.hm"
echo. >>"hlp\CLIENT.hm"
echo // Dialogs (IDD_*) >>"hlp\CLIENT.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\CLIENT.hm"
echo. >>"hlp\CLIENT.hm"
echo // Frame Controls (IDW_*) >>"hlp\CLIENT.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\CLIENT.hm"
REM -- Make help for Project CLIENT


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\CLIENT.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\CLIENT.hlp" goto :Error
if not exist "hlp\CLIENT.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\CLIENT.hlp" Debug
if exist Debug\nul copy "hlp\CLIENT.cnt" Debug
if exist Release\nul copy "hlp\CLIENT.hlp" Release
if exist Release\nul copy "hlp\CLIENT.cnt" Release
echo.
goto :done

:Error
echo hlp\CLIENT.hpj(1) : error: Problem encountered creating help file

:done
echo.
