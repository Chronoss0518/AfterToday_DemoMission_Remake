LoadBGM Battle1.wav -l
LoadBGM MainBattle1.wav -l -ls 220 -le 647
LoadMap TestField.x -s 100.0,100.0,100.0 -h
LoadMap TestField2.x -s 100.0,100.0,100.0
LoadMecha AirRobot.amf -u player -p 0.0,200.0,0.0 -pc -t 0
LoadMecha GuardianRobot.amf -u enemyTest -p 0.0,200.0,0.0 -t 1 -cc mob.cpu
Initialize
Play Battle1.wav
SetControllerUsing False
SetControllerUsing True
Loop Loop1
SkipIfMore 1 0 -m
GameOver
SkipIfMore 1 0 -e
End Loop1
Play MainBattle1.wav
Loop Loop2
SkipIfLess 1 3 -t 1
LoadMecha GuardianRobot.amf -u enemy -p 0.0,700.0,0.0 -rp min-10.0,0.0,-10.0 max10.0,0.0,10.0 -t 1 -cc mob.cpu
SkipIfLess 1 3 -t 0
LoadMecha AirRobot.amf -u member -p 0.0,700.0,0.0 -rp min-10.0,0.0,-10.0 max10.0,0.0,10.0 -t 0 -cc mob.cpu
SkipIfMore 1 0 -m
GameOver
End Loop2