LoadBGM TestBattle.wav
LoadBGM MainBattle.wav
LoadMap TestField.x -s 100.0,100.0,100.0 -h
LoadMap TestField2.x -s 1000.0,100.0,100.0
LoadMecha enemyTest NormalRobot.amf -p 0.0,700.0,0.0 -t 1 -cc
LoadMecha player NormalRobot.amf -p 0.0,700.0,0.0 -pc
Play TestBattle.wav
Loop1
SkipIfFew 1 -m 1
GameOver
SkipIfFew 1 -e 1	
End Loop1
Play MainBattle.wav
Loop2
SkipIfFew 1 -e 4
LoadMecha enemy NormalRobot.amf -p 0.0,700.0,0.0 -t 1 -cc
SkipIfFew 1 -m 4
LoadMecha member NormalRobot.amf -p 0.0,700.0,0.0 -cc
SkipIfFew 1 -m 1
GameOver
End Loop2