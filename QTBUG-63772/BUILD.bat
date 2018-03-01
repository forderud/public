@echo off

set "PATH=%QT_ROOT_64%\bin;%PATH%"
call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" amd64

:: generate project file
qmake -tp vc

:: start visual studio
QTBUG-63772.vcxproj

pause
