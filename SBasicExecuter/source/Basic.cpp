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
#define L00020_01 3
#define L00020_02 4
#define L00020_03 5
#define L00025_01 6
#define L00025_02 7
#define L00030_01 8
#define L00030_02 9
#define L00040_01 10
#define L00050_01 11
#define L00060_01 12
#define L00070_01 13
#define L00080_01 14
#define L00090_01 15
#define L00100_01 16
#define L00110_01 17
#define L00120_01 18
#define L00120_02 19
#define L00120_03 20
#define L00130_01 21
#define L00150_01 22
#define L00150_02 23
#define L00160_01 24
#define L00160_02 25
#define L00170_01 26
#define L00170_02 27
#define L00180_01 28
#define L00190_01 29
#define L00190_02 30
#define L00200_01 31
#define L00200_02 32
#define L00200_03 33
#define L00200_04 34
#define L00200_05 35
#define L00210_01 36
#define L00210_02 37
#define L00220_01 38
#define L00220_02 39
#define L00220_03 40
#define L00230_01 41
#define L00230_02 42
#define L00240_01 43
#define L00250_01 44
#define L00250_02 45
#define L00250_03 46
#define L00260_01 47
#define L00270_01 48
#define L00270_02 49
#define L00270_03 50
#define L00270_04 51
#define L00280_01 52
#define L00290_01 53
#define L00300_01 54
#define L00300_02 55
#define L00310_01 56
#define L00310_02 57
#define L00310_03 58
#define L00310_04 59
#define L00320_01 60
#define L00330_01 61
#define L00330_02 62
#define L00330_03 63
#define L00330_04 64
#define L00340_01 65
#define L00340_02 66
#define L00340_03 67
#define L00350_01 68
#define L00350_02 69
#define L00350_03 70
#define L00360_01 71
#define L00360_02 72
#define L00360_03 73
#define L00370_01 74
#define L00370_02 75
#define L00370_03 76
#define L00380_01 77
#define L00380_02 78
#define L00390_01 79
#define L00390_02 80
#define L00400_01 81
#define L00400_02 82
#define L00410_01 83
#define L00410_02 84
#define L00420_01 85
#define L00420_02 86
#define L00430_01 87
#define L00430_02 88
#define L00440_01 89
#define L00440_02 90
#define L00450_01 91
#define L00450_02 92
#define L00460_01 93
#define L00470_01 94
#define L00470_02 95
#define L00470_03 96
#define L00480_01 97
#define L00480_02 98
#define L00480_03 99
#define L00480_04 100
#define L00490_01 101
// Data line define

// Grobal
Executer* executer = Executer::GetInstance();
bool end = false;
dms::String strS;
dms::Variable DY;
dms::Variable DX;
dms::Variable F;
dms::String strTI;
dms::Variable BH;
dms::Variable BV;
dms::Variable ST;
dms::Variable Y;
dms::Variable H1;
dms::Variable V1;
dms::Variable X;
dms::Variable Z;
dms::Variable C;
dms::Variable H;
dms::Variable V;
dms::Variable CH;
dms::Array<dms::Array<dms::Variable, 2+1>, 250+1> arrayMA;
dms::Array<dms::Array<dms::Variable, 2+1>, 250+1> arrayMI;

// Initialize variable
void Clr(void)
{
	strS.clear();
	DY.clear();
	DX.clear();
	F.clear();
	strTI.clear();
	BH.clear();
	BV.clear();
	ST.clear();
	Y.clear();
	H1.clear();
	V1.clear();
	X.clear();
	Z.clear();
	C.clear();
	H.clear();
	V.clear();
	CH.clear();
	arrayMA.clear();
	arrayMI.clear();
}

// Data
void SetupData(void)
{
}

// Program
void l00010_01() { SetTextStretchWidth(2_n); }
void l00010_02() { SetStretchWidth(1_n); }
void l00010_03() { ; }
void l00020_01() { Print(Chr(6_n), true); }
void l00020_02() { CColor(0_n, 4_n, 0_n); }
void l00020_03() { Color(0_n, 7_n, 7_n, 0_n); }
void l00025_01() { Box(0_n,0_n,639_n,199_n,2_n); }
void l00025_02() { Box(1_n,1_n,638_n,198_n,2_n); }
void l00030_01() { Cursor(1_n,7_n); }
void l00030_02() { Print("\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"_s, true); }
void l00040_01() { Print(Tab(2_n)+" ºÉÌßÛ¸Ş×ÑÊ ¼­³Ø®³½ÙÉÆ Ô¸ 36Ìßİ ¦"_s, true); }
void l00050_01() { Print(Tab(2_n)+"Ö³¼Ï½¡ ¼Ş¶İÉÖÕ³É±ÙÄ·Æ ¼Ş¯º³¼Ã¸ÀŞ»²¡"_s, true); }
void l00060_01() { Print(Tab(2_n)+" ÃŞÊºÚ¶× ±ÅÀ¦ 3¼Ş¹Şİ ¶×- ¸Ş×Ì¨¯¸ É"_s, true); }
void l00070_01() { Print(Tab(2_n)+"ĞØ®¸Ã·Å ¾¶²Í ºŞ¼®³À² ²À¼Ï¼®³ !"_s, true); }
void l00080_01() { Print(""_s, true); }
void l00090_01() { Print(Tab(3_n)+"\x86\x86 ºÚÖØ ÌßÛ¸Ş×Ñ ¦ ¼Ş¯º³ ¼Ï½¡ \x86\x86"_s, true); }
void l00100_01() { Print(Tab(5_n)+"ÄŞÚ¶ µ½·Å Key ¦ µ¼Ã¸ÀŞ»²¡"_s, true); }
void l00110_01() { Print(Tab(1_n)+"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"_s, true); }
void l00120_01() { Get(&strS); }
void l00120_02() { If(strS==""_s, L00120_03, L00130_01); }
void l00120_03() { Goto(L00120_01); }
void l00130_01() { Print(Chr(6_n), true); }
void l00150_01() { CColor(0_n, 1_n); }
void l00150_02() { Print("       *** 3D-COLOR-GRAPHIC ***"_s, true); }
void l00160_01() { Cursor(32_n,23_n); }
void l00160_02() { Print("MZ-2000"_s, true); }
void l00170_01() { Cursor(13_n,12_n); }
void l00170_02() { Print("Wait a moment !"_s, true); }
void l00180_01() { ; }
void l00190_01() { For(&DY, 0_n, 2_n, 1); }
void l00190_02() { For(&DX, 0_n, 250_n, 1); }
void l00200_01() { arrayMA[DX][DY]=199_n; }
void l00200_02() { arrayMI[DX][DY]=0_n; }
void l00200_03() { Next(&DX); }
void l00200_04() { Next(&DY); }
void l00200_05() { ; }
void l00210_01() { Cursor(13_n,12_n); }
void l00210_02() { Print("Just start now !"_s, true); }
void l00220_01() { For(&F, 0_n, 2500_n, 1); }
void l00220_02() { Next(&F); }
void l00220_03() { ; }
void l00230_01() { Cursor(13_n,12_n); }
void l00230_02() { Print("                "_s, true); }
void l00240_01() { strTI="000000"_s; }
void l00250_01() { BH=270_n; }
void l00250_02() { BV=173_n; }
void l00250_03() { ST=12_n; }
void l00260_01() { For(&Y, -100_n, 500_n, 10_n); }
void l00270_01() { BH=BH-3_n; }
void l00270_02() { BV=BV-1_n; }
void l00270_03() { H1=BH; }
void l00270_04() { V1=BV; }
void l00280_01() { For(&X, -300_n, 300_n, 2_n); }
void l00290_01() { H1=H1+1_n; }
void l00300_01() { If(X/ST==Int(X/ST), L00300_02, L00310_01); }
void l00300_02() { V1=V1-1_n; }
void l00310_01() { If((X > -100_n)*(X < 100_n)*(Y < 100_n), L00310_02, L00320_01); }
void l00310_02() { Z=0_n; }
void l00310_03() { C=1_n; }
void l00310_04() { Goto(L00400_01); }
void l00320_01() { Z=(Cos(X/100_n*Pi())+1_n)*(Cos(Y/100_n*Pi())+1_n)*15_n; }
void l00330_01() { C=1_n; }
void l00330_02() { If(Int(Z) > 53_n, L00330_03, L00340_01); }
void l00330_03() { C=2_n; }
void l00330_04() { Goto(L00390_01); }
void l00340_01() { If(Int(Z) > 45_n, L00340_02, L00350_01); }
void l00340_02() { C=3_n; }
void l00340_03() { Goto(L00390_01); }
void l00350_01() { If(Int(Z) > 36_n, L00350_02, L00360_01); }
void l00350_02() { C=7_n; }
void l00350_03() { Goto(L00390_01); }
void l00360_01() { If(Int(Z) > 26_n, L00360_02, L00370_01); }
void l00360_02() { C=5_n; }
void l00360_03() { Goto(L00390_01); }
void l00370_01() { If(Int(Z) > 14_n, L00370_02, L00380_01); }
void l00370_02() { C=6_n; }
void l00370_03() { Goto(L00390_01); }
void l00380_01() { If(Int(Z), L00380_02, L00390_01); }
void l00380_02() { C=4_n; }
void l00390_01() { If((Y > 100_n)*(Y < 300_n)*((X < -100_n)+(X > 100_n)), L00390_02, L00400_01); }
void l00390_02() { Z=-Z; }
void l00400_01() { H=H1; }
void l00400_02() { V=V1-Z; }
void l00410_01() { DX=H-Int(H/251_n)*251_n; }
void l00410_02() { DY=Int(H/251_n); }
void l00420_01() { If((V <= arrayMI[DX][DY])*(V >= arrayMA[DX][DY]), L00420_02, L00430_01); }
void l00420_02() { Goto(L00470_01); }
void l00430_01() { If(V > arrayMI[DX][DY], L00430_02, L00440_01); }
void l00430_02() { arrayMI[DX][DY]=V; }
void l00440_01() { If(V < arrayMA[DX][DY], L00440_02, L00450_01); }
void l00440_02() { arrayMA[DX][DY]=V; }
void l00450_01() { If(V < 0_n, L00450_02, L00460_01); }
void l00450_02() { Goto(L00470_01); }
void l00460_01() { Set(H,V,C); }
void l00470_01() { Next(&X); }
void l00470_02() { Next(&Y); }
void l00470_03() { ; }
void l00480_01() { Cursor(2_n,18_n); }
void l00480_02() { Print("<TIME>"_s, true); }
void l00480_03() { Print(Tab(2_n)+Mid(strTI,3_n,2_n)+" min"_s, true); }
void l00480_04() { Print(Tab(2_n)+Right(strTI,2_n)+" sec"_s, true); }
void l00490_01() { End(); }

std::vector<void (*)(void)> lineList =
{
	l00010_01,
	l00010_02,
	l00010_03,
	l00020_01,
	l00020_02,
	l00020_03,
	l00025_01,
	l00025_02,
	l00030_01,
	l00030_02,
	l00040_01,
	l00050_01,
	l00060_01,
	l00070_01,
	l00080_01,
	l00090_01,
	l00100_01,
	l00110_01,
	l00120_01,
	l00120_02,
	l00120_03,
	l00130_01,
	l00150_01,
	l00150_02,
	l00160_01,
	l00160_02,
	l00170_01,
	l00170_02,
	l00180_01,
	l00190_01,
	l00190_02,
	l00200_01,
	l00200_02,
	l00200_03,
	l00200_04,
	l00200_05,
	l00210_01,
	l00210_02,
	l00220_01,
	l00220_02,
	l00220_03,
	l00230_01,
	l00230_02,
	l00240_01,
	l00250_01,
	l00250_02,
	l00250_03,
	l00260_01,
	l00270_01,
	l00270_02,
	l00270_03,
	l00270_04,
	l00280_01,
	l00290_01,
	l00300_01,
	l00300_02,
	l00310_01,
	l00310_02,
	l00310_03,
	l00310_04,
	l00320_01,
	l00330_01,
	l00330_02,
	l00330_03,
	l00330_04,
	l00340_01,
	l00340_02,
	l00340_03,
	l00350_01,
	l00350_02,
	l00350_03,
	l00360_01,
	l00360_02,
	l00360_03,
	l00370_01,
	l00370_02,
	l00370_03,
	l00380_01,
	l00380_02,
	l00390_01,
	l00390_02,
	l00400_01,
	l00400_02,
	l00410_01,
	l00410_02,
	l00420_01,
	l00420_02,
	l00430_01,
	l00430_02,
	l00440_01,
	l00440_02,
	l00450_01,
	l00450_02,
	l00460_01,
	l00470_01,
	l00470_02,
	l00470_03,
	l00480_01,
	l00480_02,
	l00480_03,
	l00480_04,
	l00490_01,
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
