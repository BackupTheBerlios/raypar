@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by SERVER.HPJ. >"hlp\SERVER.hm"
echo. >>"hlp\SERVER.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\SERVER.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\SERVER.hm"
echo. >>"hlp\SERVER.hm"
echo // Prompts (IDP_*) >>"hlp\SERVER.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\SERVER.hm"
echo. >>"hlp\SERVER.hm"
echo // Resources (IDR_*) >>"hlp\SERVER.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\SERVER.hm"
echo. >>"hlp\SERVER.hm"
echo // Dialogs (IDD_*) >>"hlp\SERVER.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\SERVER.hm"
echo. >>"hlp\SERVER.hm"
echo // Frame Controls (IDW_*) >>"hlp\SERVER.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\SERVER.hm"
REM -- Make help for Project SERVER


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\SERVER.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\SERVER.hlp" goto :Error
if not exist "hlp\SERVER.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\SERVER.hlp" Debug
if exist Debug\nul copy "hlp\SERVER.cnt" Debug
if exist Release\nul copy "hlp\SERVER.hlp" Release
if exist Release\nul copy "hlp\SERVER.cnt" Release
echo.
goto :done

:Error
echo hlp\SERVER.hpj(1) : error: Problem encountered creating help file

:done
echo.

