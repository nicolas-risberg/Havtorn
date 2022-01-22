call External\premake\premake5.exe vs2019
@echo.
@echo Checking license in files...
@echo off
for %%G in (.cpp, .h, .hlsl, .hlsli) do forfiles /p Source /s /m *%%G /C "cmd /c findstr /G:%~dp0License.txt @path >nul 2>&1 & if errorlevel 1 ((type %~dp0License.txt) > license.txt.new & type @path >> license.txt.new & move /y license.txt.new @path & if exist license.txt.new del license.txt.new)"
@echo on
@echo.
@echo Done.
@echo off
@echo.
PAUSE