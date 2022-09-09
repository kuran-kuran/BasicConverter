#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "String.hpp"

class Screen
{
public:
	Screen(void);
	~Screen(void);
	void Initialize(int width, int height, int fontWidth, int fontHeight, unsigned int* frameBuffer);
	void LoadFont(void* buffer, int bufferSize);
	void SetStretchWidth(int stretch);
	void SetStretchHeight(int stretch);
	void SetTextStretchWidth(int stretch);
	void SetTextStretchHeight(int stretch);
	void Clear(unsigned int colorMask = 0xFFFFFFFF);
	void ClearText(bool all = false);
	void Fill(unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
	void DrawPoint(int x, int y, unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
	unsigned int GetPoint(int x, int y);
	void DrawRectangle(int x, int y, int width, int height, unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
	void DrawBox(int x, int y, int width, int height, unsigned int color, unsigned int colorMask = 0xFFFFFFFF, bool fill = false, unsigned int fillColor = 0xFFFFFFFF, unsigned int fillColorMask = 0xFFFFFFFF);
	void DrawLine(int x1, int y1, int x2, int y2, unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
	void DrawLine(std::vector<int>& positionList, unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
	void DrawSprite(const unsigned int* buffer, int x, int y, int width, int height, unsigned int colorMask, bool enableColorKey, unsigned int colorKey);
	void DrawSprite(const unsigned int* buffer, int x, int y, int width, int height, int sourceX, int sourceY, int sourceWidth, unsigned int colorMask, bool enableColorKey, unsigned int colorKey);
	void DrawFont(int x, int y, int charcterNumber, unsigned int color);
	void DrawFont(int x, int y, int charcterNumber, unsigned int color, unsigned int backColor);
	void Print(dms::String text, bool newline);
	void Paint(int x, int y, unsigned int color, std::vector<unsigned int>& borderColorList);
	void Pattern(int row, dms::String& pattern, unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
	void DrawCircle(int x, int y, int r, double h = 0.5, double ks = 0.0, double ke = M_PI * 2.0, int o = 0, unsigned int color = 0xFFFFFFFF, unsigned int colorMask = 0xFFFFFFFF);
	void ScrollXRange(int left, int right);
	void ScrollYRange(int top, int bottom);
	void SetTextX(int x);
	void SetTextY(int y);
	void SetGraphicX(int x);
	void SetGraphicY(int y);
	int GetTextX(void);
	int GetTextY(void);
	int GetGraphicX(void);
	int GetGraphicY(void);
	int GetTextWidth(void);
	int GetTextHeight(void);
	int GetGraphicWidth(void);
	int GetGraphicHeight(void);
	unsigned char GetScreenText(int x, int y);
	unsigned int* GetScreenBuffer(void);
	void Flip(unsigned int textColorMask, unsigned int graphicColorMask, unsigned int backGroundColor, int priority);
	void ReturnText(void);
	void ShowCursor(void);
	void HideCursor(void);
	void DrawPattern(int x, int y, unsigned char pattern, unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
private:
	static const int cursorCountMax;
	void DrawSpriteNoClip(unsigned int* distinationBuffer, const unsigned int* buffer, int x, int y, int width, int height, int sourceWidth, unsigned int colorMask, bool enableColorKey, unsigned int colorKey, bool enableBackColor, unsigned int backColor);
	bool CheckColor(unsigned int color, std::vector<unsigned int> checkColor);
	void PaintProc(int x, int y, unsigned int color, std::vector<unsigned int>& borderColorList);
	void ScrollText(int x, int y, int width, int height, int dir);
	void ReDrawText(void);
	void FillFrameBuffer(unsigned int color);
	void FlipGraphic(unsigned int colorMask);
	void FlipText(unsigned int colorMask);
	void FlipCursor(unsigned int colorMask);
	void DrawCursorFrameBuffer(int x, int y, int charcterNumber, unsigned int color, unsigned int colorMask);
	bool ControlCode(int character);
	// 画面の画像バッファ、サイズはwidth*height*sizeof(insigned int)、カラーフォーマットはARGB
	unsigned int* screenBuffer;
	int screenWidth;
	int screenHeight;
	size_t screenBufferSize;
	unsigned int* textScreenBuffer;
	unsigned char* textBuffer;
	int textWidth;
	int textHeight;
	unsigned int* fontBuffer;
	int fontBufferWidth;
	int fontBufferHeight;
	int fontWidth;
	int fontHeight;
	unsigned int* frameBuffer;
	int textX;
	int textY;
	int graphicX;
	int graphicY;
	int stretchWidth;
	int stretchHeight;
	int textStretchWidth;
	int textStretchHeight;
	bool showCursor;
	int cursorCount;
	int blinkCursor;
	int scrollTop;
	int scrollLeft;
	int scrollBottom;
	int scrollRight;
	bool isTabControll;
};

#endif
