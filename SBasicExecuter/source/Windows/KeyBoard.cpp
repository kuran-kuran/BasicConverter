#include "KeyBoard.hpp"
#include "../Format.hpp"

KeyBoard::KeyBoard(void)
:code(0)
,kana(false)
,grph(false)
,shift(false)
,sftlock(false)
,repeat(false)
{
	this->changeTable =
	{
		{ 40, {  1,   1,   1,   1,   1,   1}}, // Å´
		{ 38, {  2,   2,   2,   2,   2,   2}}, // Å™
		{ 39, {  3,   3,   3,   3,   3,   3}}, // Å®
		{ 37, {  4,   4,   4,   4,   4,   4}}, // Å©
		{ 36, {  5,   6,   5,   5,   5,   5}}, // HOME/CLR
		{  8, {  7,   7,   7,   7,   7,   7}}, // BS Å® DEL
		{ 46, {  7,   7,   7,   7,   7,   7}}, // DEL
		{ 45, {  8,   8,   8,   8,   8,   8}}, // INST
		{ 13, { 13,  13,  13,  13,  13,  13}}, // CR
		{ 32, { 32,  32,  32,  32,  32,  32}}, // SPACE

		{'1', {'1', '!',   0, 241, 199,   0}},
		{'2', {'2', '\"',  0, 242, 204,   0}},
		{'3', {'3', '#',   0, 243, 177, 167}},
		{'4', {'4', '$',   0, 244, 179, 169}},
		{'5', {'5', '%',   0, 245, 180, 170}},
		{'6', {'6', '&',   0, 246, 181, 171}},
		{'7', {'7', '\'',  0, 247, 212, 172}},
		{'8', {'8', '(',   0, 248, 213, 173}},
		{'9', {'9', ')',   0, 249, 214, 174}},
		{'0', {'0', '_',   0, 240, 220, 166}},
		{189, {'-', '=',   0,   0, 206,   0}},
		{222, { 94, 126,   0,   0, 205,   0}},
		{220, { 92, 124,   0,   0, 166,   0}},

		{'Q', {'Q', 'q', 151, 251, 192,   0}},
		{'W', {'W', 'w', 149, 164, 195,   0}},
		{'E', {'E', 'e', 158, 229, 178,   0}},
		{'R', {'R', 'r', 159, 252, 189,   0}},
		{'T', {'T', 't', 156, 254, 182,   0}},
		{'Y', {'Y', 'y', 157, 176, 221,   0}},
		{'U', {'U', 'u', 138, 146, 197,   0}},
		{'I', {'I', 'i', 137, 233, 198,   0}},
		{'O', {'O', 'o', 140, 239, 215,   0}},
		{'P', {'P', 'p', 139, 250, 190,   0}},
		{192, {'@', '`',   0,   0, 222,   0}},
		{219, {'[', '{',   0,   0, 223, 162}},

		{'A', {'A', 'a', 152, 225, 193,   0}},
		{'S', {'S', 's', 150, 253, 196,   0}},
		{'D', {'D', 'd', 154, 228, 188,   0}},
		{'F', {'F', 'f', 155, 230, 202,   0}},
		{'G', {'G', 'g', 153, 231, 183,   0}},
		{'H', {'H', 'h', 128, 232, 184,   0}},
		{'J', {'J', 'j', 144, 234, 207,   0}},
		{'K', {'K', 'k', 141, 235, 201,   0}},
		{'L', {'L', 'l', 143, 236, 216,   0}},
		{187, {';', '+',   0,   0, 218,   0}},
		{186, {':', '*',   0,   0, 185,   0}},
		{221, {']', '}',   0,   0, 209,   0}},

		{'Z', {'Z', 'z', 142, 224, 194, 175}},
		{'X', {'X', 'x', 133, 165, 187,   0}},
		{'C', {'C', 'c', 134, 227, 191,   0}},
		{'V', {'V', 'v', 135, 160, 203,   0}},
		{'B', {'B', 'b', 136, 226, 186,   0}},
		{'N', {'N', 'n', 148, 238, 208,   0}},
		{'M', {'M', 'm', 145, 237, 211,   0}},
		{188, {',', '<', 255,   0, 200,   0}},
		{190, {'.', '>',   0,   0, 217, 161}},
		{191, {'/', 132, 131,   0, 210,   0}},
		{226, {'?', 130, 129,   0, 219,   0}},

		{ 97, {'1', '1', '1', '1', '1', '1'}},
		{ 98, {'2', '2', '2', '2', '2', '2'}},
		{ 99, {'3', '3', '3', '3', '3', '3'}},
		{100, {'4', '4', '4', '4', '4', '4'}},
		{101, {'5', '5', '5', '5', '5', '5'}},
		{102, {'6', '6', '6', '6', '6', '6'}},
		{103, {'7', '7', '7', '7', '7', '7'}},
		{104, {'8', '8', '8', '8', '8', '8'}},
		{105, {'9', '9', '9', '9', '9', '9'}},
		{ 96, {'0', '0', '0', '0', '0', '0'}},
		{107, {'+', '+', '+', '+', '+', '+'}},
		{109, {'-', '-', '-', '-', '-', '-'}},

		{112, {112, 112, 112, 112, 112, 112}},
		{113, {113, 113, 113, 113, 113, 113}},
		{114, {114, 114, 114, 114, 114, 114}},
		{115, {115, 115, 115, 115, 115, 115}},
		{116, {116, 116, 116, 116, 116, 116}},
		{117, {117, 117, 117, 117, 117, 117}},
		{118, {118, 118, 118, 118, 118, 118}},
		{119, {119, 119, 119, 119, 119, 119}},
		{120, {120, 120, 120, 120, 120, 120}},
		{121, {121, 121, 121, 121, 121, 121}}

	};
}

KeyBoard::~KeyBoard(void)
{
}

void KeyBoard::SetKeyUp(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 16)
	{
		this->shift = false;
		return;
	}
	OutputDebugStringA(dms::Format("keyup  : [%c], [%u], [%x], kana[%d]\n", static_cast<unsigned char>(wParam), static_cast<unsigned char>(wParam), static_cast<unsigned char>(wParam), this->kana).c_str());
	if(this->beforeCode == wParam)
	{
		this->code = 0;
		this->beforeCode = 0;
	}
}

void KeyBoard::SetKeyDown(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 16)
	{
		this->shift = true;
//		OutputDebugStringA(dms::Format("shift[%d]\n", this->shift).c_str());
		return;
	}
	if(wParam == VK_MENU)
	{
		this->grph = !this->grph;
		this->kana = this->grph ? false : this->grph;
//		OutputDebugStringA(dms::Format("grph[%d]\n", this->grph).c_str());
		return;
	}
	if(wParam == 242)
	{
		this->kana = !this->kana;
		this->grph = this->kana ? false : this->kana;
//		OutputDebugStringA(dms::Format("kana[%d]\n", this->kana).c_str());
		return;
	}
//	OutputDebugStringA(dms::Format("keydown: [%c], [%u], [%x], kana[%d], grph[%d]\n", static_cast<unsigned char>(wParam), static_cast<unsigned char>(wParam), static_cast<unsigned char>(wParam), this->kana, this->grph).c_str());
	unsigned char code = static_cast<unsigned char>(wParam);
	if(changeTable.find(code) == changeTable.end())
	{
		return;
	}
	if((this->shift == true) & (this->grph == true))
	{
		this->code = this->changeTable[code].shift_grph;
	}
	else if((this->shift == true) & (this->kana == true))
	{
		this->code = this->changeTable[code].shift_kana;
	}
	else if(this->shift == true)
	{
		this->code = this->changeTable[code].shift;
	}
	else if(this->grph == true)
	{
		this->code = this->changeTable[code].grph;
	}
	else if(this->kana == true)
	{
		this->code = this->changeTable[code].kana;
	}
	else
	{
		this->code = this->changeTable[code].normal;
	}
	this->beforeCode = wParam;
}

void KeyBoard::SetGrph(bool grph)
{
	this->grph = grph;
}

void KeyBoard::SetKana(bool kana)
{
	this->kana = kana;
}

bool KeyBoard::GetGrph(void)
{
	return this->grph;
}

bool KeyBoard::GetKana(void)
{
	return this->kana;
}

unsigned char KeyBoard::Get(void)
{
	unsigned resultCode = this->code;
	if(this->repeat == false)
	{
		this->code = 0;
		this->beforeCode = 0;
	}
	return resultCode;
}

void KeyBoard::SetRepeat(bool enable)
{
	this->repeat = enable;
}
