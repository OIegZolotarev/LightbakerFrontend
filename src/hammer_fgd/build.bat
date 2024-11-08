REM re2c -i и win_bison -label

re2c hammer_fgd.y > hammer_fgd.re2c.y
win_bison hammer_fgd.re2c.y -o hammer_fgd.bison.cpp  