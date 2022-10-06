#include "Command.hpp"
#include "Basic.hpp"

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
void l00010_01() { GraphInput(1_n); }
void l00010_02() { GraphOutput(O_1); }
void l00010_03() { ClearGraph(); }
void l00010_04() { ; }
void l00010_05() { Print(Chr(6_n), false); }
void l00020_01() { Cursor(14_n,12_n); }
void l00030_01() { Print("HELLO WORLD"_s, true); }
void l00040_01() { Line({111_n,94_n,201_n,94_n,201_n,104_n,111_n,104_n,111_n,94_n}); }
void l00050_01() { End(); }

std::vector<void (*)(void)> lineList =
{
	l00010_01,	l00010_02,	l00010_03,	l00010_04,	l00010_05,	l00020_01,	l00030_01,	l00040_01,	l00050_01,
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
