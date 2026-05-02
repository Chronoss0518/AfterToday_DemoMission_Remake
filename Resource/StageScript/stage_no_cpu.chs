//戦闘BGMを追加//
LoadBGM reconnaissance.wav -l
//マップオブジェクトの追加//
LoadMap ExaminationHall.x -s 50.0,50.0,50.0 -h
//マップ上にメカを配置(Player)//
LoadMecha -u player -p 0.0,700.0,200.0 -r 0.0,180.0f,0.0 -pc -t 0
//マップ上にメカを配置//
LoadMecha -l NormalRobot.amf -u enemyTest -p 0.0,700.0,0.0 -t 1
//初期化が終わるまで待機//
Initialize
//Battle1.wavを再生//
Play reconnaissance.wav
SetControllerUsing true
//Loopの開始//
Loop Loop1
//チームメンバーが1以上の場合は次のコードをスキップする//
SkipIfSmaller 2 0 -m
//ステージ失敗を通達する//
SetControllerUsing false
Failed
//敵のメンバーが1以上の場合は次のコードをスキップする//
SkipIfMore 1 0 -e
//Loopを繰り返す命令//
End Loop1
Success