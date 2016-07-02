@echo off
set /p BUILD=<build.txt
@echo %BUILD%
set /a BUILD=%BUILD%+1
>build.txt echo %BUILD%
