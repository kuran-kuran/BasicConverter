#include <Windows.h>
#include <mmsystem.h>
#include "resource.h"
#include "KeyBoard.hpp"
#include "WinMain.hpp"
#include "../Executer.hpp"
#include "../Format.hpp"
#include "../FileData.hpp"
#include "../Json.hpp"
#include "../Config.hpp"

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Imm32.lib")

void MainLoop_Setup(unsigned int* frameBuffer);
void MainLoop_Finalize(void);
void MainLoop_Reset(void);
void MainLoop_Loop(void);
void MainLoop_NoWaitLoop(void);
void MainLoop_Flip(void);

HINSTANCE instanceHandle;
HWND windowHandle;
DWORD windowStyle;
TIMECAPS timeCaps;

// CreateDIBSection
HDC bitmapHDC;
HBITMAP bitmapHandle;
LPBYTE bitmapInfoBuffer;
LPDWORD bitmap;
LPBITMAPINFO bitmapInfo;
struct Setting
{
	int screenScale;
	int execSpeed;
	int displayColor;
};
Setting setting = {1, 5, DISPLAY_COLOR};

// DIBSectionとScreenバッファのサイズは同じ、ウインドウに転送するときに拡大して大きさを整える
void InitializeCreateDIBSection(HWND windowHandle)
{
	HDC hdc;
	hdc = GetDC(windowHandle);
	bitmapInfoBuffer = reinterpret_cast<LPBYTE>(GlobalAlloc(GPTR, sizeof(BITMAPINFO)));
	bitmapInfo = reinterpret_cast<LPBITMAPINFO>(bitmapInfoBuffer);
	bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo->bmiHeader.biWidth = SCREEN_WIDTH;
	bitmapInfo->bmiHeader.biHeight = SCREEN_HEIGHT;
	bitmapInfo->bmiHeader.biPlanes = 1;
	bitmapInfo->bmiHeader.biBitCount = 32;
	bitmapInfo->bmiHeader.biCompression = BI_RGB;
	bitmapInfo->bmiHeader.biSizeImage = 0;
	bitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	bitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	bitmapInfo->bmiHeader.biClrUsed = 0;
	bitmapInfo->bmiHeader.biClrImportant = 0;
	bitmapHandle = CreateDIBSection(hdc, bitmapInfo, DIB_RGB_COLORS, reinterpret_cast<PVOID*>(&bitmap), NULL, 0);
	bitmapHDC = CreateCompatibleDC(hdc);
	SelectObject(bitmapHDC, bitmapHandle);
}

void FinalizeCreateDIBSection()
{
	GlobalFree(bitmapInfoBuffer);
	DeleteDC(bitmapHDC);
	DeleteObject(bitmapHandle);
}

void DrawCreateDIBSection(HDC hdc)
{
	RECT destRect;
	GetClientRect(windowHandle, &destRect);
	StretchBlt(hdc, destRect.left, destRect.top, destRect.right, destRect.bottom, bitmapHDC, 0, 0, bitmapInfo->bmiHeader.biWidth, bitmapInfo->bmiHeader.biHeight, SRCCOPY);
}

unsigned int* GetFrameBuffer(void)
{
	return reinterpret_cast<unsigned int*>(bitmap);
}

HWND GetWindowHandle(void)
{
	return windowHandle;
}

void SetWindowSize(int screenScale)
{
	int width = DISPLAY_WIDTH * screenScale;
	int height = DISPLAY_HEIGHT * screenScale;
	RECT position;
	RECT rect = {};
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;
	// ウインドウ位置
	GetWindowRect(windowHandle, &position);
	// ウインドウサイズ
	AdjustWindowRect(&rect, windowStyle, TRUE);
	if(rect.left != 0)
	{
		rect.right -= rect.left;
		rect.left = 0;
	}
	if(rect.top != 0)
	{
		rect.bottom -= rect.top;
		rect.top = 0;
	}
	SetWindowPos(windowHandle, HWND_TOP, position.left, position.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
}

int GetSpeedId(int speed)
{
	int id = ID_SPEEDx1;
	static std::vector<int> speedTable = {1, 5, 10, 25, 100, 500, 1000, INT_MAX};
	for(int i = 0; i < static_cast<int>(speedTable.size()); ++ i)
	{
		if(speedTable[i] == speed)
		{
			return i;
		}
	}
	return id;
}

void UncheckSpeedMenu(void)
{
	CheckMenuItem(GetMenu(windowHandle), ID_SPEED_MIN, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx1, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx2, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx5, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx20, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx100, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx200, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(windowHandle), ID_SPEED_MAX, MF_BYCOMMAND | MF_UNCHECKED);
}

// 設定読み込み
void LoadSetting(void)
{
	FileData settingFile;
	if(settingFile.Load("Setting.json") == true)
	{
		std::string jsonText = reinterpret_cast<char*>(settingFile.GetBuffer());
		dms::Json settingJson;
		settingJson.Set(jsonText);
		setting.screenScale = dms::Json::Int(settingJson["screenScale"]);
		setting.execSpeed = dms::Json::Int(settingJson["execSpeed"]);
		setting.displayColor = dms::Json::Int(settingJson["displayColor"]);
	}
}

// 設定書き込み
void SaveSetting(void)
{
	dms::Json settingJson;
	settingJson.SetData("screenScale", setting.screenScale);
	settingJson.SetData("execSpeed", setting.execSpeed);
	settingJson.SetData("displayColor", setting.displayColor);
	std::string jsonText = settingJson.Encode();
	if(jsonText.empty() == false)
	{
		FileData settingFile;
		settingFile.SetBuffer(&jsonText[0], jsonText.size());
		settingFile.Save("Setting.json");
	}
}

LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HIMC himcPrev = 0;
	Executer* executer = Executer::GetInstance();
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(windowHandle);
			break;
		case ID_FILE_RESET:
			MainLoop_Reset();
			break;
		case ID_WINDOWSIZEx1:
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx1, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx2, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx3, MF_BYCOMMAND | MF_UNCHECKED);
			setting.screenScale = 1;
			SetWindowSize(setting.screenScale);
			break;
		case ID_WINDOWSIZEx2:
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx1, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx2, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx3, MF_BYCOMMAND | MF_UNCHECKED);
			setting.screenScale = 2;
			SetWindowSize(setting.screenScale);
			break;
		case ID_WINDOWSIZEx3:
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx1, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx2, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx3, MF_BYCOMMAND | MF_CHECKED);
			setting.screenScale = 3;
			SetWindowSize(setting.screenScale);
			break;
		case ID_SPEED_MIN:
			UncheckSpeedMenu();
			CheckMenuItem(GetMenu(windowHandle), ID_SPEED_MIN, MF_BYCOMMAND | MF_CHECKED);
			setting.execSpeed = 1;
			break;
		case ID_SPEEDx1:
			UncheckSpeedMenu();
			CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx1, MF_BYCOMMAND | MF_CHECKED);
			setting.execSpeed = 5;
			break;
		case ID_SPEEDx2:
			UncheckSpeedMenu();
			CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx2, MF_BYCOMMAND | MF_CHECKED);
			setting.execSpeed = 10;
			break;
		case ID_SPEEDx5:
			UncheckSpeedMenu();
			CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx5, MF_BYCOMMAND | MF_CHECKED);
			setting.execSpeed = 25;
			break;
		case ID_SPEEDx20:
			UncheckSpeedMenu();
			CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx20, MF_BYCOMMAND | MF_CHECKED);
			setting.execSpeed = 100;
			break;
		case ID_SPEEDx100:
			UncheckSpeedMenu();
			CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx100, MF_BYCOMMAND | MF_CHECKED);
			setting.execSpeed = 500;
			break;
		case ID_SPEEDx200:
			UncheckSpeedMenu();
			CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx200, MF_BYCOMMAND | MF_CHECKED);
			setting.execSpeed = 1000;
			break;
		case ID_SPEED_MAX:
			UncheckSpeedMenu();
			CheckMenuItem(GetMenu(windowHandle), ID_SPEED_MAX, MF_BYCOMMAND | MF_CHECKED);
			setting.execSpeed = INT_MAX;
			break;
		case ID_DISPLAYCOLOR_COLOR:
			CheckMenuItem(GetMenu(windowHandle), ID_DISPLAYCOLOR_COLOR, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_DISPLAYCOLOR_GREEN, MF_BYCOMMAND | MF_UNCHECKED);
			setting.displayColor = DISPLAY_COLOR;
			executer->SetGreenDisplay(false);
			break;
		case ID_DISPLAYCOLOR_GREEN:
			CheckMenuItem(GetMenu(windowHandle), ID_DISPLAYCOLOR_COLOR, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(GetMenu(windowHandle), ID_DISPLAYCOLOR_GREEN, MF_BYCOMMAND | MF_CHECKED);
			setting.displayColor = DISPLAY_GREEN;
			executer->SetGreenDisplay(true);
			break;
		default:
			return DefWindowProc(windowHandle, message, wParam, lParam);
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(windowHandle, &ps);
		DrawCreateDIBSection(hdc);
		EndPaint(windowHandle, &ps);
		break;
	}
	case WM_DESTROY:
		SaveSetting();
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		// 日本語入力モードにならないようにする
		if(LOWORD(wParam) != WA_INACTIVE) {
			himcPrev = ImmAssociateContext(windowHandle, 0);
		} else {
			ImmAssociateContext(windowHandle, himcPrev);
		}
		break;
	case WM_KEYDOWN:
		KeyBoard::GetInstance().SetKeyDown(wParam, lParam);
		break;
	case WM_KEYUP:
		KeyBoard::GetInstance().SetKeyUp(wParam, lParam);
		break;
	case WM_SYSKEYDOWN:
		if(wParam == VK_MENU)
		{
			KeyBoard::GetInstance().SetKeyDown(wParam, lParam);
			return TRUE;
		}
		break;
	case WM_SYSKEYUP:
		KeyBoard::GetInstance().SetKeyUp(wParam, lParam);
		break;
	}
	return DefWindowProc(windowHandle, message, wParam, lParam);
}

void InitializeWindow(HINSTANCE instance, int cmd_show, int width, int height, LPCWSTR window_name, LPCWSTR class_name)
{
	WNDCLASSEXW wcex = {};
	int window_width;
	int window_height;
	instanceHandle = instance;
	// ウインドウクラスの定義
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(instance, reinterpret_cast<PCTSTR>(IDI_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINMAIN);
	wcex.lpszClassName = class_name;
	wcex.hIconSm = LoadIcon(wcex.hInstance, reinterpret_cast<PCTSTR>(IDI_ICON));
	RegisterClassEx(&wcex);
	// ウインドウのスタイル設定
	windowStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;
	// ウインドウの大きさを補正する
	RECT window_rect = {0, 0, width, height};
	AdjustWindowRect(&window_rect, windowStyle, TRUE);
	window_width = window_rect.right - window_rect.left;
	window_height = window_rect.bottom - window_rect.top;
	// ウインドウ作成
	windowHandle = CreateWindowW(wcex.lpszClassName, window_name, windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height, nullptr, nullptr, instance, nullptr);
	InitializeCreateDIBSection(windowHandle);
	ShowWindow(windowHandle, cmd_show);
	UpdateWindow(windowHandle);
	CheckMenuItem(GetMenu(windowHandle), ID_WINDOWSIZEx1 + setting.screenScale - 1, MF_BYCOMMAND | MF_CHECKED);
	CheckMenuItem(GetMenu(windowHandle), ID_SPEEDx1 + GetSpeedId(setting.execSpeed) - 1, MF_BYCOMMAND | MF_CHECKED);
	CheckMenuItem(GetMenu(windowHandle), ID_DISPLAYCOLOR_COLOR + setting.displayColor, MF_BYCOMMAND | MF_CHECKED);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	(void)CoInitialize(NULL);
	LoadSetting();
	InitializeWindow(hInstance, nCmdShow, DISPLAY_WIDTH * setting.screenScale, DISPLAY_HEIGHT * setting.screenScale, WINDOW_TITLE, WINDOW_TITLE);
	// タイマ精度設定
	timeGetDevCaps(&timeCaps, sizeof(TIMECAPS));
	timeBeginPeriod(timeCaps.wPeriodMin);
	// メインループ初期化
	MainLoop_Setup(reinterpret_cast<unsigned int*>(bitmap));
	// ディスプレイカラー設定
	Executer* executer = Executer::GetInstance();
	executer->SetGreenDisplay(setting.displayColor == DISPLAY_COLOR ? false : true);
	// メインループ
	DWORD frameTime = 1000 / FPS;
	DWORD beforeTime = timeGetTime();
	MSG msg;
	for(;;)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0))
			{
				break;
			}
			if(!TranslateAccelerator(msg.hwnd, NULL, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			DWORD time = timeGetTime();
			DWORD diffTime = time - beforeTime;
			if(diffTime >= frameTime)
			{
				int execCount = 0;
				for(;;)
				{
					DWORD execTime = timeGetTime();
					DWORD execDiffTime = time - execTime;
					if(execDiffTime >= 15)
					{
						break;
					}
					MainLoop_Loop();
					++ execCount;
					if(execCount >= setting.execSpeed)
					{
						break;
					}
				}
				MainLoop_Flip();
				InvalidateRect(windowHandle, NULL, FALSE);
				UpdateWindow(windowHandle);
				beforeTime = time;
			}
			MainLoop_NoWaitLoop();
		}
	}
	// プログラム終了
	MainLoop_Finalize();
	FinalizeCreateDIBSection();
	timeEndPeriod(timeCaps.wPeriodMin);
	CoUninitialize();
	return static_cast<int>(msg.wParam);
}
