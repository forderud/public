set PATH=%PATH%;C:\Qt\5.8\msvc2015_64\bin
set PATH=%PATH%;C:\Program Files\Cmake\bin

cmake.exe -G "Visual Studio 14 Win64" -DCMAKE_CONFIGURATION_TYPES=Release;Debug

pause
