// ソフトハウス殺人事件用のパッチ
// Basic.cppのMainLoop_Setup()でJikenPatch()を実行してください
#include "Command.hpp"
#include "Executer.hpp"
#include "Basic.hpp"

#ifndef L11410_06
#define L11410_06 0
#endif
#ifndef L11430_01
#define L11430_01 0
#endif

// 0xF6A2 0: 5: マサに自販機が壊れていると聞いた, 6: さっき言ったとおりです
// 0xFFEE 0: 1: ナイフからチバのシモンを取った
// 0xFFEF 0: 1: カワダからカギをキク, 2: カワダからカギを取る
// 0xFFF0 0: 閉じている 1: ドアまたはカベが開いている
// 0xFFF1 0: 社長と1度も喋っていない, 1:一度喋った, 2:エレベータを教えてもらった
// 0xFFF2 0: リンゴについて知らない, 1: マサにリンゴについて聞いた
// 0xFFF3 1: 1階, 2: 2階
// 0xFFF4 0: フロッピーディスクを持っていない, 1:フロッピーディスクを持っている
// 0xFFF5 0: パソコンを見てない 1: パソコンを見た
// 0xFFF6 0: パソコンを見てない 1: パソコンを見た
// 0xFFF7 0: チバにハンバイキが壊れていると聞いていない, 1: チバにハンバイキが壊れていると聞く
// 0xFFF8 0: 初期値 1:マサにミトについて聞いた, 2: マサに自販機が壊れていると聞いた, 3: マサに横領に気が付いたミトについて聞く, 4: マサに横領について聞く
// 0xFFF9 0: キミガキがミトについて隠している 1: キミガキがミトについて正直に話す
// 0xFFFA 0: 1: カワダにケンカについて聞く
// 0xFFFB 0: 1: コウにケンカについて聞く
// 0xFFFC L: 聞いた人物など ｼｬﾁｮｳ,ﾏｻ,ﾁﾊﾞ,ｷﾐｶﾞｷ,ｺｳ,ﾐﾄ,ｶﾜﾀﾞ,ﾅﾏｴ,ｷﾉｳ,ｼﾞｹﾝ
// 0xFFFD K: シナリオの結果
// 0xFFFE カーソルX座標保存
// 0xFFFF カーソルY座標保存

//社長に話を聞く（一応）。
//現場調査。ディスクを拾う。
//パソコンを使う。
//チバに販売機について聞く。
//マサを問い詰める。
//キミガキを問い詰める。
//カワダにケンカについて聞く。
//カワダにカギを聞き、取る。
//金庫を開ける。
//ナイフを調べる。
//チバにナイフの事を聞く。
//犯人を推理。

// 矢印表示
void DrawArrow(dms::String* parameter)
{
	int dir = (*parameter)[0]; // 矢印の方向 (0:↑, 8:↓, 16:→, 24:←
	int color = (*parameter)[1]; // 矢印の色 (2:赤, 6:黄色)
	int y = (*parameter)[2]; // Y座標
	int x = (*parameter)[3]; // X座標
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

// 人物判定
int Actor(dms::String& input)
{
	std::vector<dms::String> actors =  {"","ｼｬﾁｮｳ","ﾏｻ","ﾁﾊﾞ","ｷﾐｶﾞｷ","ｺｳ","ﾐﾄ","ｶﾜﾀﾞ","ﾅﾏｴ","ｷﾉｳ","ｼﾞｹﾝ"};
	for(int i = 1; i < static_cast<int>(actors.size()); ++ i)
	{
		if(input.find(&actors[i][0], 0) != dms::String::npos)
		{
			// 見つけた
			return i;
		}
	}
	return 0;
}

// 205 アホバカ判別
void Scenario205(dms::String& input, int type, int& k, int& l)
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

// 320 行けない方向判別
void Scenario320(dms::String& input, int type, int& k, int& l)
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
		else if(input.find("ｳｼﾛ", 0) != -1)
		{
			k = 4;
		}
	}
}

// 1150 初期位置
void Scenario1150(dms::String& input, int type, int& k, int& l)
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
	if(input.find("ｲｸ", 0) != -1)
	{
		if((doorFlag == 1) && (input.find("ﾋﾀﾞﾘ", 0) != -1))
		{
			k = 3;
		}
		if(input.find("ｳｼﾛ", 0) != -1)
		{
			k = 4;
		}
		if(input.find("ﾏｴ", 0) != -1)
		{
			k = 5;
		}
	}
}

// 1420 社長
void Scenario1420(dms::String& input, int type, int& k, int& l)
{
	dms::Variable elevatorFlag = Executer::GetInstance()->Peek(0xFFF1); // 0:社長と1度も喋っていない, 1:一度喋った, 2:エレベータを教えてもらった
	dms::Variable appleFlag = Executer::GetInstance()->Peek(0xFFF2); // 0: マサにリンゴについて聞いてない 1: 聞いた
	if(input.find("ｹﾞﾝﾊﾞ", 0) != -1)
	{
		k = 1;
	}
	else if(input.find("ｼﾞｹﾝ", 0) != -1)
	{
		k = 2;
	}
	else if((input.find("ｷｸ", 0) != -1) || (input.find("ｼｯﾃ", 0) != -1))
	{
		if((appleFlag == 1) && (input.find("ﾘﾝｺﾞ", 0) != -1))
		{
			k = 3;
		}
		else if(input.find("ｸﾘｽﾀﾙ", 0) != -1)
		{
			k = 4;
		}
		else
		{
			k = 5;
			l = Actor(input);
		}
	}
	else if(input.find("ｲｸ", 0) != -1)
	{
		if(input.find("ﾐｷﾞ", 0) != -1)
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
		else if(input.find("ｳｼﾛ", 0) != -1)
		{
			k = 8;
		}
	}
	else if((input.find("ｼｬﾁｮｳ", 0) != -1) || (input.find("ｼﾗﾍﾞ", 0) != -1))
	{
		k = 9;
	}
}

// 1840 エレベーター前
void Scenario1840(dms::String& input, int type, int& k, int& l)
{
	dms::Variable floor = Executer::GetInstance()->Peek(0xFFF3); // 何階にいるか
	if(input.find("ｲｸ", 0) != -1)
	{
		if(floor == 1)
		{
			// 1階にいる
			if(input.find("ﾐｷﾞ", 0) != -1)
			{
				k = 2;
			}
			else if(input.find("ﾋﾀﾞﾘ", 0) != -1)
			{
				k = 4;
			}
		}
		else
		{
			// 2階にいる
			if(input.find("ﾐｷﾞ", 0) != -1)
			{
				k = 3;
			}
			else if(input.find("ﾋﾀﾞﾘ", 0) != -1)
			{
				k = 5;
			}
			else if(input.find("ｳｼﾛ", 0) != -1)
			{
				k = 6;
			}
		}
	}
	else if(((input.find("ﾎﾞﾀﾝ", 0) != -1) || (input.find("ｽｲｯﾁ", 0) != -1)) && (input.find("ｵｽ", 0) != -1))
	{
		k = 1;
	}
}

// 1965 現場
void Scenario1965(dms::String& input, int type, int& k, int& l)
{
	if((input.find("ｼｯﾃ", 0) != -1) || (input.find("ｷｸ", 0) != -1))
	{
		k = 1;
	}
	else if(input.find("ｲｸ", 0) != -1)
	{
		if(input.find("ﾐｷﾞ", 0) != -1)
		{
			k = 2;
		}
		else if(input.find("ﾋﾀﾞﾘ", 0) != -1)
		{
			k = 3;
		}
		else if(input.find("ｳｼﾛ", 0) != -1)
		{
			k = 4;
		}
	}
	else if(input.find("ﾄﾙ", 0) != -1)
	{
		if(input.find("ﾃﾞｨｽｸ", 0) != -1)
		{
			dms::Variable diskFlag = Executer::GetInstance()->Peek(0xFFF4); // フロッピーディスクを持っているか
			if(diskFlag == 0)
			{
				k = 5;
			}
		}
	}
	else if(input.find("ｼﾗﾍﾞ", 0) != -1)
	{
		if(input.find("ｼｲﾝ", 0) != -1)
		{
			k = 6;
		}
		else if(input.find("ﾐﾄ", 0) != -1)
		{
			k = 7;
		}
	}
}

// 2050 マサ
void Scenario2050(dms::String& input, int type, int& k, int& l)
{
	if(input.find("ｲｸ", 0) != -1)
	{
		if(input.find("ﾋﾀﾞﾘ", 0) != -1)
		{
			k = 1;
		}
		else if(input.find("ﾐｷﾞ", 0) != -1)
		{
			k = 2;
		}
		else if(input.find("ｳｼﾛ", 0) != -1)
		{
			k = 11;
		}
	}
	else if(input.find("ﾂｶ", 0) != -1)
	{
		if((input.find("ﾏｲｺﾝ", 0) != -1) || (input.find("ｺﾝﾋﾟｭｰﾀ", 0) != -1) || (input.find("ﾊﾟｿｺﾝ", 0) != -1))
		{
			dms::Variable diskFlag = Executer::GetInstance()->Peek(0xFFF4); // フロッピーディスクを持っているか
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
	else if((input.find("ｼｯﾃ", 0) != -1) || (input.find("ｷｸ", 0) != -1))
	{
		k = -1;
		if(input.find("ｵｳﾘｮｳ", 0) != -1)
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
		else if(input.find("ﾊﾝﾊﾞｲｷ", 0) != -1)
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
				// キミガキ
				k = 11;
				l = 11;
			}
			else if(l == 6)
			{
				// ミト
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
	else if((input.find("ﾏｻ", 0) != -1) || (input.find("ｼﾗﾍﾞ", 0) != -1))
	{
		k = 9;
	}
}

// 2340 パソコン
void Scenario2340(dms::String& input, int type, int& k, int& l)
{
	if(input.find("ｲｸ", 0) != -1)
	{
		if(input.find("ﾋﾀﾞﾘ", 0) != -1)
		{
			k = 1;
		}
		else if(input.find("ﾐｷﾞ", 0) != -1)
		{
			k = 2;
		}
		else if(input.find("ｳｼﾛ", 0) != -1)
		{
			k = 8;
		}
	}
	else if(input.find("ﾍﾔ", 0) != -1)
	{
		if(input.find("ﾐﾙ", 0) != -1)
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

// 2510 コウ
void Scenario2510(dms::String& input, int type, int& k, int& l)
{
	if(input.find("ｱｹ", 0) != -1)
	{
		if(input.find("ｷﾝｺ", 0) != -1)
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
	else if(input.find("ｲｸ", 0) != -1)
	{
		if(input.find("ﾐｷﾞ", 0) != -1)
		{
			k = 3;
		}
		else if(input.find("ﾋﾀﾞﾘ", 0) != -1)
		{
			k = 4;
		}
		else if(input.find("ｳｼﾛ", 0) != -1)
		{
			k = 8;
		}
	}
	else if((input.find("ｼｯﾃ", 0) != -1) || (input.find("ｷｸ", 0) != -1))
	{
		dms::Variable flag10 = Executer::GetInstance()->Peek(0xFFFA);
		if((input.find("ｹﾝｶ", 0) != -1) && (flag10 != 0))
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
	else if((input.find("ｺｳ", 0) != -1) || (input.find("ｼﾗﾍﾞ", 0) != -1))
	{
		k = 5;
	}
}

// 2715 壁
void Scenario2715(dms::String& input, int type, int& k, int& l)
{
	dms::Variable holeFlag = Executer::GetInstance()->Peek(0xFFF0); // ドアが開いているか
	if(input.find("ｲｸ", 0) != -1)
	{
		if(input.find("ﾐｷﾞ", 0) != -1)
		{
			k = 2;
		}
		else if(input.find("ｳｼﾛ", 0) != -1)
		{
			k = 3;
		}
		else if(input.find("ﾋﾀﾞﾘ", 0) != -1)
		{
			k = 4;
		}
		else if(input.find("ﾏｴ", 0) != -1)
		{
			if(holeFlag == 1)
			{
				k = 5;
			}
		}
	}
	else if((input.find("ｶﾍﾞ", 0) != -1) || (input.find("ｵｽ", 0) != -1))
	{
		Executer::GetInstance()->Poke(dms::Variable(0xFFF0), 1);
		k = 1;
	}
}

// 2810 世捨て人のチバ
void Scenario2810(dms::String& input, int type, int& k, int& l)
{
	if(input.find("ｲｸ", 0) != -1)
	{
		if(input.find("ｳｼﾛ", 0) != -1)
		{
			k = 1;
		}
	}
	else if((input.find("ｷｸ", 0) != -1) || (input.find("ｼｯﾃ", 0) != -1))
	{
		dms::Variable flag = Executer::GetInstance()->Peek(0xFFEE); // ナイフからシモンを取ったか?
		if(input.find("ﾊﾝﾊﾞｲｷ", 0) != -1)
		{
			// チバにハンバイキが壊れていると聞く
			Executer::GetInstance()->Poke(dms::Variable(0xFFF7), 1);
			k = 3;
		}
		else if((input.find("ﾅｲﾌ", 0) != -1) && (flag != 0))
		{
			// ナイフについて聞く
			k = 4;
		}
		else
		{
			// 人物について聞く
			k = 5;
			l = Actor(input);
		}
	}
	else if((input.find("ﾁﾊﾞ", 0) != -1) || (input.find("ｼﾗﾍﾞ", 0) != -1))
	{
		k = 2;
	}
}

// 3150 キミガキ
void Scenario3150(dms::String& input, int type, int& k, int& l)
{
	dms::Variable doorFlag = Executer::GetInstance()->Peek(0xFFF0); // ドアが開いているか
	if(input.find("ｲｸ", 0) != -1)
	{
		if(input.find("ｳｼﾛ", 0) != -1)
		{
			k = 1;
		}
		else if(input.find("ﾋﾀﾞﾘ", 0) != -1)
		{
			k = 2;
		}
	}
	else if((input.find("ｼｯﾃ", 0) != -1) || (input.find("ｷｸ", 0) != -1))
	{
		l = Actor(input);
		// ミトについて聞いた場合はフラグ操作する
		if(l == 6)
		{
			dms::Variable flag = Executer::GetInstance()->Peek(0xFFF8);
			if(flag == 4)
			{
				// フラグが立っていたら正直に話す
				Executer::GetInstance()->Poke(dms::Variable(0xFFF9), 1);
			}
			else
			{
				// フラグが立っていなかったらしらばっくれる
				l = 1;
			}
		}
		k = 4;
	}
	else if((input.find("ｷﾐｶﾞｷ", 0) != -1) || (input.find("ｼﾗﾍﾞ", 0) != -1))
	{
		k = 3;
	}
}

// 3705 マイコンショップ前
void Scenario3705(dms::String& input, int type, int& k, int& l)
{
	dms::Variable doorFlag = Executer::GetInstance()->Peek(0xFFF0); // ドアが開いているか
	if(input.find("ｱｹ", 0) != -1)
	{
		if(input.find("ﾄﾞｱ", 0) != -1)
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
	else if(input.find("ｲｸ", 0) != -1)
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
			if(doorFlag == 1)
			{
				k = 3;
			}
		}
	}
}

// 3810 カワダ
void Scenario3810(dms::String& input, int type, int& k, int& l)
{
	if((input.find("ｼｯﾃ", 0) != -1) || (input.find("ｷｸ", 0) != -1))
	{
		dms::Variable flag9 = Executer::GetInstance()->Peek(0xFFF9);
		dms::Variable flag11 = Executer::GetInstance()->Peek(0xFFFB);
		if((input.find("ｶｷﾞ", 0) != -1) && (flag11 == 1))
		{
			k = 1;
			Executer::GetInstance()->Poke(dms::Variable(0xFFEF), 1);
		}
		else if((input.find("ｹﾝｶ", 0) != -1) && (flag9 == 1))
		{
			k = 2;
			Executer::GetInstance()->Poke(dms::Variable(0xFFFA), 1);
		}
		else
		{
			// 人物について聞く
			k = 3;
			l = Actor(input);
		}
	}
	else if((input.find("ｼﾗﾍﾞ", 0) != -1) || (input.find("ｶﾜﾀﾞ", 0) != -1))
	{
		k = 4;
	}
	else if((input.find("ｲｸ", 0) != -1) || (input.find("ｳｼﾛ", 0) != -1))
	{
		k = 5;
	}
	else if(input.find("ﾓｳｶﾘﾏｯｶ", 0) != -1)
	{
		k = 8;
	}
	else if(input.find("ﾄﾙ", 0) != -1)
	{
		dms::Variable flag11 = Executer::GetInstance()->Peek(0xFFFB);
		if(flag11 != 0)
		{
			if(input.find("ｶｷﾞ", 0) != -1)
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

// 4160 ナイフ
void Scenario4160(dms::String& input, int type, int& k, int& l)
{
	if(input.find("ｲｸ", 0) != -1)
	{
		if(input.find("ﾋﾀﾞﾘ", 0) != -1)
		{
			k = 4;
		}
		else if(input.find("ﾐｷﾞ", 0) != -1)
		{
			k = 5;
		}
		else if(input.find("ｳｼﾛ", 0) != -1)
		{
			k = 6;
		}
	}
	else if(input.find("ｼﾗﾍﾞ", 0) != -1)
	{
		if(input.find("ｼﾓﾝ", 0) != -1)
		{
			Executer::GetInstance()->Poke(dms::Variable(0xFFEE), 1);
			k = 1;
		}
		else if(input.find("ﾅｲﾌ", 0) != -1)
		{
			k = 2;
		}
	}
	else if((input.find("ﾍﾔ", 0) != -1) && (input.find("ﾐﾙ", 0) != -1))
	{
		k = 3;
	}
}

// 4410 犯人
void Scenario4410(dms::String& input, int type, int& k, int& l)
{
	if(input.find("ｸﾏﾀﾞ", 0) != -1)
	{
		k = 1;
	}
	else if(input.find("ｼｬﾁｮｳ", 0) != -1)
	{
		k = 2;
	}
	else if(input.find("ﾏｻ", 0) != -1)
	{
		k = 3;
	}
	else if(input.find("ﾁﾊﾞ", 0) != -1)
	{
		k = 4;
	}
	else if(input.find("ｷﾐｶﾞｷ", 0) != -1)
	{
		k = 5;
	}
	else if(input.find("ｺｳ", 0) != -1)
	{
		k = 6;
	}
	else if(input.find("ﾐﾄ", 0) != -1)
	{
		k = 7;
	}
	else if(input.find("ﾜﾀｼ", 0) != -1)
	{
		k = 8;
	}
	else if(input.find("ｶﾜﾀﾞ", 0) != -1)
	{
		k = 9;
	}
	else if(input.find("ｵﾏｴ", 0) != -1)
	{
		k = 10;
	}
	else if(input.find("ｱｲﾂ", 0) != -1)
	{
		k = 11;
	}
}

// シナリオ制御
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

// 矩形表示
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
