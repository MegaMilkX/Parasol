@echo off
if not defined DevEnvDir (call "%VS140COMNTOOLS%..\..\VC\vcvarsall")

REM Collect all source files
setlocal enableextensions enabledelayedexpansion
set SOURCES=
for /F %%A in ('dir /b /S *.cpp') do set SOURCES=!SOURCES! "%%A"

set LIBRARIES=kernel32.lib ^
user32.lib ^
gdi32.lib ^
winspool.lib ^
comdlg32.lib ^
advapi32.lib ^
shell32.lib ^
ole32.lib ^
oleaut32.lib ^
uuid.lib ^
odbc32.lib ^
odbccp32.lib ^
opengl32.lib ^
winmm.lib ^
Shlwapi.lib ^
legacy_stdio_definitions.lib ^
freetype26MT.lib ^
SOIL.lib

set /p BUILDINDEX=<build.txt

set BUILDDIR=build\%BUILDINDEX%

mkdir %BUILDDIR%
mkdir obj
pushd obj
cl /I F:\libs\openssl-1.0.2d-vs2013\include ^
/I F:\libs\SQLITE3\sqlite\H ^
/I F:\milk\repos\ExcelFormat\include ^
/I F:\libs\koicxx ^
/I C:\Users\milk\Documents\libs\MetaTraderManagerAPI ^
/I F:\libs\MetaTraderServerAPI\include ^
/I C:\Users\milk\Documents\libs\t4b-license\include ^
/I F:\milk\repos\t4b-lib\include ^
/I F:\libs\boost_1_55_0 ^
/I F:\milk\repos\websocketpp ^
/I ..\external ^
/I ..\include ^
/I ..\src\aurora\external\freetype\include ^
/D "_UNICODE" ^
/D "UNICODE" ^
/GS ^
/GL ^
/analyze- ^
/W3 ^
/Gy ^
/Zc:wchar_t ^
/EHsc ^
/MT ^
/WX- ^
/Zc:forScope ^
/Gd ^
/Oy- ^
/Oi ^
/Gm- ^
/O2 ^
/nologo ^
%SOURCES% ^
/link ^
/OUT:"..\%BUILDDIR%\game.exe" ^
%LIBRARIES% ^
/MACHINE:X86 ^
/OPT:REF ^
/SAFESEH ^
/OPT:ICF ^
/ERRORREPORT:PROMPT ^
/NOLOGO ^
/LIBPATH:"F:\libs\openssl-1.0.2d-vs2013\lib" ^
/LIBPATH:"F:\libs\SQLITE3\Release" ^
/LIBPATH:"F:\milk\repos\ExcelFormat\Release" ^
/LIBPATH:"F:\libs\boost_1_55_0\stage\lib" ^
/LIBPATH:"..\external" ^
/LIBPATH:"..\lib" ^
/TLBID:1

if %ERRORLEVEL% GEQ 1 goto failed
    popd
    set /a BUILDINDEX=%BUILDINDEX%+1
    >build.txt echo %BUILDINDEX%
:failed
    popd
