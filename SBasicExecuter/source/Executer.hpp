#ifndef EXECUTER_HPP
#define EXECUTER_HPP

#include <stdarg.h>
#include <time.h>
#include <vector>
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>
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
	void Set(dms::Variable x, dms::Variable y, dms::Variable color = -1, dms::Variable overlap = -1);
	void Reset(dms::Variable x, dms::Variable y, dms::Variable color = -1, dms::Variable overlap = -1);
	void Line(dms::Variable x0, dms::Variable y0, dms::Variable x1, dms::Variable y1, dms::Variable color = -1, dms::Variable overlap = -1);
	void Line(std::vector<dms::Variable> positionList, dms::Variable color = -1, dms::Variable overlap = -1);
	void Bline(std::vector<dms::Variable> positionList, dms::Variable color = -1, dms::Variable overlap = -1);
	void Paint(dms::Variable x, dms::Variable y, std::vector<dms::Variable> boarderColorList, dms::Variable color = -1, dms::Variable overlap = -1);
	void Box(dms::Variable x1, dms::Variable y1, dms::Variable x2, dms::Variable y2, dms::Variable f = -1, dms::Variable color = -1, dms::Variable overlap =-1);
	void Circle(dms::Variable x, dms::Variable y, dms::Variable r, dms::Variable h = -1, dms::Variable ks = -1, dms::Variable ke = -1, dms::Variable o = 0, dms::Variable color = -1, dms::Variable overlap = -1);
	void Color(dms::Variable priority, dms::Variable color, dms::Variable output, dms::Variable overlap);
	void CColor(dms::Variable priority, dms::Variable color, dms::Variable backGroundColor);
	void SetData(int number, std::vector<Data> data);
	dms::Variable ReadVariable(void);
	dms::String ReadString(void);
	void Restore(int number);
	void Position(dms::Variable x, dms::Variable y);
	void Pattern(dms::Variable row, dms::String pattern, dms::Variable color = -1, dms::Variable overlap = -1);
	void GraphInput(int input);
	void GraphOutput(unsigned int output);
	void ClearGraph(dms::Variable color = -1);
	void FillGraph(dms::Variable color = -1);
	void SetStretchWidth(dms::Variable stretch);
	void SetStretchHeight(dms::Variable stretch);
	void SetTextStretchWidth(dms::Variable stretch);
	void SetTextStretchHeight(dms::Variable stretch);
	void ScrollXRange(dms::Variable left, dms::Variable right);
	void ScrollYRange(dms::Variable top, dms::Variable bottom);
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
	void SetCsrh(dms::Variable h);
	void SetCsrv(dms::Variable v);
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
	void Usr(dms::Variable address, dms::String option = "");
	void Limit(dms::Variable address);
	void DebugLog(dms::String text);
	void Out(dms::Variable ioAddress, dms::Variable data);
	void Run(void);
	Screen& GetScreen(void);
	void UsrPatch(dms::Variable address, void (*callback)(dms::String*));
private:
	Executer(void);
	~Executer(void);
	unsigned int GetColor(int colorCode);
	unsigned int GetBColor(int colorCode);
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
	unsigned int backGroundColor;
	int priority;
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
	int overlap;
	std::map<int, void (*)(dms::String*)> usrPatchList;
};
#endif
