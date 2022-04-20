#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Executer.hpp"

enum
{
	O_0 = 0b000,
	O_1 = 0b001,
	O_2 = 0b010,
	O_3 = 0b100,
	O_12 = 0b011,
	O_21 = O_12,
	O_13 = 0b101,
	O_31 = O_13,
	O_23 = 0b011,
	O_32 = O_23,
	O_123 = 0b111
};

inline void Cls(void)
{
	Executer::GetInstance()->Cls();
}

inline void Gosub(int jumpLine)
{
	Executer::GetInstance()->Gosub(jumpLine);
}

inline void Return(void)
{
	Executer::GetInstance()->Return();
}

inline void Goto(int jumpLine)
{
	Executer::GetInstance()->Goto(jumpLine);
}

inline void Stop(void)
{
	Executer::GetInstance()->Stop();
}

inline void End(void)
{
	Executer::GetInstance()->End();
}

inline void For(dms::Variable* index, dms::Variable start, dms::Variable to, dms::Variable step)
{
	Executer::GetInstance()->For(index, start, to, step);
}

inline void Next(dms::Variable* index)
{
	Executer::GetInstance()->Next(index);
}

void If(bool conditions, int matchJumpLine, int mismatchJumpLine)
{
	return Executer::GetInstance()->If(conditions, matchJumpLine, mismatchJumpLine);
}

inline void OnGoto(dms::Variable number, std::vector<int> numberList)
{
	Executer::GetInstance()->OnGoto(number, numberList);
}

inline void OnGosub(dms::Variable number, std::vector<int> numberList)
{
	Executer::GetInstance()->OnGosub(number, numberList);
}

inline void Cursor(dms::Variable x, dms::Variable y)
{
	Executer::GetInstance()->Cursor(x, y);
}

inline void Print(dms::String text, bool newline)
{
	Executer::GetInstance()->Print(text, newline);
}

inline void Set(dms::Variable x, dms::Variable y)
{
	Executer::GetInstance()->Set(x, y);
}

inline void Reset(dms::Variable x, dms::Variable y)
{
	Executer::GetInstance()->Reset(x, y);
}

inline void Line(dms::Variable x0, dms::Variable y0, dms::Variable x1, dms::Variable y1)
{
	Executer::GetInstance()->Line(x0, y0, x1, y1);
}

inline void Line(std::vector<dms::Variable> positionList)
{
	Executer::GetInstance()->Line(positionList);
}

inline void Bline(std::vector<dms::Variable> positionList)
{
	Executer::GetInstance()->Bline(positionList);
}

inline void Paint(dms::Variable x, dms::Variable y, unsigned int color, std::vector<unsigned int> boarderColorList)
{
	Executer::GetInstance()->Paint(x, y, color, boarderColorList);
}

inline void SetData(int number, std::vector<Data> data)
{
	Executer::GetInstance()->SetData(number, data);
}

inline dms::Variable ReadVariable(void)
{
	return Executer::GetInstance()->ReadVariable();
}

inline dms::String ReadString(void)
{
	return Executer::GetInstance()->ReadString();
}

inline void Restore(int number)
{
	Executer::GetInstance()->Restore(number);
}
inline void Position(dms::Variable x, dms::Variable y)
{
	Executer::GetInstance()->Position(x, y);
}

inline void Pattern(dms::Variable row, dms::String pattern)
{
	Executer::GetInstance()->Pattern(row, pattern);
}

inline void GraphInput(int input)
{
	Executer::GetInstance()->GraphInput(input);
}

inline void GraphOutput(unsigned int output)
{
	Executer::GetInstance()->GraphOutput(output);
}

inline void ClearGraph(void)
{
	return Executer::GetInstance()->ClearGraph();
}

inline void FillGraph(void)
{
	return Executer::GetInstance()->FillGraph();
}

inline void SetStretchWidth(int stretch)
{
	return Executer::GetInstance()->SetStretchWidth(stretch);
}

inline void SetStretchHeight(int stretch)
{
	return Executer::GetInstance()->SetStretchHeight(stretch);
}

inline void SetTextStretchWidth(int stretch)
{
	return Executer::GetInstance()->SetTextStretchWidth(stretch);
}

inline void SetTextStretchHeight(int stretch)
{
	return Executer::GetInstance()->SetTextStretchHeight(stretch);
}

inline void ScrollXRange(int left, int right)
{
	return Executer::GetInstance()->ScrollXRange(left, right);
}

inline void ScrollYRange(int top, int bottom)
{
	return Executer::GetInstance()->ScrollYRange(top, bottom);
}

inline void DefKey(dms::Variable index, dms::String defKeyText)
{
	Executer::GetInstance()->DefKey(index, defKeyText);
}

inline dms::Variable Csrh(void)
{
	return Executer::GetInstance()->Csrh();
}

inline dms::Variable Csrv(void)
{
	return Executer::GetInstance()->Csrv();
}

inline dms::Variable Posh(void)
{
	return Executer::GetInstance()->Posh();
}

inline dms::Variable Posv(void)
{
	return Executer::GetInstance()->Posv();
}

inline dms::String Character(dms::Variable x, dms::Variable y)
{
	return Executer::GetInstance()->Character(x, y);
}

inline dms::String Time(void)
{
	return Executer::GetInstance()->Time();
}

inline void SetTime(dms::String time)
{
	return Executer::GetInstance()->SetTime(time);
}

inline dms::String Left(dms::String text, dms::Variable n)
{
	return Executer::GetInstance()->Left(text, n);
}

inline dms::String Right(dms::String text, dms::Variable n)
{
	return Executer::GetInstance()->Right(text, n);
}

inline dms::String Mid(dms::String text, dms::Variable m, dms::Variable n)
{
	return Executer::GetInstance()->Mid(text, m, n);
}

inline dms::Variable Len(dms::String text)
{
	return Executer::GetInstance()->Len(text);
}

inline dms::String Chr(dms::Variable chrCode)
{
	return Executer::GetInstance()->Chr(chrCode);
}

inline dms::String Str(dms::Variable number)
{
	return Executer::GetInstance()->Str(number);
}

inline dms::Variable Asc(dms::String text)
{
	return Executer::GetInstance()->Asc(text);
}

inline dms::Variable Val(dms::String text)
{
	return Executer::GetInstance()->Val(text);
}

inline dms::Variable Peek(dms::Variable address)
{
	return Executer::GetInstance()->Peek(address);
}

inline dms::String Tab(dms::Variable number)
{
	return Executer::GetInstance()->Tab(number);
}

inline dms::String Space(dms::Variable number)
{
	return Executer::GetInstance()->Space(number);
}

inline dms::Variable Size(void)
{
	return Executer::GetInstance()->Size();
}

inline dms::String String(dms::String text, dms::Variable number)
{
	return Executer::GetInstance()->String(text, number);
}

inline dms::Variable Rnd(dms::Variable number)
{
	return Executer::GetInstance()->Rnd(number);
}

inline dms::Variable Sin(dms::Variable number)
{
	return Executer::GetInstance()->Sin(number);
}

inline dms::Variable Cos(dms::Variable number)
{
	return Executer::GetInstance()->Cos(number);
}

inline dms::Variable Tan(dms::Variable number)
{
	return Executer::GetInstance()->Tan(number);
}

inline dms::Variable Atn(dms::Variable number)
{
	return Executer::GetInstance()->Atn(number);
}

inline dms::Variable Exp(dms::Variable number)
{
	return Executer::GetInstance()->Exp(number);
}

inline dms::Variable Int(dms::Variable number)
{
	return Executer::GetInstance()->Int(number);
}

inline dms::Variable Log(dms::Variable number)
{
	return Executer::GetInstance()->Log(number);
}

inline dms::Variable Ln(dms::Variable number)
{
	return Executer::GetInstance()->Ln(number);
}

inline dms::Variable Abs(dms::Variable number)
{
	return Executer::GetInstance()->Abs(number);
}

inline dms::Variable Sgn(dms::Variable number)
{
	return Executer::GetInstance()->Sgn(number);
}

inline dms::Variable Sqr(dms::Variable number)
{
	return Executer::GetInstance()->Sqr(number);
}

inline void Input(dms::String* inputText, bool showQuestion)
{
	Executer::GetInstance()->Input(inputText, showQuestion);
}

inline void Input(dms::Variable* inputNumber, bool showQuestion)
{
	Executer::GetInstance()->Input(inputNumber, showQuestion);
}

inline void Get(dms::String* getText)
{
	Executer::GetInstance()->Get(getText);
}

inline void Get(dms::Variable* getNumber)
{
	Executer::GetInstance()->Get(getNumber);
}

inline dms::Variable Pi(void)
{
	return Executer::GetInstance()->Pi();
}

inline void Tempo(dms::Variable tempo)
{
	Executer::GetInstance()->Tempo(tempo);
}

inline void Music(dms::String mml)
{
	Executer::GetInstance()->Music(mml);
}

inline void Poke(dms::Variable address, dms::Variable data)
{
	Executer::GetInstance()->Poke(address, data);
}

inline void Usr(dms::Variable address)
{
	Executer::GetInstance()->Usr(address);
}

inline void Limit(dms::Variable address)
{
	Executer::GetInstance()->Limit(address);
}

inline void DebugLog(dms::String text)
{
	Executer::GetInstance()->DebugLog(text);
}

#endif
