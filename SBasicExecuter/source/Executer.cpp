#include <stdarg.h>
#include <iostream>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Executer.hpp"
#include "Variable.hpp"
#include "FileData.hpp"
#include "Format.hpp"
#include "Dependent.hpp"
#include "Config.hpp"

Executer* Executer::instance = NULL;

const size_t Executer::mainMemorySize = 64 * 1024;
const size_t Executer::textMemorySize = 2 * 1024;
const size_t Executer::graphicMemorySize = 16 * 1024;
const size_t Executer::ioMemorySize = 256;
const size_t Executer::defKeySize = 10;
const size_t Executer::tabSettingAddress = 0x11C1;
const size_t Executer::tabSettingSize = 15;

Executer::Executer(void)
:executeLine(0)
,jumpLine(-1)
,end(false)
,lineList()
,stack()
,screen()
,dataManager()
,colorMask(0x000000FF)
,outputGraphicColorMask(0x00000000)
,outputTextColorMask(0xFFFFFFFF)
,startTime(clock())
,mainMemory(mainMemorySize, 0)
,textMemory(textMemorySize, 0)
,graphicMemory()
,ioMemory(ioMemorySize, 0)
,defKey()
,mt()
,input(false)
,inputText()
,storeText(NULL)
,storeNumber(NULL)
,beepMusic()
,isOutputLog(false)
{
}

Executer::~Executer(void)
{
}

Executer* Executer::GetInstance(void)
{
	if(Executer::instance == NULL)
	{
		Executer::instance = new Executer();
	}
	return Executer::instance;
}

void Executer::Initialize(std::vector<void (*)(void)> lineList, unsigned int* frameBuffer)
{
	DebugLog("Executer initialize\n");
	this->lineList = lineList;
	this->jumpLine = -1;
	this->end = false;
	// 画面設定
	DebugLog("Screen initialize\n");
	this->screen.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, FONT_WIDTH, FONT_HEIGHT, frameBuffer);
	// フォント設定
	DebugLog("Load font\n");
	FileData fontfile;
	fontfile.Load("mz2000font.png");
	this->screen.LoadFont(fontfile.GetBuffer(), static_cast<int>(fontfile.GetBufferSize()));
	// ファンクションキー設定
	this->defKey =
	{
		dms::String("RUN  ") + Chr(13), dms::String("LIST") + Chr(13), "CONSOLE", dms::String("CONT") + Chr(13), dms::String("AUTO") + Chr(13),
		"CHR$(", "DEF KEY(", "GRAPH", "SAVE\"", dms::String("LOAD ") + Chr(13)
	};
	// グラフィックメモリクリア
	this->graphicMemory[0].assign(graphicMemorySize, 0);
	this->graphicMemory[1].assign(graphicMemorySize, 0);
	this->graphicMemory[2].assign(graphicMemorySize, 0);
	// オートリピート設定
	this->mainMemory[0x952] = 166;
	// タブ位置設定
	this->mainMemory[0x11C1] = 5;
	this->mainMemory[0x11C2] = 10;
	this->mainMemory[0x11C3] = 15;
	this->mainMemory[0x11C4] = 20;
	this->mainMemory[0x11C5] = 25;
	this->mainMemory[0x11C6] = 30;
	this->mainMemory[0x11C7] = 35;
	this->mainMemory[0x11C8] = 40;
	this->mainMemory[0x11C9] = 45;
	this->mainMemory[0x11CA] = 50;
	this->mainMemory[0x11CB] = 55;
	this->mainMemory[0x11CC] = 60;
	this->mainMemory[0x11CD] = 65;
	this->mainMemory[0x11CE] = 70;
	this->mainMemory[0x11CF] = 75;
	// 乱数初期化
	std::random_device randomDevice;
	unsigned int seedValue = randomDevice();
	this->mt.seed(seedValue);
	DebugLog("Initialize sound\n");
	// サウンド初期化
	SoundStream& soundStream = SoundStream::GetInstance();
	soundStream.Initialize(48000);
}

bool Executer::ExecuteLoop(void)
{
	if(this->beepMusic.Playing() == true)
	{
		return this->end;
	}
	if(this->input == true)
	{
		StoreInput();
		return this->end;
	}
	if(this->executeLine >= this->lineList.size())
	{
		return this->end;
	}
	DebugLog(dms::Format("Execute line: %d\n", this->executeLine));
	this->lineList[this->executeLine]();
	if(this->jumpLine == -1)
	{
		++ this->executeLine;
	}
	else
	{
		this->executeLine = this->jumpLine;
		this->jumpLine = -1;
	}
	return this->end;
}

void Executer::ExecuteNoWaitLoop(void)
{
	SoundStream& soundStream = SoundStream::GetInstance();
	soundStream.Update();
	int writeSize = soundStream.GetWritableSize();
	if(writeSize > 0)
	{
		std::vector<short> musicBuffer;
		beepMusic.GetBuffer(musicBuffer, writeSize);
		soundStream.Write(&musicBuffer[0]);
	}
}

void Executer::Flip(void)
{
	if(this->input == false)
	{
		DebugLog("Flip\n");
	}
	this->screen.Flip(this->outputTextColorMask, this->outputGraphicColorMask);
}

void Executer::Clear(void)
{
	this->screen.Clear(this->colorMask);
}

void Executer::Cls(void)
{
	this->screen.ClearText();
}

void Executer::Gosub(int jumpLine)
{
	this->jumpLine = jumpLine;
	this->stack.PushInt(this->executeLine + 1);
}

void Executer::Return(void)
{
	this->jumpLine = this->stack.GetInt(dms::Stack::intSize);
	this->stack.PopInt();
}

void Executer::Goto(int jumpLine)
{
	this->jumpLine = jumpLine;
}

void Executer::Stop(void)
{
	this->end = true;
}

void Executer::End(void)
{
	this->end = true;
}

void Executer::For(dms::Variable* index, dms::Variable start, dms::Variable to, dms::Variable step)
{
	*index = start;
	this->stack.PushInt(this->executeLine + 1);
	this->stack.PushPointer(index);
	this->stack.PushVariable(to);
	this->stack.PushVariable(step);
}

void Executer::Next(dms::Variable* index)
{
	if(index != NULL)
	{
		size_t stackSize = this->stack.GetSize();
		if(stackSize > 0)
		{
			for(size_t i = 0; i < stackSize / 11; ++ i)
			{
				size_t rewindIndex = i * 11 + 10;
				dms::Variable* tempIndex = reinterpret_cast<dms::Variable*>(this->stack.GetPointer(rewindIndex));
				if(index == tempIndex)
				{
					if(i == 0)
					{
						break;
					}
					size_t popSize = i * 11;
					if(popSize != 0)
					{
						this->stack.Pop(popSize);
						break;
					}
				}
			}
		}
	}
	dms::Variable step = this->stack.GetVariable(dms::Stack::variableSize);
	this->stack.PopVariable();
	dms::Variable to = this->stack.GetVariable(dms::Stack::variableSize);
	this->stack.PopVariable();
	dms::Variable* start = reinterpret_cast<dms::Variable*>(this->stack.GetPointer(dms::Stack::pointerSize));
	this->stack.PopPointer();
	int jumpLine = this->stack.GetInt(dms::Stack::intSize);
	this->stack.PopInt();
	*start += step;
	if(step > 0)
	{
		if(*start > to)
		{
			return;
		}
	}
	else if(step < 0)
	{
		if(*start < to)
		{
			return;
		}
	}
	this->stack.PushInt(jumpLine);
	this->stack.PushPointer(start);
	this->stack.PushVariable(to);
	this->stack.PushVariable(step);
	this->jumpLine = jumpLine;
}

void Executer::If(bool conditions, int matchJumpLine, int mismatchJumpLine)
{
	if(conditions)
	{
		this->jumpLine = matchJumpLine;
	}
	else
	{
		this->jumpLine = mismatchJumpLine;
	}
}

void Executer::OnGosub(dms::Variable number, std::vector<int> numberList)
{
	if((number < 1) || (number.GetInt() > static_cast<int>(numberList.size())))
	{
		return;
	}
	int index = number.GetInt() - 1;
	Gosub(numberList[index]);
}

void Executer::OnGoto(dms::Variable number, std::vector<int> numberList)
{
	if ((number < 1) || (number.GetInt() > static_cast<int>(numberList.size())))
	{
		return;
	}
	int index = number.GetInt() - 1;
	Goto(numberList[index]);
}

void Executer::Cursor(dms::Variable x, dms::Variable y)
{
	this->screen.SetTextX(x.GetInt());
	this->screen.SetTextY(y.GetInt());
}

void Executer::Print(dms::String text, bool newline)
{
	this->screen.Print(text, newline);
}

void Executer::Set(dms::Variable x, dms::Variable y)
{
	this->screen.DrawPoint(x.GetInt(), y.GetInt(), 0xFFFFFFFF, this->colorMask);
}

void Executer::Reset(dms::Variable x, dms::Variable y)
{
	this->screen.DrawPoint(x.GetInt(), y.GetInt(), 0, this->colorMask);
}

void Executer::Line(dms::Variable x0, dms::Variable y0, dms::Variable x1, dms::Variable y1)
{
	this->screen.DrawLine(x0.GetInt(), y0.GetInt(), x1.GetInt(), y1.GetInt(), 0xFFFFFFFF, this->colorMask);
}

void Executer::Line(std::vector<dms::Variable> positionList)
{
	std::vector<int> linePositionList;
	for(size_t i = 0; i < positionList.size(); ++ i)
	{
		linePositionList.push_back(positionList[i].GetInt());
	}
	this->screen.DrawLine(linePositionList, 0xFFFFFFFF, this->colorMask);
}

void Executer::Bline(std::vector<dms::Variable> positionList)
{
	std::vector<int> linePositionList;
	for (size_t i = 0; i < positionList.size(); ++i)
	{
		linePositionList.push_back(positionList[i].GetInt());
	}
	this->screen.DrawLine(linePositionList, 0, this->colorMask);
}

void Executer::Paint(dms::Variable x, dms::Variable y, unsigned int color, std::vector<unsigned int> boarderColorList)
{
	this->screen.Paint(x.GetInt(), y.GetInt(), color, boarderColorList);
}

void Executer::SetData(int number, std::vector<Data> data)
{
	this->dataManager.SetData(number, data);
}

dms::Variable Executer::ReadVariable(void)
{
	return this->dataManager.ReadVariable();
}

dms::String Executer::ReadString(void)
{
	return this->dataManager.ReadString();
}

void Executer::Restore(int number)
{
	this->dataManager.Restore(number);
}

void Executer::Position(dms::Variable x, dms::Variable y)
{
	this->screen.SetGraphicX(x.GetInt());
	this->screen.SetGraphicY(y.GetInt());
}

void Executer::Pattern(dms::Variable row, dms::String pattern)
{
	this->screen.Pattern(row.GetInt(), pattern, 0xFFFFFFFF, this->colorMask);
}

void Executer::GraphInput(int input)
{
	// 描画ページ選択
	switch (input)
	{
	case 1:
		this->colorMask = 0x000000FF;
		break;
	case 2:
		this->colorMask = 0x00FF0000;
		break;
	case 3:
		this->colorMask = 0x0000FF00;
		break;
	}
}

void Executer::GraphOutput(unsigned int output)
{
	// 表示ページ選択
	this->outputGraphicColorMask = 0x0;
	if (output & 0b001)
	{
		this->outputGraphicColorMask |= 0x000000FF;
	}
	if (output & 0b010)
	{
		this->outputGraphicColorMask |= 0x00FF0000;
	}
	if (output & 0b100)
	{
		this->outputGraphicColorMask |= 0x0000FF00;
	}
}

void Executer::ClearGraph(void)
{
	this->screen.Clear(this->colorMask);
}

void Executer::FillGraph(void)
{
	this->screen.Fill(0xFFFFFFFF, this->colorMask);
}

void Executer::SetStretchWidth(int stretch)
{
	this->screen.SetStretchWidth(stretch);
	GraphOutput(0b000);
	GraphInput(1);
	ClearGraph();
	GraphInput(2);
	ClearGraph();
	GraphInput(3);
	ClearGraph();
	this->screen.SetGraphicX(0);
	this->screen.SetGraphicY(0);
}

void Executer::SetStretchHeight(int stretch)
{
	this->screen.SetStretchHeight(stretch);
	GraphOutput(0b000);
	GraphInput(1);
	ClearGraph();
	GraphInput(2);
	ClearGraph();
	GraphInput(3);
	ClearGraph();
	this->screen.SetGraphicX(0);
	this->screen.SetGraphicY(0);
}

void Executer::SetTextStretchWidth(int stretch)
{
	this->screen.SetTextStretchWidth(stretch);
	GraphOutput(0b000);
	Cls();
}

void Executer::SetTextStretchHeight(int stretch)
{
	this->screen.SetTextStretchHeight(stretch);
	GraphOutput(0b000);
	Cls();
}

void Executer::ScrollXRange(int left, int right)
{
	this->screen.ScrollXRange(left, right);
	Cls();
}

void Executer::ScrollYRange(int top, int bottom)
{
	this->screen.ScrollYRange(top, bottom);
	Cls();
}

void Executer::DefKey(dms::Variable index, dms::String defKeyText)
{
	int dekKeyIndex = index.GetInt() - 1;
	this->defKey[dekKeyIndex] = defKeyText;
}

void Executer::SetTime(dms::String time)
{
	if(time.size() != 6)
	{
		return;
	}
	int h = atoi(Left(time, 2).c_str());
	if(h >= 24)
	{
		return;
	}
	int m = atoi(Mid(time, 2, 2).c_str());
	if(m >= 60)
	{
		return;
	}
	int s = atoi(Right(time, 2).c_str());
	if(s >= 60)
	{
		return;
	}
	clock_t setTime = (static_cast<clock_t>(h) * 60 * 60 + static_cast<clock_t>(m) * 60 + s) * CLOCKS_PER_SEC;
	this->startTime = clock() + setTime;
}

dms::String Executer::Time(void)
{
	time_t nowTime = clock();
	time_t bootTime = (nowTime - this->startTime) / CLOCKS_PER_SEC % 86400;
	int h = static_cast<int>((bootTime / 60 / 60) % 60);
	int m = static_cast<int>(bootTime % (60 * 60) / 60);
	int s = static_cast<int>(bootTime % 60);
	return dms::Format("%02d%02d%02d", h, m, s);
}

dms::String Executer::Left(dms::String text, dms::Variable n)
{
	size_t left = n.GetInt();
	if(text.size() < left)
	{
		left = text.size();
	}
	return text.substr(0, left);
}

dms::String Executer::Right(dms::String text, dms::Variable n)
{
	size_t position = 0;
	if(text.size() >= n.GetInt())
	{
		position = text.size() - n.GetInt();
	}
	dms::String z = text.substr(position);
	return text.substr(position);
}

dms::String Executer::Mid(dms::String text, dms::Variable m, dms::Variable n)
{
	return text.substr(static_cast<size_t>(m.GetInt()) - 1, n.GetInt());
}

dms::Variable Executer::Len(dms::String text)
{
	return static_cast<int>(text.size());
}

dms::String Executer::Chr(dms::Variable code)
{
	return dms::Format("%c", code.GetInt());
}

dms::String Executer::Str(dms::Variable number)
{
	if(number > 0)
	{
		return number.ToString().substr(1);
	}
	return number.ToString();
}

dms::Variable Executer::Asc(dms::String text)
{
	if(text.empty() == true)
	{
		return 0;
	}
	return static_cast<int>(text[0]);
}

dms::Variable Executer::Val(dms::String text)
{
	return atoi(text.c_str());
}

dms::Variable Executer::Peek(dms::Variable address)
{
	return this->mainMemory[address.GetInt()];
}

dms::String Executer::Tab(dms::Variable number)
{
	int textX = this->screen.GetTextX() + number.GetInt();
	int x = textX % this->screen.GetTextWidth();
	int addY = textX / this->screen.GetTextWidth();
	// タブ位置にカーソル移動
	this->screen.SetTextX(x);
	if(addY > 0)
	{
		int y = this->screen.GetTextY() + addY;
		if(y >= this->screen.GetTextHeight())
		{
			y = this->screen.GetTextHeight() - 1;
		}
		this->screen.SetTextY(y);
	}
	return "";
}

dms::String Executer::Space(dms::Variable number)
{
	return dms::String(number.GetInt(), ' ');
}

dms::Variable Executer::Size(void)
{
	return 65535;
}

dms::String Executer::String(dms::String text, dms::Variable number)
{
	return dms::String(number.GetInt(), text[0]);
}

dms::Variable Executer::Rnd(dms::Variable number)
{
	if(number <= 0)
	{
		// 乱数を初期化する
		this->mt.seed();
	}
	std::uniform_real_distribution<> rand1(0.0, 1.0);
	dms::Variable rand = rand1(this->mt);
	return rand;
}

dms::Variable Executer::Sin(dms::Variable number)
{
	return sin(number.GetDouble());
}

dms::Variable Executer::Cos(dms::Variable number)
{
	return cos(number.GetDouble());
}

dms::Variable Executer::Tan(dms::Variable number)
{
	return tan(number.GetDouble());
}

dms::Variable Executer::Atn(dms::Variable number)
{
	return atan(number.GetDouble());
}

dms::Variable Executer::Exp(dms::Variable number)
{
	return exp(number.GetDouble());
}

dms::Variable Executer::Int(dms::Variable number)
{
	return number.GetInt();
}

dms::Variable Executer::Log(dms::Variable number)
{
	return log10(number.GetDouble());
}

dms::Variable Executer::Ln(dms::Variable number)
{
	return log(number.GetDouble());
}

dms::Variable Executer::Abs(dms::Variable number)
{
	return number < 0 ? -number : number;
}

dms::Variable Executer::Sgn(dms::Variable number)
{
	if(number == 0)
	{
		return 0;
	}
	return number < 0 ? -1 : 1;
}

dms::Variable Executer::Sqr(dms::Variable number)
{
	return sqrt(number.GetDouble());
}

dms::Variable Executer::Csrh(void)
{
	return this->screen.GetTextX();
}

dms::Variable Executer::Csrv(void)
{
	return this->screen.GetTextY();
}

dms::Variable Executer::Posh(void)
{
	return this->screen.GetGraphicX();
}

dms::Variable Executer::Posv(void)
{
	return this->screen.GetGraphicY();
}

dms::String Executer::Character(dms::Variable x, dms::Variable y)
{
	return dms::Format("%c", this->screen.GetScreenText(x.GetInt(), y.GetInt()));
}

void Executer::Input(dms::String* inputText, bool showQuestion)
{
	this->inputText = "";
	this->input = true;
	this->storeText = inputText;
	this->storeNumber = NULL;
	if(showQuestion == true)
	{
		Print("? ", false);
	}
	this->screen.ShowCursor();
}

void Executer::Input(dms::Variable* inputNumber, bool showQuestion)
{
	this->inputText = "";
	this->input = true;
	this->storeNumber = inputNumber;
	this->storeText = NULL;
	if(showQuestion == true)
	{
		Print("? ", false);
	}
	this->screen.ShowCursor();
}

void Executer::StoreInput(void)
{
	dms::String getText;
	Get(&getText);
	if(getText == "\x07")
	{
		if(this->inputText.size() == 1)
		{
			this->inputText.clear();
			Print(Chr(4) + " " + Chr(4), false);
		}
		else if(this->inputText.size() > 0)
		{
			this->inputText = this->inputText.substr(0, this->inputText.size() - 1);
			Print(Chr(4) + " " + Chr(4), false);
		}
	}
	else if(getText == "\x0d")
	{
		EndInput();
		this->screen.ReturnText();
	}
	else if((getText[0] >= 16) && (getText[0] <= 25))
	{
		int functionKeyIndex = getText[0] - 16;
		dms::String inputText = this->defKey[functionKeyIndex];
		if(inputText.empty() == false)
		{
			for(size_t i = 0; i < inputText.size(); ++ i)
			{
				dms::String inputCharacter = dms::Format("%c", inputText[i]);
				if(inputCharacter == "\x7F")
				{
					EndInput();
					this->screen.ReturnText();
					break;
				}
				this->inputText += inputCharacter;
				Print(inputCharacter, false);
			}
		}
	}
	else if(getText.empty() == false)
	{
		if(static_cast<unsigned char>(getText[0]) >= 30)
		{
			this->inputText += getText;
			Print(getText, false);
		}
	}
}

void Executer::EndInput(void)
{
	if(this->storeText)
	{
		*this->storeText = this->inputText;
	}
	else if(this->storeNumber)
	{
		char* check;
		dms::Variable result;
		int intValue = static_cast<int>(strtol(this->inputText.c_str(), &check, 10));
		if(*check == '\0')
		{
			result = intValue;
		}
		else
		{
			double doubleValue = strtod(this->inputText.c_str(), &check);
			if (*check != '\0')
			{
				result = doubleValue;
			}
		}
		*this->storeNumber = result;
	}
	this->input = false;
	this->screen.HideCursor();
}

void Executer::Get(dms::String* getText)
{
	*getText = "";
	KeyBoard& keyBoard = KeyBoard::GetInstance();
	unsigned char getCode = keyBoard.Get();
	if(getCode != 0)
	{
		*getText = Chr(getCode);
	}
}

void Executer::Get(dms::Variable* getNumber)
{
	*getNumber = 0;
	dms::String keyGet;
	Get(&keyGet);
	if(keyGet.empty() == false)
	{
		if(isdigit(keyGet[0]) != 0)
		{
			*getNumber = atoi(keyGet.c_str());
		}
	}
}

dms::Variable Executer::Pi(void)
{
	return M_PI;
}

void Executer::Tempo(dms::Variable tempo)
{
	this->beepMusic.SetTempo(tempo.GetInt());
}

void Executer::Music(dms::String mml)
{
	this->beepMusic.MML(mml);
}

void Executer::Poke(dms::Variable address, dms::Variable data)
{
	size_t memoryAddress = static_cast<size_t>(address.GetInt());
	this->mainMemory[memoryAddress] = static_cast<unsigned char>(data.GetInt());
	KeyBoard& keyBoard = KeyBoard::GetInstance();
	if(this->mainMemory[0x952] == 0)
	{
		keyBoard.SetRepeat(true);
	}
	else if(this->mainMemory[0x952] == 166)
	{
		keyBoard.SetRepeat(false);
	}
}

void Executer::Usr(dms::Variable address, dms::String option)
{
	if(address.GetInt() == 0x0F14)
	{
		// Beep
		dms::Variable tone = Peek(0x0F1A);
		this->beepMusic.Beep(tone.GetInt());
	}
}

void Executer::Limit(dms::Variable address)
{
}

void Executer::Out(dms::Variable ioAddress, dms::Variable data)
{
}

void Executer::DebugLog(dms::String text)
{
	if(this->isOutputLog == false)
	{
		return;
	}
	FileData file;
	file.SetBuffer(&text[0], text.size());
	file.SaveAdd("Executer.log");
}

void Executer::Run(void)
{
	this->executeLine = 0;
	this->jumpLine = -1;
	this->end = false;
	Restore(-1);
}

void Executer::Box(dms::Variable x1,dms::Variable y1,dms::Variable x2,dms::Variable y2)
{
	if(x1 > x2)
	{
		dms::Variable temp = x1;
		x1 = x2;
		x2 = temp;
	}
	if(y1 > y2)
	{
		dms::Variable temp = y1;
		y1 = y2;
		y2 = temp;
	}
	int width = x2.GetInt() - x1.GetInt();
	int height = y2.GetInt() - y1.GetInt();
	this->screen.DrawBox(x1.GetInt(), y1.GetInt(), width, height, 0xFFFFFFFF, this->colorMask);
}
