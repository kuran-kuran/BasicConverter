#include <math.h>
#include "Executer.hpp"
#include "Format.hpp"
#include "Command.hpp"

// #pragma execution_character_set("utf-8")

// 10 print "sample";
// 20 a=10
// 30 if a=10 then gosub *func1
// 40 print "a=";a
// 50 gosub *func2
// 60 print "a=";a
// 70 print TI$:goto 10
// 80 *func1
// 90 a=0
// 100 *func2
// 110 a=a+1
// 120 return

#define L10 0
#define L70 6
#define L75 7
#define L80 9
#define L100 11
#define L130 14
#define L180 19

Executer* executer = Executer::GetInstance();
bool end = false;

dms::Variable a;
dms::Variable i;
dms::String s;
dms::String in;

void SetData()
{
	std::vector<Data> data1 = { 10, 11 };
	std::vector<Data> data2 = {"test1", "test2"};
	SetData(L80, data1);
	SetData(L100, data2);
	SetData(0, { 10, 20, 30, 40 });
	SetData(0, { Data("test3"), Data("test4"), Data("test5"), Data("test6"), Data(8), Data(0.5) });
	SetData(0, { "test3", "test4", "test5", "test6" });
	SetData(0, { "00", "00", "00", "30", "68", "E4", "66", "66", "66", "66", "76", "6E", "C4", "40", "00", "00" });
}

void l10()	{Cursor(10,5);Input(&in);}
void l20()	{Cursor(10,6);Print(in, true);a=10;}
void l30()	{Gosub(L80);}
void l40()	{Print(dms::Format("a=%s", a.GetString().c_str()), true);}
void l50()	{Gosub(L130);}
void l60()	{Print(dms::Format("a=%s", a.GetString().c_str()), true);}
void l70()	{Print(Time().c_str(), true);s=Get();Print(dms::Format("s=%s", s.c_str()), true);}
void l75()	{If(s=="1", L70);}
void l77()	{Goto(L10);}
void l80()	{}
void l90()	{a=0;}
void l100()	{}
void l110()	{a=a+1;}
void l120()	{Return();}
void l130() {For(&i, 3, 6, 0.5);};
void l140() {Print(dms::Format("i=%s", i.GetString().c_str()), true);}
void l150() {Next();};
void l160()	{If(a>10, L180);}
void l170()	{a=a+1;}
void l180()	{Return();}

std::vector<void (*)(void)> lineList =
{
l10,
l20,
l30,
l40,
l50,
l60,
l70,
l75,
l77,
l80,
l90,
l100,
l110,
l120,
l130,
l140,
l150,
l160,
l170,
l180,
};

void MainLoop_Setup(unsigned int* frameBuffer)
{
	executer->Initialize(lineList, frameBuffer);
	executer->Clear();
	executer->Cls();
	SetData();
	dms::Variable data1 = ReadVariable();
	dms::Variable data2 = ReadVariable();
	dms::String string1= ReadString();
	dms::String string2 = ReadString();
	GraphInput(1);
	GraphOutput(O_123);
	Print("012345678901234567890123456789012345678", true);
	Print(dms::Format("Data: %s, %s, %s, %s", data1.GetString().c_str(), data2.GetString().c_str(), string1.c_str(), string2.c_str()), true);
	Line({50, 50, 150, 50, 100, 60, 200, 20, 240, 50, 100, 80, 100, 120, 50, 100, 50, 50});
	GraphInput(1);
	GraphOutput(O_123);
	Position(150, 10);
	Pattern(-8, dms::String({255, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 255, 255, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 255}));
	GraphInput(2);
	GraphOutput(O_123);
	Line({Posh(), Posv(), 200, 199});
	dms::String right = Right("abcde12345", 5);
	dms::String left = Left("abcde12345", 5);
	dms::String mid = Mid("abcde12345", 3, 5);
	Print(dms::Format("%s:%s:%s", right.c_str(), left.c_str(), mid.c_str()), true);
	dms::String text65 = Chr(65);
	dms::String text0 = Chr(0) + Chr(0);
	dms::Variable t65 = Asc(text65);
	dms::Variable t0 = Asc(text0);
	dms::String t65t = t65.GetString();
	dms::String t0t = t0.GetString();
	dms::String t65t2 = Asc(text65).GetString();
	dms::String t0t2 = Asc(text0).GetString();
	Print(dms::Format("text65:%s, text0:%s, %s, %s", text65.c_str(), text0.c_str(), Asc(text65).GetString().c_str(), Asc(text0).GetString().c_str()), true);
	dms::String pat({255, 0, 0, 0, 0, 0, 0, 255});
	Position(150, 30);
	Pattern(-8, pat);
	SetTime("235950");
	dms::Variable r = 0.39369908;
	dms::Variable a = Sqr(r);
	dms::Variable b = Sqr(2);
	Print(dms::String("TEST") + Chr(32), true);
	int aa = 0;
}

void MainLoop_Finalize(void)
{
}

void MainLoop_Loop()
{
	if(end == true)
	{
		return;
	}
	end = executer->Execute();
}

void MainLoop_Flip()
{
	executer->Flip();
}
