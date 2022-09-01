// ソフトハウス殺人事件用のパッチ
// Basic.cppのMainLoop_Setup()でJikenPatch()を実行してください
#include "Executer.hpp"

// 元は640x200だが画面モードが320x200なのでそちらに最適化
void DrawRect2(dms::String* parameter)
{
	dms::Variable x = Executer::GetInstance()->Peek(0xFF64); // X(8ドット単位)
	dms::Variable y = Executer::GetInstance()->Peek(0xFF68); // Y(1ドット単位)
	dms::Variable h = Executer::GetInstance()->Peek(0xFF1A); // 高さ(1ドット単位)
	dms::Variable w = Executer::GetInstance()->Peek(0xFF1D); // 幅(8ドット単位)
	dms::Variable b = Executer::GetInstance()->Peek(0xFF26); // 青(8ドット分)
	dms::Variable r = Executer::GetInstance()->Peek(0xFF2D); // 赤(8ドット分)
	dms::Variable g = Executer::GetInstance()->Peek(0xFF34); // 緑(8ドット分)
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
