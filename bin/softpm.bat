@echo off

set "current_dir=%~dp0"

set "current_dir=%current_dir:~0,-1%"

for %%I in ("%current_dir%") do set "parent_dir=%%~dpI"

set "parent_dir=%parent_dir:~0,-1%"

set PYTHONPATH=%cd%\packages\windows;%PYTHONPATH%

python "%parent_dir%\scripts\softpm-windows.py" %*
