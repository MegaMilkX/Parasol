@echo off
setlocal enableextensions enabledelayedexpansion
set SOURCES=
for /F %%A in ('dir /b /S *.cpp') do set SOURCES=!SOURCES! "%%A"