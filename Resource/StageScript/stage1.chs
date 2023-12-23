//戦闘BGMを追加//
LoadBGM Battle1.wav -l
//戦闘BGMを追加//
LoadBGM MainBattle1.wav -l -ls 220 -le 647
//マップオブジェクトの追加//
LoadMap TestField.x -s 100.0,100.0,100.0 -h
//マップオブジェクトの追加//
LoadMap TestField2.x -s 100.0,100.0,100.0
//マップ上にメカを配置(Player)//
LoadMecha AirRobot.amf -u player -p 0.0,200.0,0.0 -pc -t 0
//マップ上にメカを配置//
LoadMecha GuardianRobot.amf -u enemyTest -p 0.0,200.0,0.0 -t 1 -cc mob.cpu
//初期化が終わるまで待機//
Initialize
//Battle1.wavを再生//
Play Battle1.wav
//Controllerからの入力を受け付けるかの設定//
SetControllerUsing False
//Controllerからの入力を受け付けるかの設定//
SetControllerUsing True
//Loopの開始//
//Loop Loop1
//チームメンバーが1以上の場合は次のコードをスキップする//
SkipIfMore 1 0 -m
//ステージ失敗を通達する//
Failed
//敵のメンバーが1以上の場合は次のコードをスキップする//
SkipIfMore 1 0 -e
//Loopを繰り返す命令//
//End Loop1
//MainBattle1.wavを再生//
Play MainBattle1.wav
//Loopの開始//
Loop Loop2
//チームType1のメンバーが3以上の場合は次のコードをスキップする//
SkipIfLess 1 3 -t 1
LoadMecha GuardianRobot.amf -u enemy -p 60.0,700.0,60.0 -rp min-10.0,0.0,-10.0 max10.0,0.0,10.0 -t 1 -cc mob.cpu
//チームType0のメンバーが3以上の場合は次のコードをスキップする//
SkipIfLess 1 3 -t 0
LoadMecha AirRobot.amf -u member -p -60.0,700.0,-60.0 -rp min-10.0,0.0,-10.0 max10.0,0.0,10.0 -t 0 -cc mob.cpu
//チームメンバーが1以上の場合は次のコードをスキップする//
SkipIfMore 1 0 -m
//ステージ失敗を通達する//
Failed
//Loopを繰り返す命令//
End Loop2
Success