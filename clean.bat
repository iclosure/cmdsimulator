@echo off

cd /d "%~dp0"

set PRO_DIR="%~dp0"

rem ===

rd /s /q "%PRO_DIR%\.vscode"
rd /s /q "%PRO_DIR%\bin"
rd /s /q "%PRO_DIR%\lib"

del /f "%PRO_DIR%\*.user*"

::end
rem pause