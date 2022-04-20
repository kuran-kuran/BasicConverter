#ifndef EXECUTER_HPP
#define EXECUTER_HPP

#include <stdarg.h>
#include <time.h>
#include <vector>
#include <random>
#include "Stack.hpp"
#include "Variable.hpp"
#include "Screen.hpp"
#include "DataManager.hpp"
#include "Array.hpp"
#include "BeepMusic.hpp"
#include "Format.hpp"

class Executer
{
public:
	static Executer* GetInstance(void);
	void Initialize(std::vector<void (*)(void)> lineList, unsigned int* frameBuffer);
	bool ExecuteLoop(void);
	void ExecuteNoWaitLoop(void);
	void Clear(void);
	void Cls(void);
	void Flip(void);
	void Gosub(int jumpLine);
	void Return(void);
	void Goto(int jumpLine);
	void Stop(void);
	void End(void);
	void For(dms::Variable* index, dms::Variable start, dms::Variable to, dms::Variable step);
	void Next(dms::Variable* index);
	void If(bool conditions, int matchJumpLine, int mismatchJumpLine);
	void OnGosub(dms::Variable number, std::vector<int> numberList);
	void OnGoto(dms::Variable number, std::vector<int> numberList);
	void Cursor(dms::Variable x, dms::Variable y);
	void Print(dms::String text, bool newline);
	void Set(dms::Variable x, dms::Variable y);
	void Reset(dms::Variable x, dms::Variable y);
	void Line(dms::Variable x0, dms::Variable y0, dms::Variable x1, dms::Variable y1);
	void Line(std::vector<dms::Variable> positionList);
	void Bline(std::vector<dms::Variable> positionList);
	void Paint(dms::Variable x, dms::Variable y, unsigned int color, std::vector<unsigned int> boarderColorList);
	void SetData(int number, std::vector<Data> data);
	dms::Variable ReadVariable(void);
	dms::String ReadString(void);
	void Restore(int number);
	void Position(dms::Variable x, dms::Variable y);
	void Pattern(dms::Variable row, dms::String pattern);
	void GraphInput(int input);
	void GraphOutput(unsigned int output);
	void ClearGraph(void);
	void FillGraph(void);
	void SetStretchWidth(int stretch);
	void SetStretchHeight(int stretch);
	void SetTextStretchWidth(int stretch);
	void SetTextStretchHeight(int stretch);
	void ScrollXRange(int left, int right);
	void ScrollYRange(int top, int bottom);
	void DefKey(dms::Variable index, dms::String defKeyText);
	void SetTime(dms::String time);
	dms::String Time(void);
	dms::String Left(dms::String text, dms::Variable n);
	dms::String Right(dms::String text, dms::Variable n);
	dms::String Mid(dms::String text, dms::Variable m, dms::Variable n);
	dms::Variable Len(dms::String text);
	dms::String Chr(dms::Variable code);
	dms::String Str(dms::Variable number);
	dms::Variable Asc(dms::String text);
	dms::Variable Val(dms::String text);
	dms::Variable Peek(dms::Variable address);
	dms::String Tab(dms::Variable number);
	dms::String Space(dms::Variable number);
	dms::Variable Size(void);
	dms::String String(dms::String text, dms::Variable number);
	dms::Variable Rnd(dms::Variable number);
	dms::Variable Sin(dms::Variable number);
	dms::Variable Cos(dms::Variable number);
	dms::Variable Tan(dms::Variable number);
	dms::Variable Atn(dms::Variable number);
	dms::Variable Exp(dms::Variable number);
	dms::Variable Int(dms::Variable number);
	dms::Variable Log(dms::Variable number);
	dms::Variable Ln(dms::Variable number);
	dms::Variable Abs(dms::Variable number);
	dms::Variable Sgn(dms::Variable number);
	dms::Variable Sqr(dms::Variable number);
	dms::Variable Csrh(void);
	dms::Variable Csrv(void);
	dms::Variable Posh(void);
	dms::Variable Posv(void);
	dms::String Character(dms::Variable x, dms::Variable y);
	void Input(dms::String* inputText, bool showQuestion);
	void Input(dms::Variable* inputNumber, bool showQuestion);
	void StoreInput(void);
	void EndInput(void);
	void Get(dms::String* getText);
	void Get(dms::Variable* getNumber);
	dms::Variable Pi(void);
	void Tempo(dms::Variable tempo);
	void Music(dms::String mml);
	void Poke(dms::Variable address, dms::Variable data);
	void Usr(dms::Variable address);
	void Limit(dms::Variable address);
	void DebugLog(dms::String text);
private:
	Executer(void);
	~Executer(void);
	static const size_t mainMemorySize;
	static const size_t textMemorySize;
	static const size_t graphicMemorySize;
	static const size_t ioMemorySize;
	static const size_t defKeySize;
	static const size_t tabSettingAddress;
	static const size_t tabSettingSize;
	static Executer* instance;
	int executeLine;
	int jumpLine;
	bool end;
	std::vector<void (*)(void)> lineList;
	dms::Stack stack;
	Screen screen;
	DataManager dataManager;
	unsigned int colorMask;
	unsigned int outputGraphicColorMask;
	unsigned int outputTextColorMask;
	clock_t startTime;
	std::vector<unsigned char> mainMemory;
	std::vector<unsigned char> textMemory;
	std::vector<unsigned char> graphicMemory[3];
	std::vector<unsigned char> ioMemory;
	std::vector<dms::String> defKey;
	std::mt19937 mt;
	bool input;
	dms::String inputText;
	dms::String* storeText;
	dms::Variable* storeNumber;
	BeepMusic beepMusic;
	bool isOutputLog;
};
#endif
