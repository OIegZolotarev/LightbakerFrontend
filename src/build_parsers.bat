@echo off

echo Building FGD parser...
cd hammer_fgd
call build.bat

echo Building MAP parser...
cd.. 
cd hammer_map
call build.bat
cd..