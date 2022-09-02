// ソフトハウス殺人事件用のパッチ
// Basic.cppのMainLoop_Setup()でJikenPatch()を実行してください
#include "Command.hpp"
#include "Executer.hpp"
#include "Basic.hpp"

// 0xFFF0 ドアが開いているか
// 0xFFFE カーソルX座標保存
// 0xFFFF カーソルY座標保存

// 矢印表示
void DrawArrow(dms::String* parameter)
{
	int dir = (*parameter)[0]; // 矢印の方向 (0:↑, 8:↓, 16:→, 24:←
	int color = (*parameter)[1]; // 矢印の色 (2:赤, 6:黄色)
	int y = (*parameter)[2]; // Y座標
	int x = (*parameter)[3]; // X座標
	int data5 = (*parameter)[4];
	unsigned char data = 0x80 >> data5;
	unsigned char arrowPattern[] = {32,112,168,32,32,32,32,32, 32,32,32,32,32,168,112,32, 0,0,32,16,248,16,32,0, 0,0,32,64,248,64,32,0};
	Screen& screen = Executer::GetInstance()->GetScreen();
	static unsigned int colorTable[8] = { 0xFF000000, 0xFF0000FF, 0xFFFF0000, 0xFFFF00FF, 0xFF00FF00, 0xFF00FFFF, 0xFFFFFF00, 0xFFFFFFFF };
	for(int i = 0; i < 8; ++ i)
	{
		int drawX = x * 4;
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

void ScenarioFD1A(dms::String& input, int type, int& k, int& l)
{
	if(input.find("ｱﾎ", 0) != -1)
	{
		k = 1;
	}
	if(input.find("ﾊﾞｶ", 0) != -1)
	{
		k = 1;
	}
}

void ScenarioFFDC(dms::String& input, int type, int& k, int& l)
{
	if(input.find("ｲｸ", 0) != -1)
	{
		if(input.find("ﾐｷﾞ", 0) != -1)
		{
			k = 1;
		}
		else if(input.find("ﾋﾀﾞﾘ", 0) != -1)
		{
			k = 2;
		}
		else if(input.find("ﾏｴ", 0) != -1)
		{
			k = 3;
		}
	}
}

void ScenarioFF7A(dms::String& input, int type, int& k, int& l)
{
	dms::Variable doorFlag = Executer::GetInstance()->Peek(0xFFF0); // ドアが開いているか
	if(input.find("ｱｹ", 0) != -1)
	{
		if(input.find("ﾄﾞｱ", 0) != -1)
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
	if((doorFlag == 1) && (input.find("ｲｸ", 0) != -1))
	{
		if(input.find("ﾋﾀﾞﾘ", 0) != -1)
		{
			k = 3;
		}
	}
}

void Scenario(dms::String* parameter)
{
	if((*parameter).size() <= 5)
	{
		return;
	}
	int type = static_cast<unsigned char>((*parameter)[0]) * 256 + (*parameter)[1];
	dms::String input = (*parameter).substr(5);
	int k = 0;
	int l = 0;
	switch(type)
	{
	case 0xFD1A:
		ScenarioFD1A(input,type, k, l);
		break;
	case 0xFFDC:
		ScenarioFFDC(input,type, k, l);
		break;
	case 0xFF7A:
		ScenarioFF7A(input,type, k, l);
		break;
	}
	Executer::GetInstance()->Poke(dms::Variable(0xFFFD), k);
	Executer::GetInstance()->Poke(dms::Variable(0xFFFC), l);
}

// 元は640x200だが画面モードが320x200なのでそちらに最適化
void DrawRect(dms::String* parameter)
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

// プログラムパッチ
void l11410_06_patch() { Paint(36_n,76_n,{0_n},4_n,0_n); }
void l11430_01_patch() {}

void JikenPatch(void)
{
	// D$ = クリスタルソフトロゴ	5*16バイト+5*16バイト
	// C$ = コピーライト			104バイト
	// B$ = ソフトハウス			72バイト
	// A$ = 殺人事件				160バイト
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
