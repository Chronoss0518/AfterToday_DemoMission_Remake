//LoadBGM Battle1.wav -l
//LoadBGM MainBattle1.wav -l -ls 220 -le 647
LoadMap TestField.x -s 100.0,100.0,100.0 -h
LoadMap TestField2.x -s 100.0,100.0,100.0
LoadMecha AirRobot.amf -u player -p 0.0,700.0,0.0 -pc -t 0
LoadMecha GuardianRobot.amf -u enemyTest -p 0.0,700.0,0.0 -t 1 -cc 1
Initialize
//Play Battle1.wav
Loop Loop1
SkipIfFew 1 1 -m
GameOver
SkipIfFew 1 1 -e
End Loop1
//Play MainBattle1.wav
Loop Loop2
SkipIfFew 1 4 -e
LoadMecha GuardianRobot.amf -u enemy -p 0.0,700.0,0.0 -t 1 -cc 1
SkipIfFew 1 4 -m
LoadMecha AirRobot.amf -u member -p 0.0,700.0,0.0 -cc 1
SkipIfFew 1 1 -m
GameOver
End Loop2