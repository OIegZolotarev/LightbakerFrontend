REM re2c -i и win_bison -label

re2c -i hammer_map.y > hammer_map.re2c.y
win_bison hammer_map.re2c.y -Wcounterexamples -o hammer_map.bison.cpp  