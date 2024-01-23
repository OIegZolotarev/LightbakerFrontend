REM re2c -i и win_bison -label

re2c -i hammer_map.y > hammer_map.re2c.y
win_bison hammer_map.re2c.y -Wcounterexamples -o hammer_map.bison.cpp  

clang-format hammer_map.bison.cpp --style=file > temp.cpp
echo y | del hammer_map.bison.cpp
ren temp.cpp hammer_map.bison.cpp