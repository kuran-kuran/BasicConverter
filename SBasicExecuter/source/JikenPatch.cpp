// �\�t�g�n�E�X�E�l�����p�̃p�b�`
// Basic.cpp��MainLoop_Setup()��JikenPatch()�����s���Ă�������
#include "Executer.hpp"

// ����640x200������ʃ��[�h��320x200�Ȃ̂ł�����ɍœK��
void DrawRect2(dms::String* parameter)
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
					drawPattern = b.GetInt() & 0xF0;
					break;
				case 1:
					drawPattern = r.GetInt() & 0xF0;
					break;
				case 2:
					drawPattern = g.GetInt() & 0xF0;
					break;
				}
				screen.DrawPattern(drawX, drawY, drawPattern, 0xFFFFFFFF, maskTable[i]);
			}
		}
	}
}

void JikenPatch(void)
{
	Executer::GetInstance()->UsrPatch(0xFEF0, DrawRect2);
}
