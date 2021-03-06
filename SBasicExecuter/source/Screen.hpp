#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <vector>
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
	void ClearText(void);
	void Fill(unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
	void DrawPoint(int x, int y, unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
	unsigned int GetPoint(int x, int y);
	void DrawRectangle(int x, int y, int width, int height, unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
	void DrawLine(int x1, int y1, int x2, int y2, unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
	void DrawLine(std::vector<int>& positionList, unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
	void DrawSprite(const unsigned int* buffer, int x, int y, int width, int height, bool enableColorKey, unsigned int colorKey);
	void DrawSprite(const unsigned int* buffer, int x, int y, int width, int height, int sourceX, int sourceY, int sourceWidth, bool enableColorKey, unsigned int colorKey);
	void DrawFont(int x, int y, int charcterNumber, unsigned int color);
	void DrawFont(int x, int y, int charcterNumber, unsigned int color, unsigned int backColor);
	void Print(dms::String text, bool newline);
	void Paint(int x, int y, unsigned int color, std::vector<unsigned int>& borderColorList);
	void Pattern(int row, dms::String& pattern, unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
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
	unsigned char GetScreenText(int x, int y);
	unsigned int* GetScreenBuffer(void);
	void Flip(unsigned int textColorMask, unsigned int graphicColorMask);
	void ReturnText(void);
	void ShowCursor(void);
	void HideCursor(void);
private:
	static const int cursorCountMax;
	void DrawSpriteNoClip(unsigned int* distinationBuffer, const unsigned int* buffer, int x, int y, int width, int height, int source_width, bool enableColorKey, unsigned int colorKey, bool enableBackColor, unsigned int backColor);
	bool CheckColor(unsigned int color, std::vector<unsigned int> checkColor);
	void PaintProc(int x, int y, unsigned int color, std::vector<unsigned int>& borderColorList);
	void ScrollText(int x, int y, int width, int height, int dir);
	void ReDrawText(void);
	void DrawPattern(int x, int y, unsigned char pattern, unsigned int color, unsigned int colorMask = 0xFFFFFFFF);
	void FlipGraphic(unsigned int colorMask);
	void FlipText(unsigned int colorMask);
	void FlipCursor(unsigned int colorMask);
	void DrawCursorFrameBuffer(int x, int y, int charcterNumber, unsigned int color, unsigned int colorMask);
	bool ControlCode(int character);
	// ???????????o?b?t?@?A?T?C?Y??width*height*sizeof(insigned int)?A?J???[?t?H?[?}?b?g??ARGB
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
	bool highPriorityText;
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
};

#endif
