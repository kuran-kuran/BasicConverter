// �\�t�g�n�E�X�E�l�����p�̃p�b�`
// Basic.cpp��MainLoop_Setup()��JikenPatch()�����s���Ă�������
#include "Command.hpp"
#include "Executer.hpp"
#include "Basic.hpp"

#ifndef L11410_06
#define L11410_06 0
#endif
#ifndef L11430_01
#define L11430_01 0
#endif

// 0xF6A2 0: 5: �}�T�Ɏ��̋@�����Ă���ƕ�����, 6: �������������Ƃ���ł�
// 0xFFEE 0: 1: �i�C�t����`�o�̃V�����������
// 0xFFEF 0: 1: �J���_����J�M���L�N, 2: �J���_����J�M�����
// 0xFFF0 0: ���Ă��� 1: �h�A�܂��̓J�x���J���Ă���
// 0xFFF1 0: �В���1�x�������Ă��Ȃ�, 1:��x������, 2:�G���x�[�^�������Ă������
// 0xFFF2 0: �����S�ɂ��Ēm��Ȃ�, 1: �}�T�Ƀ����S�ɂ��ĕ�����
// 0xFFF3 1: 1�K, 2: 2�K
// 0xFFF4 0: �t���b�s�[�f�B�X�N�������Ă��Ȃ�, 1:�t���b�s�[�f�B�X�N�������Ă���
// 0xFFF5 0: �p�\�R�������ĂȂ� 1: �p�\�R��������
// 0xFFF6 0: �p�\�R�������ĂȂ� 1: �p�\�R��������
// 0xFFF7 0: �`�o�Ƀn���o�C�L�����Ă���ƕ����Ă��Ȃ�, 1: �`�o�Ƀn���o�C�L�����Ă���ƕ���
// 0xFFF8 0: �����l 1:�}�T�Ƀ~�g�ɂ��ĕ�����, 2: �}�T�Ɏ��̋@�����Ă���ƕ�����, 3: �}�T�ɉ��̂ɋC���t�����~�g�ɂ��ĕ���, 4: �}�T�ɉ��̂ɂ��ĕ���
// 0xFFF9 0: �L�~�K�L���~�g�ɂ��ĉB���Ă��� 1: �L�~�K�L���~�g�ɂ��Đ����ɘb��
// 0xFFFA 0: 1: �J���_�ɃP���J�ɂ��ĕ���
// 0xFFFB 0: 1: �R�E�ɃP���J�ɂ��ĕ���
// 0xFFFC L: �������l���Ȃ� �����,ϻ,���,�ж޷,��,��,����,�ϴ,�ɳ,�޹�
// 0xFFFD K: �V�i���I�̌���
// 0xFFFE �J�[�\��X���W�ۑ�
// 0xFFFF �J�[�\��Y���W�ۑ�

//�В��ɘb�𕷂��i�ꉞ�j�B
//���꒲���B�f�B�X�N���E���B
//�p�\�R�����g���B
//�`�o�ɔ̔��@�ɂ��ĕ����B
//�}�T��₢�l�߂�B
//�L�~�K�L��₢�l�߂�B
//�J���_�ɃP���J�ɂ��ĕ����B
//�J���_�ɃJ�M�𕷂��A���B
//���ɂ��J����B
//�i�C�t�𒲂ׂ�B
//�`�o�Ƀi�C�t�̎��𕷂��B
//�Ɛl�𐄗��B

// ���\��
void DrawArrow(dms::String* parameter)
{
	int dir = (*parameter)[0]; // ���̕��� (0:��, 8:��, 16:��, 24:��
	int color = (*parameter)[1]; // ���̐F (2:��, 6:���F)
	int y = (*parameter)[2]; // Y���W
	int x = (*parameter)[3]; // X���W
	int addX = color == 6 ? 3 : x== 58 ? 3 : 0;
	int data5 = (*parameter)[4];
	unsigned char data = 0x80 >> data5;
	unsigned char arrowPattern[] = {32,112,168,32,32,32,32,32, 32,32,32,32,32,168,112,32, 0,0,32,16,248,16,32,0, 0,0,32,64,248,64,32,0};
	Screen& screen = Executer::GetInstance()->GetScreen();
	static unsigned int colorTable[8] = { 0xFF000000, 0xFF0000FF, 0xFFFF0000, 0xFFFF00FF, 0xFF00FF00, 0xFF00FFFF, 0xFFFFFF00, 0xFFFFFFFF };
	for(int i = 0; i < 8; ++ i)
	{
		int drawX = x * 4 + addX;
		int drawY = y + i;
		unsigned char drawPattern = arrowPattern[dir + i];
		for(int j = 0; j < 8; ++ j)
		{
			unsigned int dot = (drawPattern << j) & 0x80;
			if(dot > 0)
			{
				screen.DrawPoint(drawX + j, drawY, colorTable[color], 0xFFFFFFFF);
			}
		}
	}
	Executer::GetInstance()->Poke(dms::Variable(0xF47D), data);
}

// �l������
int Actor(dms::String& input)
{
	std::vector<dms::String> actors =  {"","�����","ϻ","���","�ж޷","��","��","����","�ϴ","�ɳ","�޹�"};
	for(int i = 1; i < static_cast<int>(actors.size()); ++ i)
	{
		if(input.find(&actors[i][0], 0) != dms::String::npos)
		{
			// ������
			return i;
		}
	}
	return 0;
}

// 205 �A�z�o�J����
void Scenario205(dms::String& input, int type, int& k, int& l)
{
	if(input.find("��", 0) != -1)
	{
		k = 1;
	}
	if(input.find("�޶", 0) != -1)
	{
		k = 1;
	}
}

// 320 �s���Ȃ���������
void Scenario320(dms::String& input, int type, int& k, int& l)
{
	if(input.find("��", 0) != -1)
	{
		if(input.find("з�", 0) != -1)
		{
			k = 1;
		}
		else if(input.find("����", 0) != -1)
		{
			k = 2;
		}
		else if(input.find("ϴ", 0) != -1)
		{
			k = 3;
		}
		else if(input.find("���", 0) != -1)
		{
			k = 4;
		}
	}
}

// 1150 �����ʒu
void Scenario1150(dms::String& input, int type, int& k, int& l)
{
	dms::Variable doorFlag = Executer::GetInstance()->Peek(0xFFF0); // �h�A���J���Ă��邩
	if(input.find("��", 0) != -1)
	{
		if(input.find("�ޱ", 0) != -1)
		{
			if(doorFlag == 0)
			{
				k = 1;
			}
			else
			{
				k = 2;
			}
		}
	}
	if(input.find("��", 0) != -1)
	{
		if((doorFlag == 1) && (input.find("����", 0) != -1))
		{
			k = 3;
		}
		if(input.find("���", 0) != -1)
		{
			k = 4;
		}
		if(input.find("ϴ", 0) != -1)
		{
			k = 5;
		}
	}
}

// 1420 �В�
void Scenario1420(dms::String& input, int type, int& k, int& l)
{
	dms::Variable elevatorFlag = Executer::GetInstance()->Peek(0xFFF1); // 0:�В���1�x�������Ă��Ȃ�, 1:��x������, 2:�G���x�[�^�������Ă������
	dms::Variable appleFlag = Executer::GetInstance()->Peek(0xFFF2); // 0: �}�T�Ƀ����S�ɂ��ĕ����ĂȂ� 1: ������
	if(input.find("�����", 0) != -1)
	{
		k = 1;
	}
	else if(input.find("�޹�", 0) != -1)
	{
		k = 2;
	}
	else if((input.find("��", 0) != -1) || (input.find("���", 0) != -1))
	{
		if((appleFlag == 1) && (input.find("�ݺ�", 0) != -1))
		{
			k = 3;
		}
		else if(input.find("�ؽ��", 0) != -1)
		{
			k = 4;
		}
		else
		{
			k = 5;
			l = Actor(input);
		}
	}
	else if(input.find("��", 0) != -1)
	{
		if(input.find("з�", 0) != -1)
		{
			if(elevatorFlag == 2)
			{
				k = 6;
			}
			else
			{
				k = 7;
			}
		}
		else if(input.find("���", 0) != -1)
		{
			k = 8;
		}
	}
	else if((input.find("�����", 0) != -1) || (input.find("����", 0) != -1))
	{
		k = 9;
	}
}

// 1840 �G���x�[�^�[�O
void Scenario1840(dms::String& input, int type, int& k, int& l)
{
	dms::Variable floor = Executer::GetInstance()->Peek(0xFFF3); // ���K�ɂ��邩
	if(input.find("��", 0) != -1)
	{
		if(floor == 1)
		{
			// 1�K�ɂ���
			if(input.find("з�", 0) != -1)
			{
				k = 2;
			}
			else if(input.find("����", 0) != -1)
			{
				k = 4;
			}
		}
		else
		{
			// 2�K�ɂ���
			if(input.find("з�", 0) != -1)
			{
				k = 3;
			}
			else if(input.find("����", 0) != -1)
			{
				k = 5;
			}
			else if(input.find("���", 0) != -1)
			{
				k = 6;
			}
		}
	}
	else if(((input.find("����", 0) != -1) || (input.find("����", 0) != -1)) && (input.find("��", 0) != -1))
	{
		k = 1;
	}
}

// 1965 ����
void Scenario1965(dms::String& input, int type, int& k, int& l)
{
	if((input.find("���", 0) != -1) || (input.find("��", 0) != -1))
	{
		k = 1;
	}
	else if(input.find("��", 0) != -1)
	{
		if(input.find("з�", 0) != -1)
		{
			k = 2;
		}
		else if(input.find("����", 0) != -1)
		{
			k = 3;
		}
		else if(input.find("���", 0) != -1)
		{
			k = 4;
		}
	}
	else if(input.find("��", 0) != -1)
	{
		if(input.find("�ި��", 0) != -1)
		{
			dms::Variable diskFlag = Executer::GetInstance()->Peek(0xFFF4); // �t���b�s�[�f�B�X�N�������Ă��邩
			if(diskFlag == 0)
			{
				k = 5;
			}
		}
	}
	else if(input.find("����", 0) != -1)
	{
		if(input.find("���", 0) != -1)
		{
			k = 6;
		}
		else if(input.find("��", 0) != -1)
		{
			k = 7;
		}
	}
}

// 2050 �}�T
void Scenario2050(dms::String& input, int type, int& k, int& l)
{
	if(input.find("��", 0) != -1)
	{
		if(input.find("����", 0) != -1)
		{
			k = 1;
		}
		else if(input.find("з�", 0) != -1)
		{
			k = 2;
		}
		else if(input.find("���", 0) != -1)
		{
			k = 11;
		}
	}
	else if(input.find("¶", 0) != -1)
	{
		if((input.find("ϲ��", 0) != -1) || (input.find("���߭��", 0) != -1) || (input.find("�߿��", 0) != -1))
		{
			dms::Variable diskFlag = Executer::GetInstance()->Peek(0xFFF4); // �t���b�s�[�f�B�X�N�������Ă��邩
			if(diskFlag == 1)
			{
				k = 3;
			}
			else
			{
				k = 4;
			}
		}
	}
	else if((input.find("���", 0) != -1) || (input.find("��", 0) != -1))
	{
		k = -1;
		if(input.find("��خ�", 0) != -1)
		{
			dms::Variable flag = Executer::GetInstance()->Peek(0xFFF8);
			if(flag >= 3)
			{
				k = 7;
			}
			else
			{
				k = 8;
			}
		}
		else if(input.find("���޲�", 0) != -1)
		{
			dms::Variable vendingMachineFlag = Executer::GetInstance()->Peek(0xFFF7);
			dms::Variable flag8 = Executer::GetInstance()->Peek(0xFFF8);
			if((vendingMachineFlag == 1) && (flag8 == 1))
			{
				Executer::GetInstance()->Poke(dms::Variable(0xF6A2), 5);
				k = 5;
			}
			else
			{
				dms::Variable flagF6A2 = Executer::GetInstance()->Peek(0xF6A2);
				if(flagF6A2 == 1)
				{
					k = 10;
				}
				else
				{
					Executer::GetInstance()->Poke(dms::Variable(0xF6A2), 6);
					k = 6;
				}
			}
		}
		if(k == -1)
		{
			dms::Variable flag6 = Executer::GetInstance()->Peek(0xFFF6);
			dms::Variable flag8 = Executer::GetInstance()->Peek(0xFFF8);
			l = Actor(input);
			if((l == 4) && (flag8 == 4))
			{
				// �L�~�K�L
				k = 11;
				l = 11;
			}
			else if(l == 6)
			{
				// �~�g
				if(flag8 > 2)
				{
					l = 12;
				}
				else if(flag6 != 1)
				{
					l = 12;
				}
				k = 8;
			}
			else
			{
				k = 8;
			}
		}
	}
	else if((input.find("ϻ", 0) != -1) || (input.find("����", 0) != -1))
	{
		k = 9;
	}
}

// 2340 �p�\�R��
void Scenario2340(dms::String& input, int type, int& k, int& l)
{
	if(input.find("��", 0) != -1)
	{
		if(input.find("����", 0) != -1)
		{
			k = 1;
		}
		else if(input.find("з�", 0) != -1)
		{
			k = 2;
		}
		else if(input.find("���", 0) != -1)
		{
			k = 8;
		}
	}
	else if(input.find("��", 0) != -1)
	{
		if(input.find("��", 0) != -1)
		{
			k = 3;
		}
	}
	else if(input.find("LIST", 0) != -1)
	{
		k = 4;
	}
	else if(input.find("DIR", 0) != -1)
	{
		k = 5;
	}
	else if(input.find("MSG", 0) != -1)
	{
		k = 6;
	}
	else if(input.find("RUN", 0) != -1)
	{
		k = 7;
	}
}

// 2510 �R�E
void Scenario2510(dms::String& input, int type, int& k, int& l)
{
	if(input.find("��", 0) != -1)
	{
		if(input.find("�ݺ", 0) != -1)
		{
			dms::Variable keyFlag = Executer::GetInstance()->Peek(0xFFEF);
			if(keyFlag == 2)
			{
				k = 2;
			}
			else
			{
				k = 1;
			}
		}
	}
	else if(input.find("��", 0) != -1)
	{
		if(input.find("з�", 0) != -1)
		{
			k = 3;
		}
		else if(input.find("����", 0) != -1)
		{
			k = 4;
		}
		else if(input.find("���", 0) != -1)
		{
			k = 8;
		}
	}
	else if((input.find("���", 0) != -1) || (input.find("��", 0) != -1))
	{
		dms::Variable flag10 = Executer::GetInstance()->Peek(0xFFFA);
		if((input.find("�ݶ", 0) != -1) && (flag10 != 0))
		{
			k = 6;
			Executer::GetInstance()->Poke(dms::Variable(0xFFFB), 1);
		}
		else
		{
			k = 7;
			l = Actor(input);
		}
	}
	else if((input.find("��", 0) != -1) || (input.find("����", 0) != -1))
	{
		k = 5;
	}
}

// 2715 ��
void Scenario2715(dms::String& input, int type, int& k, int& l)
{
	dms::Variable holeFlag = Executer::GetInstance()->Peek(0xFFF0); // �h�A���J���Ă��邩
	if(input.find("��", 0) != -1)
	{
		if(input.find("з�", 0) != -1)
		{
			k = 2;
		}
		else if(input.find("���", 0) != -1)
		{
			k = 3;
		}
		else if(input.find("����", 0) != -1)
		{
			k = 4;
		}
		else if(input.find("ϴ", 0) != -1)
		{
			if(holeFlag == 1)
			{
				k = 5;
			}
		}
	}
	else if((input.find("���", 0) != -1) || (input.find("��", 0) != -1))
	{
		Executer::GetInstance()->Poke(dms::Variable(0xFFF0), 1);
		k = 1;
	}
}

// 2810 ���̂Đl�̃`�o
void Scenario2810(dms::String& input, int type, int& k, int& l)
{
	if(input.find("��", 0) != -1)
	{
		if(input.find("���", 0) != -1)
		{
			k = 1;
		}
	}
	else if((input.find("��", 0) != -1) || (input.find("���", 0) != -1))
	{
		dms::Variable flag = Executer::GetInstance()->Peek(0xFFEE); // �i�C�t����V�������������?
		if(input.find("���޲�", 0) != -1)
		{
			// �`�o�Ƀn���o�C�L�����Ă���ƕ���
			Executer::GetInstance()->Poke(dms::Variable(0xFFF7), 1);
			k = 3;
		}
		else if((input.find("Ų�", 0) != -1) && (flag != 0))
		{
			// �i�C�t�ɂ��ĕ���
			k = 4;
		}
		else
		{
			// �l���ɂ��ĕ���
			k = 5;
			l = Actor(input);
		}
	}
	else if((input.find("���", 0) != -1) || (input.find("����", 0) != -1))
	{
		k = 2;
	}
}

// 3150 �L�~�K�L
void Scenario3150(dms::String& input, int type, int& k, int& l)
{
	dms::Variable doorFlag = Executer::GetInstance()->Peek(0xFFF0); // �h�A���J���Ă��邩
	if(input.find("��", 0) != -1)
	{
		if(input.find("���", 0) != -1)
		{
			k = 1;
		}
		else if(input.find("����", 0) != -1)
		{
			k = 2;
		}
	}
	else if((input.find("���", 0) != -1) || (input.find("��", 0) != -1))
	{
		l = Actor(input);
		// �~�g�ɂ��ĕ������ꍇ�̓t���O���삷��
		if(l == 6)
		{
			dms::Variable flag = Executer::GetInstance()->Peek(0xFFF8);
			if(flag == 4)
			{
				// �t���O�������Ă����琳���ɘb��
				Executer::GetInstance()->Poke(dms::Variable(0xFFF9), 1);
			}
			else
			{
				// �t���O�������Ă��Ȃ������炵��΂������
				l = 1;
			}
		}
		k = 4;
	}
	else if((input.find("�ж޷", 0) != -1) || (input.find("����", 0) != -1))
	{
		k = 3;
	}
}

// 3705 �}�C�R���V���b�v�O
void Scenario3705(dms::String& input, int type, int& k, int& l)
{
	dms::Variable doorFlag = Executer::GetInstance()->Peek(0xFFF0); // �h�A���J���Ă��邩
	if(input.find("��", 0) != -1)
	{
		if(input.find("�ޱ", 0) != -1)
		{
			if(doorFlag == 0)
			{
				k = 4;
				Executer::GetInstance()->Poke(dms::Variable(0xFFF0), 1);
			}
			else
			{
				k = 5;
			}
		}
	}
	else if(input.find("��", 0) != -1)
	{
		if(input.find("з�", 0) != -1)
		{
			k = 1;
		}
		else if(input.find("����", 0) != -1)
		{
			k = 2;
		}
		else if(input.find("ϴ", 0) != -1)
		{
			if(doorFlag == 1)
			{
				k = 3;
			}
		}
	}
}

// 3810 �J���_
void Scenario3810(dms::String& input, int type, int& k, int& l)
{
	if((input.find("���", 0) != -1) || (input.find("��", 0) != -1))
	{
		dms::Variable flag9 = Executer::GetInstance()->Peek(0xFFF9);
		dms::Variable flag11 = Executer::GetInstance()->Peek(0xFFFB);
		if((input.find("���", 0) != -1) && (flag11 == 1))
		{
			k = 1;
			Executer::GetInstance()->Poke(dms::Variable(0xFFEF), 1);
		}
		else if((input.find("�ݶ", 0) != -1) && (flag9 == 1))
		{
			k = 2;
			Executer::GetInstance()->Poke(dms::Variable(0xFFFA), 1);
		}
		else
		{
			// �l���ɂ��ĕ���
			k = 3;
			l = Actor(input);
		}
	}
	else if((input.find("����", 0) != -1) || (input.find("����", 0) != -1))
	{
		k = 4;
	}
	else if((input.find("��", 0) != -1) || (input.find("���", 0) != -1))
	{
		k = 5;
	}
	else if(input.find("ӳ��ϯ�", 0) != -1)
	{
		k = 8;
	}
	else if(input.find("��", 0) != -1)
	{
		dms::Variable flag11 = Executer::GetInstance()->Peek(0xFFFB);
		if(flag11 != 0)
		{
			if(input.find("���", 0) != -1)
			{
				dms::Variable flagFFEF = Executer::GetInstance()->Peek(0xFFEF);
				if(flagFFEF == 2)
				{
					k = 0;
				}
				else if(flagFFEF == 1)
				{
					Executer::GetInstance()->Poke(dms::Variable(0xFFEF), 2);
					k = 7;
				}
				else
				{
					k = 6;
				}
			}
		}
	}
}

// 4160 �i�C�t
void Scenario4160(dms::String& input, int type, int& k, int& l)
{
	if(input.find("��", 0) != -1)
	{
		if(input.find("����", 0) != -1)
		{
			k = 4;
		}
		else if(input.find("з�", 0) != -1)
		{
			k = 5;
		}
		else if(input.find("���", 0) != -1)
		{
			k = 6;
		}
	}
	else if(input.find("����", 0) != -1)
	{
		if(input.find("���", 0) != -1)
		{
			Executer::GetInstance()->Poke(dms::Variable(0xFFEE), 1);
			k = 1;
		}
		else if(input.find("Ų�", 0) != -1)
		{
			k = 2;
		}
	}
	else if((input.find("��", 0) != -1) && (input.find("��", 0) != -1))
	{
		k = 3;
	}
}

// 4410 �Ɛl
void Scenario4410(dms::String& input, int type, int& k, int& l)
{
	if(input.find("����", 0) != -1)
	{
		k = 1;
	}
	else if(input.find("�����", 0) != -1)
	{
		k = 2;
	}
	else if(input.find("ϻ", 0) != -1)
	{
		k = 3;
	}
	else if(input.find("���", 0) != -1)
	{
		k = 4;
	}
	else if(input.find("�ж޷", 0) != -1)
	{
		k = 5;
	}
	else if(input.find("��", 0) != -1)
	{
		k = 6;
	}
	else if(input.find("��", 0) != -1)
	{
		k = 7;
	}
	else if(input.find("���", 0) != -1)
	{
		k = 8;
	}
	else if(input.find("����", 0) != -1)
	{
		k = 9;
	}
	else if(input.find("�ϴ", 0) != -1)
	{
		k = 10;
	}
	else if(input.find("���", 0) != -1)
	{
		k = 11;
	}
}

// �V�i���I����
void Scenario(dms::String* parameter)
{
	if((*parameter).size() <= 5)
	{
		return;
	}
	int a = (*parameter)[0];
	unsigned char parameter0 = static_cast<unsigned char>((*parameter)[0]);
	unsigned char parameter1 = static_cast<unsigned char>((*parameter)[1]);
	unsigned char parameter3 = static_cast<unsigned char>((*parameter)[3]);
	unsigned char parameter4 = static_cast<unsigned char>((*parameter)[4]);
	int type = static_cast<int>(parameter0) * 256 * 256 * 256 + parameter1 * 256 * 256 + parameter3 * 256 + parameter4;
	dms::String input = (*parameter).substr(5);
	int k = 0;
	int l = 0;
	switch(type)
	{
	case 0xFD1AFDC0:
		Scenario205(input,type, k, l);
		break;
	case 0xFFDCFDD1:
		Scenario320(input,type, k, l);
		break;
	case 0xFF7AFCCB:
		Scenario1150(input,type, k, l);
		break;
	case 0xF670FC69:
		Scenario1420(input,type, k, l);
		break;
	case 0xFFB9FC29:
		Scenario1840(input,type, k, l);
		break;
	case 0xFBAEFBD8:
		Scenario1965(input,type, k, l);
		break;
	case 0xFAB2FAFB:
		Scenario2050(input,type, k, l);
		break;
	case 0xFA4AFA70:
		Scenario2340(input,type, k, l);
		break;
	case 0xF996F9BF:
		Scenario2510(input,type, k, l);
		break;
	case 0xF8C6F8E7:
		Scenario2810(input,type, k, l);
		break;
	case 0xF93CF955:
		Scenario2715(input,type, k, l);
		break;
	case 0xF855F87A:
		Scenario3150(input,type, k, l);
		break;
	case 0xF805F81A:
		Scenario3705(input,type, k, l);
		break;
	case 0xF74EF786:
		Scenario3810(input,type, k, l);
		break;
	case 0xF6EAF70C:
		Scenario4160(input,type, k, l);
		break;
	case 0xF6A3F6D1:
		Scenario4410(input,type, k, l);
		break;
	}
	Executer::GetInstance()->Poke(dms::Variable(0xFFFD), k);
	Executer::GetInstance()->Poke(dms::Variable(0xFFFC), l);
}

// ��`�\��
// ����640x200������ʃ��[�h��320x200�Ȃ̂ł�����ɍœK��
void DrawRect(dms::String* parameter)
{
	dms::Variable x = Executer::GetInstance()->Peek(0xFF64); // X(8�h�b�g�P��)
	dms::Variable y = Executer::GetInstance()->Peek(0xFF68); // Y(1�h�b�g�P��)
	dms::Variable h = Executer::GetInstance()->Peek(0xFF1A); // ����(1�h�b�g�P��)
	dms::Variable w = Executer::GetInstance()->Peek(0xFF1D); // ��(8�h�b�g�P��)
	dms::Variable b = Executer::GetInstance()->Peek(0xFF26); // ��(8�h�b�g��)
	dms::Variable r = Executer::GetInstance()->Peek(0xFF2D); // ��(8�h�b�g��)
	dms::Variable g = Executer::GetInstance()->Peek(0xFF34); // ��(8�h�b�g��)
	Screen& screen = Executer::GetInstance()->GetScreen();
	int width = screen.GetGraphicWidth() / 8;
	static unsigned int maskTable[3] = { 0xFF0000FF, 0xFFFF0000, 0xFF00FF00 };
	for(int yy = 0; yy < h.GetInt(); ++ yy)
	{
		for(int xx = 0; xx < w.GetInt(); ++ xx)
		{
			int drawX = x.GetInt() * 4 + xx * 4;
			int drawY = y.GetInt() + yy;
			for(int i = 0; i < 3; ++ i)
			{
				unsigned char drawPattern = 0;
				switch(i)
				{
				case 0:
					drawPattern = b.GetInt();
					break;
				case 1:
					drawPattern = r.GetInt();
					break;
				case 2:
					drawPattern = g.GetInt();
					break;
				}
				for(int j = 0; j < 4; ++ j)
				{
					unsigned int dot = (drawPattern >> j) & 1;
					screen.DrawPoint(drawX + j, drawY, 0, maskTable[i]);
					if(dot > 0)
					{
						screen.DrawPoint(drawX + j, drawY, 0xFFFFFFFF, maskTable[i]);
					}
				}
			}
		}
	}
}

// �v���O�����p�b�`
void l11410_06_patch() { Paint(36_n,76_n,{0_n},4_n,0_n); }
void l11430_01_patch() {}

void JikenPatch(void)
{
	// D$ = �N���X�^���\�t�g���S	5*16�o�C�g+5*16�o�C�g
	// C$ = �R�s�[���C�g			104�o�C�g
	// B$ = �\�t�g�n�E�X			72�o�C�g
	// A$ = �E�l����				160�o�C�g
	static unsigned char memoryF480[] =
	{
		0xC0, 0x61, 0x61, 0x33, 0x33, 0x1E, 0x1E, 0x00, 0x0C, 0x00, 0x1E, 0x33, 0x33, 0x61, 0x61, 0xC0,
		0xFF, 0xFF, 0x8C, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x0C, 0x0C, 0x8C, 0x8C, 0xCC,
		0xE3, 0xE3, 0x07, 0x07, 0x0C, 0x0C, 0x0C, 0x00, 0x18, 0x00, 0x3F, 0x3F, 0x60, 0x60, 0xC0, 0xC0,
		0x01, 0x01, 0x81, 0x81, 0xC1, 0xC1, 0xC1, 0x00, 0x61, 0x00, 0xF1, 0xF1, 0x19, 0x19, 0x0D, 0x0D,
		0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF,
		0x1F, 0x20, 0x40, 0x40, 0x40, 0x40, 0x20, 0x1F, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x20, 0x1F,
		0x83, 0x44, 0x28, 0x28, 0x28, 0x08, 0x08, 0x08, 0x88, 0x48, 0x28, 0x28, 0x28, 0x28, 0x44, 0x83,
		0xF1, 0x09, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x09, 0xF1,
		0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xFF, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x18, 0x3C, 0x7C, 0xED, 0xC1, 0xED, 0xFD, 0x78, 0x30, 0x78, 0xF9, 0xDB, 0x8B, 0xDB, 0xFB, 0xF3,
		0x60, 0xF1, 0x93, 0x36, 0xE7, 0xC1, 0x1F, 0x8F, 0xC3, 0x87, 0x2C, 0x6C, 0xEF, 0xEE, 0xCC, 0x8E,
		0x81, 0xC3, 0xC3, 0x83, 0x07, 0x87, 0xCF, 0x6F, 0x0C, 0x1E, 0x36, 0x60, 0x6C, 0x62, 0x7E, 0x3C,
		0x24, 0x45, 0xCC, 0xCC, 0xFC, 0xCC, 0xDC, 0xDC, 0xFC, 0xF8, 0x60, 0x61, 0x61, 0x61, 0xE1, 0xE0,
		0x23, 0x47, 0x8F, 0x1D, 0x18, 0x1D, 0x1F, 0x8F, 0x10, 0x88, 0x84, 0x84, 0x04, 0x84, 0x88, 0x11,
		0x21, 0x23, 0x66, 0x6C, 0x6F, 0xE7, 0xE3, 0xC7, 0x86, 0xCF, 0xD9, 0xCB, 0x9E, 0xB3, 0xB3, 0x1E,
		0x08, 0x18, 0x30, 0x64, 0x6D, 0x7F, 0x3E, 0x0C, 0xC0, 0xC0, 0x60, 0x30, 0x01, 0x02, 0x04, 0x00,
		0x67, 0x60, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00, 0xFE, 0x06, 0x06, 0x0C, 0x18, 0x30, 0xC0, 0x00,
		0x18, 0x18, 0x18, 0x1F, 0x19, 0x18, 0x18, 0x00, 0x00, 0x00, 0x01, 0x02, 0x86, 0x06, 0x06, 0x00,
		0x20, 0x10, 0x08, 0x04, 0x06, 0x06, 0x06, 0x00, 0x06, 0x7F, 0x60, 0x60, 0x00, 0x01, 0x0E, 0x00,
		0x07, 0xE0, 0xE0, 0xC0, 0xC0, 0x81, 0x06, 0x00, 0xFC, 0x0C, 0x18, 0x30, 0xC0, 0xB0, 0x0E, 0x00,
		0x01, 0x1B, 0x06, 0x1F, 0x71, 0x0C, 0x0C, 0x0C, 0x7F, 0x0C, 0x1E, 0x3F, 0x2D, 0x4C, 0x8C, 0x0C,
		0x9F, 0x19, 0x19, 0x19, 0xB1, 0x21, 0x40, 0x00, 0x7F, 0x00, 0x21, 0x21, 0x1B, 0x06, 0x1B, 0x60,
		0x00, 0x80, 0x80, 0x80, 0xA0, 0xA0, 0xE0, 0x00, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x81, 0xE6,
		0x07, 0x06, 0x06, 0x06, 0x0F, 0x0F, 0x0F, 0x19, 0x19, 0x30, 0x30, 0x60, 0x60, 0xC0, 0x80, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0x60, 0x60, 0x30, 0x18, 0x06,
		0x00, 0x00, 0x3F, 0x00, 0x0F, 0x0C, 0x0F, 0x00, 0x1F, 0x00, 0x7F, 0x00, 0x1F, 0x00, 0x01, 0x00,
		0x60, 0x60, 0xFF, 0x60, 0xFF, 0x63, 0xFF, 0x60, 0xFF, 0x61, 0xFF, 0x61, 0xFF, 0x60, 0x60, 0xE0,
		0x00, 0x00, 0xC0, 0x00, 0x01, 0x01, 0x03, 0x04, 0x80, 0x80, 0xE0, 0x80, 0x80, 0x00, 0x00, 0x00,
		0x70, 0x66, 0xE6, 0xC7, 0x8C, 0xC8, 0xD0, 0xC0, 0xC0, 0xDF, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
		0xC0, 0xC0, 0xC0, 0xFC, 0xC0, 0xC0, 0xC0, 0xC0, 0xC6, 0xFE, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0
	};
	for(int i = 0; i < sizeof(memoryF480); ++ i)
	{
		Executer::GetInstance()->Poke(dms::Variable(0xF480) + dms::Variable(i), memoryF480[i]);
	}
	Executer::GetInstance()->UsrPatch(0xF3B0, DrawArrow);
	Executer::GetInstance()->UsrPatch(0xFD00, Scenario);
	Executer::GetInstance()->UsrPatch(0xFEF0, DrawRect);
	Executer::GetInstance()->ProgramPatch(L11410_06, l11410_06_patch);
	Executer::GetInstance()->ProgramPatch(L11430_01, l11430_01_patch);
}
