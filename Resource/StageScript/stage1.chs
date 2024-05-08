////戦闘BGMを追加//
LoadBGM reconnaissance.wav -l
//マップオブジェクトの追加//
LoadMap ExaminationHall.x -s 50.0,50.0,50.0 -h
//マップ上にメカを配置(Player)//
LoadMecha -u player -p 0.0,700.0,200.0 -r 0.0,180.0f,0.0 -pc -t 0  -l AirRobot.amf
//マップ上にメカを配置//
LoadMecha -l AirRobot.amf -u enemyTest -p 0.0,700.0,0.0 -t 1 -cc mob.cpu
//初期化が終わるまで待機//
Initialize
//Battle1.wavを再生//
Play reconnaissance.wav
SetControllerUsing false
Message 作戦開始。\n目標は、あの水色の機体だ。 --messenger operator --addFrame 0.1 --afterFrame 3.0 --stop --endDrawKeyFlg
Message 相手は速度重視の機体だ。\n気を抜くなよ。 --messenger operator --addFrame 0.1 --afterFrame 3.0 --stop --endDrawKeyFlg
Message ミッション開始だ --messenger operator --addFrame 0.1 --afterFrame 3.0
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
Message 目標の撃破を確認。\nよくやった。 --messenger operator --addFrame 0.1 --afterFrame 5.0 --stop --endDrawKeyFlg
Success