//�퓬BGM��ǉ�//
LoadBGM Battle1.wav -l
//�퓬BGM��ǉ�//
LoadBGM MainBattle1.wav -l -ls 220 -le 647
//�}�b�v�I�u�W�F�N�g�̒ǉ�//
LoadMap TestField.x -s 100.0,100.0,100.0 -h
//�}�b�v�I�u�W�F�N�g�̒ǉ�//
LoadMap TestField2.x -s 100.0,100.0,100.0
//�}�b�v��Ƀ��J��z�u(Player)//
LoadMecha AirRobot.amf -u player -p 0.0,200.0,0.0 -pc -t 0
//�}�b�v��Ƀ��J��z�u//
LoadMecha GuardianRobot.amf -u enemyTest -p 0.0,200.0,0.0 -t 1 -cc mob.cpu
//���������I���܂őҋ@//
Initialize
//Battle1.wav���Đ�//
Play Battle1.wav
//Controller����̓��͂��󂯕t���邩�̐ݒ�//
SetControllerUsing False
//Controller����̓��͂��󂯕t���邩�̐ݒ�//
SetControllerUsing True
//Loop�̊J�n//
//Loop Loop1
//�`�[�������o�[��1�ȏ�̏ꍇ�͎��̃R�[�h���X�L�b�v����//
SkipIfMore 1 0 -m
//�X�e�[�W���s��ʒB����//
Failed
//�G�̃����o�[��1�ȏ�̏ꍇ�͎��̃R�[�h���X�L�b�v����//
SkipIfMore 1 0 -e
//Loop���J��Ԃ�����//
//End Loop1
//MainBattle1.wav���Đ�//
Play MainBattle1.wav
//Loop�̊J�n//
Loop Loop2
//�`�[��Type1�̃����o�[��3�ȏ�̏ꍇ�͎��̃R�[�h���X�L�b�v����//
SkipIfLess 1 3 -t 1
LoadMecha GuardianRobot.amf -u enemy -p 60.0,700.0,60.0 -rp min-10.0,0.0,-10.0 max10.0,0.0,10.0 -t 1 -cc mob.cpu
//�`�[��Type0�̃����o�[��3�ȏ�̏ꍇ�͎��̃R�[�h���X�L�b�v����//
SkipIfLess 1 3 -t 0
LoadMecha AirRobot.amf -u member -p -60.0,700.0,-60.0 -rp min-10.0,0.0,-10.0 max10.0,0.0,10.0 -t 0 -cc mob.cpu
//�`�[�������o�[��1�ȏ�̏ꍇ�͎��̃R�[�h���X�L�b�v����//
SkipIfMore 1 0 -m
//�X�e�[�W���s��ʒB����//
Failed
//Loop���J��Ԃ�����//
End Loop2
Success