#include "Command.hpp"

// User defined literals
dms::String operator "" _s(const char* str, std::size_t length)
{
	return dms::String(str);
}

dms::Variable operator "" _n(unsigned long long x)
{
	return static_cast<int>(x);
}

dms::Variable operator "" _f( long double value )
{
	return static_cast<double>(value);
}

// Program line define
#define L00010_01 0
#define L00010_02 1
#define L00010_03 2
#define L00010_04 3
#define L00020_01 4
#define L00030_01 5
#define L00040_01 6
#define L00050_01 7
// Data line define

// Grobal
Executer* executer = Executer::GetInstance();
bool end = false;
dms::String strTI;

// Initialize variable
void Clr(void)
{
}

// Data
void SetupData(void)
{
}

// Program
void l00010_01() { GraphInput(1); }
void l00010_02() { GraphOutput(O_1); }
void l00010_03() { ClearGraph(); }
void l00010_04() { Print(Chr(6_n), false); }
void l00020_01() { Cursor(14_n,12_n); }
void l00030_01() { Print("HELLO WORLD"_s, true); }
void l00040_01() { Line({111_n,94_n,201_n,94_n,201_n,104_n,111_n,104_n,111_n,94_n}); }
void l00050_01() { End(); }

std::vector<void (*)(void)> lineList =
{
	l00010_01,
	l00010_02,
	l00010_03,
	l00010_04,
	l00020_01,
	l00030_01,
	l00040_01,
	l00050_01,
};


void SetTimeCallback(dms::String* string)
{
	SetTime(*string);
}

void MainLoop_Setup(unsigned int* frameBuffer)
{
	executer->Initialize(lineList, frameBuffer);
	SetupData();
	strTI.SetStoreCallback(SetTimeCallback);
}

void MainLoop_Finalize()
{
}

void MainLoop_Loop()
{
	if(end == true)
	{
		return;
	}
	end = executer->ExecuteLoop();
}

void MainLoop_NoWaitLoop()
{
	executer->ExecuteNoWaitLoop();
}

void MainLoop_Flip()
{
	strTI.SetStoreCallback(NULL);
	strTI = Time();
	strTI.SetStoreCallback(SetTimeCallback);
	executer->Flip();
}
