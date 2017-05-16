z@echo off

cd /d "%~dp0"

set QTDIR="C:\Qt\Qt5.4.2\5.4\msvc2010_opengl"
set QTCREATOR_DIR="C:\Qt\Qt5.4.2\Tools\QtCreator"

set PRO_DIR="%~dp0"

if not exist "%QTDIR%" goto end
if not exist "%QTCREATOR_DIR%" goto end
if not exist "%PRO_DIR%" goto end

rem ====

copy "%QTCREATOR_DIR%\bin\qt.conf" "%PRO_DIR%\bin\"

rem ====

if  exist "%PRO_DIR%\bin\plugins\" (rd /s /q "%PRO_DIR%\bin\plugins\")

rem ====

copy "%QTDIR%\bin\icudt5*.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\icuin5*.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\icuuc5*.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\Qt5Core.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\Qt5Gui.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\Qt5OpenGL.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\Qt5PrintSupport.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\Qt5Sql.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\Qt5Svg.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\Qt5Widgets.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\Qt5Xml.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\Qt5XmlPattern.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\Qt5Network.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\Qt5Qml.dll" "%PRO_DIR%\bin\"
copy "%QTDIR%\bin\Qt5Quick.dll" "%PRO_DIR%\bin\"

rem ====

xcopy /y /e /s /h "%QTDIR%\plugins\iconengines\q*.dll" "%PRO_DIR%\bin\plugins\iconengines\"
del /f "%PRO_DIR%\bin\plugins\iconengines\q*d.dll"

xcopy /y /e /s /h "%QTDIR%\plugins\imageformats\q*.dll" "%PRO_DIR%\bin\plugins\imageformats\"
del /f "%PRO_DIR%\bin\plugins\imageformats\q*d.dll"

xcopy /y /e /s /h "%QTDIR%\plugins\platforms\q*.dll" "%PRO_DIR%\bin\plugins\platforms\"
del /f "%PRO_DIR%\bin\plugins\platforms\q*d.dll"

xcopy /y /e /s /h "%QTDIR%\plugins\sqldrivers\q*.dll" "%PRO_DIR%\bin\plugins\sqldrivers\"
del /f "%PRO_DIR%\bin\plugins\sqldrivers\q*d.dll"

xcopy /y /e /s /h "%QTDIR%\qml\QtQuick.2\*" "%PRO_DIR%\bin\qml\QtQuick.2\"
del /f "%PRO_DIR%\bin\qml\QtQuick.2\q*d.dll"
del /f "%PRO_DIR%\bin\qml\QtQuick.2\*.pdb"


::end
rem pause