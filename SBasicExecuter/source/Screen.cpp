#include <stdio.h>
#include <stdlib.h>
#include "Screen.hpp"
#include "Png.hpp"
#include "Line.hpp"
#include "Format.hpp"
#include "String.hpp"
#include "Dependent.hpp"
#include "Config.hpp"

const int Screen::cursorCountMax = 18;

Screen::Screen(void)
:screenBuffer(NULL)
,screenWidth(0)
,screenHeight(0)
,screenBufferSize(0)
,textScreenBuffer(NULL)
,textBuffer(NULL)
,textWidth(0)
,textHeight(0)
,fontBuffer(NULL)
,fontBufferWidth(0)
,fontBufferHeight(0)
,fontWidth(0)
,fontHeight(0)
,frameBuffer(NULL)
,textX(0)
,textY(0)
,graphicX(0)
,graphicY(0)
,stretchWidth(2)
,stretchHeight(1)
,textStretchWidth(2)
,textStretchHeight(1)
,showCursor(false)
,cursorCount(0)
,blinkCursor(0)
,scrollTop(0)
,scrollLeft(0)
,scrollRight(79)
,scrollBottom(24)
{
}

Screen::~Screen(void)
{
	if (this->fontBuffer != NULL)
	{
		free(this->fontBuffer);
		this->fontBuffer = NULL;
	}
	if(this->textBuffer != NULL)
	{
		free(this->textBuffer);
		this->textBuffer = NULL;
	}
	if(this->textScreenBuffer != NULL)
	{
		free(this->textScreenBuffer);
		this->textScreenBuffer = NULL;
	}
	if(this->screenBuffer != NULL)
	{
		free(this->screenBuffer);
		this->screenBuffer = NULL;
	}
}

void Screen::Initialize(int width, int height, int fontWidth, int fontHeight, unsigned int* frameBuffer)
{
	this->screenWidth = width;
	this->screenHeight = height;
	this->fontWidth = fontWidth;
	this->fontHeight = fontHeight;
	this->textWidth = width / fontWidth;
	this->textHeight = height / fontHeight;
	this->scrollRight = this->textWidth - 1;
	this->scrollBottom = this->textHeight - 1;
	this->screenBufferSize = static_cast<size_t>(width) * static_cast<size_t>(height) * sizeof(unsigned int);
	this->screenBuffer = reinterpret_cast<unsigned int*>(malloc(screenBufferSize));
	memset(this->screenBuffer, 0, screenBufferSize);
	this->textScreenBuffer = reinterpret_cast<unsigned int*>(malloc(screenBufferSize));
	memset(this->textScreenBuffer, 0, screenBufferSize);
	size_t textBufferSize = static_cast<size_t>(textWidth) * textHeight;
	this->textBuffer = reinterpret_cast<unsigned char*>(malloc(textBufferSize));
	memset(this->textBuffer, 0, textBufferSize);
	this->frameBuffer = frameBuffer;
}

void Screen::LoadFont(void* buffer, int bufferSize)
{
	dms::Png png;
	png.Load(buffer, bufferSize);
	this->fontBufferWidth = png.GetWidth();
	this->fontBufferHeight = png.GetHeight();
	size_t fontBufferSize = sizeof(unsigned int) * png.GetWidth() * png.GetHeight();
	this->fontBuffer = reinterpret_cast<unsigned int*>(malloc(fontBufferSize));
	memcpy(this->fontBuffer, png.GetBuffer(), fontBufferSize);
}

void Screen::SetStretchWidth(int stretch)
{
	this->stretchWidth = stretch;
}

void Screen::SetStretchHeight(int stretch)
{
	this->stretchHeight = stretch;
}

void Screen::SetTextStretchWidth(int stretch)
{
	this->textStretchWidth = stretch;
	this->textWidth = 80 / stretch;
	this->scrollRight = this->textWidth - 1;
}

void Screen::SetTextStretchHeight(int stretch)
{
	this->textStretchHeight = stretch;
	this->textHeight = 25 / stretch;
	this->scrollBottom = this->textHeight - 1;
}

void Screen::Clear(unsigned int colorMask)
{
	DrawRectangle(0, 0, this->screenWidth, this->screenHeight, 0, colorMask);
}

void Screen::ClearText(bool all)
{
	memset(this->textScreenBuffer, 0, sizeof(unsigned int) * screenWidth * screenHeight);
	int width;
	int height;
	int top;
	int left;
	if(all == true)
	{
		width = this->textWidth;
		height = this->textHeight;
		top = 0;
		left = 0;
	}
	else
	{
		width = this->scrollRight - this->scrollLeft;
		height = this->scrollBottom - this->scrollTop;
		top = this->scrollTop;
		left = this->scrollLeft;
	}
	for(int y = 0; y <= height; ++ y)
	{
		for(int x = 0; x <= width; ++ x)
		{
			int address = (top + y) * textWidth + left + x;
			this->textBuffer[address] = 0;
		}
	}
	ReDrawText();
	this->textX = this->scrollLeft;
	this->textY = this->scrollTop;
}

void Screen::Fill(unsigned int color, unsigned int colorMask)
{
	DrawRectangle(0, 0, this->screenWidth, this->screenHeight, color, colorMask);
}

void Screen::DrawPoint(int x, int y, unsigned int color, unsigned int colorMask)
{
	this->graphicX = x;
	this->graphicY = y;
	int destinationAddress = this->screenWidth * y + x;
	unsigned int writeColor = colorMask == 0xFFFFFFFF ? color : (this->screenBuffer[destinationAddress] & ~colorMask) | (color & colorMask);
	this->screenBuffer[destinationAddress] = writeColor;
}

unsigned int Screen::GetPoint(int x, int y)
{
	return this->screenBuffer[this->screenWidth * y + x];
}

void Screen::DrawRectangle(int x, int y, int width, int height, unsigned int color, unsigned int colorMask)
{
	int destinationAddress = this->screenWidth * y + x;
	for (int scanY = 0; scanY < height; ++ scanY)
	{
		for (int scanX = 0; scanX < width; ++ scanX)
		{
			unsigned int writeColor = colorMask == 0xFFFFFFFF ? color : (this->screenBuffer[destinationAddress + scanX] & ~colorMask) | (color & colorMask);
			this->screenBuffer[destinationAddress + scanX] = writeColor;
		}
		destinationAddress += this->screenWidth;
	}
}

void Screen::DrawBox(int x, int y, int width, int height, unsigned int color, unsigned int colorMask, bool fill, unsigned int fillColor, unsigned int fillColorMask)
{
	if(fill == true)
	{
		for(int yy = 0; yy < height; ++ yy)
		{
			for(int xx = 0; xx < width; ++ xx)
			{
				DrawPoint(x + xx, y + yy, fillColor, fillColorMask);
			}
		}
	}
	std::vector<int> positionList =
	{
		x, y,
		x + width, y,
		x + width, y + height,
		x, y + height,
		x, y
	};
	DrawLine(positionList, color, colorMask);
}

void Screen::DrawLine(int x0, int y0, int x1, int y1, unsigned int color, unsigned int colorMask)
{
	dms::Line line;
	line.Set(x0, y0, x1, y1);
	int x = x0;
	int y = y0;
	DrawPoint(x, y, color, colorMask);
	while(line.Get(x, y) == false)
	{
		DrawPoint(x, y, color, colorMask);
	}
}

void Screen::DrawLine(std::vector<int>& positionList, unsigned int color, unsigned int colorMask)
{
	size_t positionCount = positionList.size() / 2;
	if(positionCount < 2)
	{
		return;
	}
	int x0 = positionList[0];
	int y0 = positionList[1];
	for(size_t i = 1; i < positionCount; ++ i)
	{
		size_t index = i * 2;
		int x1 = positionList[index];
		int y1 = positionList[index + 1];
		DrawLine(x0, y0, x1, y1, color, colorMask);
		x0 = x1;
		y0 = y1;
	}
}

void Screen::DrawSprite(const unsigned int* buffer, int x, int y, int width, int height, unsigned int colorMask, bool enableColorKey, unsigned int colorKey)
{
	DrawSprite(buffer, x, y, width, height, 0, 0, width, colorMask, enableColorKey, colorKey);
}

void Screen::DrawSprite(const unsigned int* buffer, int x, int y, int width, int height, int sourceX, int sourceY, int sourceWidth, unsigned int colorMask, bool enableColorKey, unsigned int colorKey)
{
	if (x + width <= 0)
	{
		return;
	}
	if (y + height <= 0)
	{
		return;
	}
	if (x >= this->screenWidth)
	{
		return;
	}
	if (y >= this->screenHeight)
	{
		return;
	}
	int clip_width = width;
	int clip_height = height;
	const unsigned int* clip_buffer = buffer + static_cast<size_t>(sourceY) * static_cast<size_t>(sourceWidth) + static_cast<size_t>(sourceX);
	if (x > this->screenWidth - width)
	{
		clip_width = width - (x - (this->screenWidth - width));
	}
	if (x < 0)
	{
		clip_buffer -= x;
		clip_width += x;
		x = 0;
	}
	if (y > this->screenHeight - height)
	{
		clip_height = height - (y - (this->screenHeight - height));
	}
	if (y < 0)
	{
		clip_buffer -= (static_cast<size_t>(y) * static_cast<size_t>(sourceWidth));
		clip_height += y;
		y = 0;
	}
	DrawSpriteNoClip(this->screenBuffer, clip_buffer, x, y, clip_width, clip_height, sourceWidth, colorMask, enableColorKey, colorKey, false, 0);
}

void Screen::DrawFont(int x, int y, int charcterNumber, unsigned int color)
{
	if(this->fontBuffer == NULL)
	{
		return;
	}
	size_t address = static_cast<size_t>(this->textWidth) * y + x;
	this->textBuffer[address] = charcterNumber;
	int fontX = (charcterNumber / 16) * this->fontHeight;
	int fontY = (charcterNumber % 16) * this->fontWidth;
	unsigned int* fontBuffer = &(this->fontBuffer[fontY * this->fontBufferWidth + fontX]);
	DrawSpriteNoClip(this->textScreenBuffer, fontBuffer, x * 8, y * 8, this->fontWidth, this->fontHeight, this->fontBufferWidth, color, true, 0, true, 0x00000000);
}

void Screen::DrawFont(int x, int y, int charcterNumber, unsigned int color, unsigned int backColor)
{
	if(this->fontBuffer == NULL)
	{
		return;
	}
	size_t address = static_cast<size_t>(this->textWidth) * y + x;
	this->textBuffer[address] = charcterNumber;
	int fontX = (charcterNumber / 16) * this->fontHeight;
	int fontY = (charcterNumber % 16) * this->fontWidth;
	unsigned int* fontBuffer = &(this->fontBuffer[fontY * this->fontBufferWidth + fontX]);
	DrawSpriteNoClip(this->textScreenBuffer, fontBuffer, x * 8, y * 8, this->fontWidth, this->fontHeight, this->fontBufferWidth, true, 0, color, true, backColor);
}

void Screen::Print(dms::String text, bool newline)
{
	for(size_t i = 0; i < text.size(); ++ i)
	{
		int character = static_cast<unsigned char>(text[i]);
		if(ControlCode(character) == true)
		{
			continue;
		}
		DrawFont(this->textX, this->textY, character, 0xFFFFFFFF);
		++ this->textX;
		if(this->textX > this->scrollRight)
		{
			ReturnText();
		}
	}
	if(newline == true)
	{
		ReturnText();
	}
}

void Screen::Paint(int x, int y, unsigned int color, std::vector<unsigned int>& borderColorList)
{
	borderColorList.push_back(color);
	PaintProc(x, y, color, borderColorList);
}

void Screen::Pattern(int row, dms::String& pattern, unsigned int color, unsigned int colorMask)
{
	int vy = row < 0 ? 1 : -1;
	int absRow = row < 0 ? -row : row;
	int x = this->graphicX;
	int y = this->graphicY;
	int rowCount = 0;
	for(int i = 0; i < static_cast<int>(pattern.size()); ++i)
	{
		DrawPattern(x, y, pattern[i], color, colorMask);
		y += vy;
		++ rowCount;
		if(rowCount >= absRow)
		{
			y = this->graphicY;
			x += 8;
			rowCount = 0;
		}
	}
	this->graphicX = x;
	this->graphicY = y;
}

void Screen::DrawCircle(int x, int y, int r, double h, double ks, double ke, int o, unsigned int color, unsigned int colorMask)
{
	double start = ks;
	double end = -ke;
	double radius = static_cast<double>(r);
	double xStart = cos(start) * radius;
	double yStart = sin(start) * radius;
	if(h < 1.0)
	{
		yStart *= h;
	}
	else if(h > 1.0)
	{
		xStart /= h;
	}
	xStart += static_cast<double>(x);
	yStart += static_cast<double>(y);
	double xBefore = xStart;
	double yBefore = yStart;
	double step = M_PI * 2.0 / 360.0;
	start -= step;
	for(double i = start; i > end; i -= step)
	{
		double nextX = cos(i) * radius;
		double nextY = sin(i) * radius;
		if(h < 1.0)
		{
			nextY *= h;
		}
		else if(h > 1.0)
		{
			nextX /= h;
		}
		nextX += static_cast<double>(x);
		nextY += static_cast<double>(y);
		DrawLine(static_cast<int>(xBefore), static_cast<int>(yBefore), static_cast<int>(nextX), static_cast<int>(nextY), color, colorMask);
		xBefore = nextX;
		yBefore = nextY;
	}
	if(o != 0)
	{
		DrawLine(static_cast<int>(xStart), static_cast<int>(yStart), static_cast<int>(x), static_cast<int>(y), color, colorMask);
		DrawLine(static_cast<int>(xBefore), static_cast<int>(yBefore), static_cast<int>(x), static_cast<int>(y), color, colorMask);
	}
}

void Screen::ScrollXRange(int left, int right)
{
	this->scrollLeft = left;
	this->scrollRight = right;
	this->textX = this->scrollLeft;
	this->textY = this->scrollTop;
}

void Screen::ScrollYRange(int top, int bottom)
{
	this->scrollTop = top;
	this->scrollBottom = bottom;
	this->textX = this->scrollLeft;
	this->textY = this->scrollTop;
}

void Screen::SetTextX(int x)
{
	this->textX = x;
}

void Screen::SetTextY(int y)
{
	this->textY = y;
}

void Screen::SetGraphicX(int x)
{
	this->graphicX = x;
}

void Screen::SetGraphicY(int y)
{
	this->graphicY = y;
}

int Screen::GetTextX(void)
{
	return this->textX;
}

int Screen::GetTextY(void)
{
	return this->textY;
}

int Screen::GetGraphicY(void)
{
	return this->graphicY;
}

int Screen::GetGraphicX(void)
{
	return this->graphicX;
}

unsigned int* Screen::GetScreenBuffer(void)
{
	return this->screenBuffer;
}

unsigned char Screen::GetScreenText(int x, int y)
{
	return this->textBuffer[y * this->textWidth + x];
}

int Screen::GetTextWidth(void)
{
	return this->textWidth;
}

int Screen::GetTextHeight(void)
{
	return this->textHeight;
}

int Screen::GetGraphicWidth(void)
{
	return this->screenWidth;
}

int Screen::GetGraphicHeight(void)
{
	return this->screenHeight;
}

void Screen::Flip(unsigned int textColorMask, unsigned int graphicColorMask, unsigned int backGroundColor, int priority)
{
	FillFrameBuffer(backGroundColor);
	if(priority == 0)
	{
		FlipGraphic(graphicColorMask);
		FlipText(textColorMask);
		FlipCursor(textColorMask);
	}
	else
	{
		FlipText(textColorMask);
		FlipCursor(textColorMask);
		FlipGraphic(graphicColorMask);
	}
}
void Screen::DrawSpriteNoClip(unsigned int* distinationBuffer, const unsigned int* buffer, int x, int y, int width, int height, int sourceWidth, unsigned int colorMask, bool enableColorKey, unsigned int colorKey, bool enableBackColor, unsigned int backColor)
{
	int sourceAddress = 0;
	int destinationAddress = this->screenWidth * y + x;
	for(int scan_y = 0; scan_y < height; ++ scan_y)
	{
		for(int scan_x = 0; scan_x < width; ++ scan_x)
		{
			unsigned int color = buffer[sourceAddress + scan_x];
			if(enableColorKey == false || ((enableColorKey == true) && (color != colorKey)))
			{
				distinationBuffer[destinationAddress + scan_x] = color & colorMask;
			}
			else if (enableBackColor == true)
			{
				distinationBuffer[destinationAddress + scan_x] = backColor;
			}
		}
		sourceAddress += sourceWidth;
		destinationAddress += this->screenWidth;
	}
}

void Screen::ReturnText(void)
{
	this->textX = 0;
	++ this->textY;
	if(this->textY > this->scrollBottom)
	{
		// todo 1行スクロールする
		int width = this->scrollRight - this->scrollLeft + 1;
		int height = this->scrollBottom - this->scrollTop + 1;
		ScrollText(this->scrollLeft, this->scrollTop, width, height, 1);
		this->textY = this->scrollBottom;
	}
}

// colorがcheckColorに含まれるか
bool Screen::CheckColor(unsigned int color, std::vector<unsigned int> checkColor)
{
	if(checkColor.empty() == true)
	{
		return false;
	}
	for(size_t i = 0; i < checkColor.size(); ++ i)
	{
		if((checkColor[i] & 0xFFFFFF) == (color & 0xFFFFFF))
		{
			return true;
		}
	}
	return false;
}

void Screen::PaintProc(int x, int y, unsigned int color, std::vector<unsigned int>& borderColorList)
{
	DrawPoint(x, y, color);
	if(x < (this->screenWidth - 1) && CheckColor(GetPoint(x + 1, y), borderColorList) == false)
	{
		PaintProc(x + 1, y, color, borderColorList);
	}
	if(x > 0 && CheckColor(GetPoint(x - 1, y), borderColorList) == false)
	{
		PaintProc(x - 1, y, color, borderColorList);
	}
	if(y < (this->screenHeight - 1) && CheckColor(GetPoint(x, y + 1), borderColorList) == false)
	{
		PaintProc(x, y + 1, color, borderColorList);
	}
	if(y > 0 && CheckColor(GetPoint(x, y - 1), borderColorList) == false)
	{
		PaintProc(x, y - 1, color, borderColorList);
	}
}

// dir 1:上, 2:左, 3:下, 4:右
void Screen::ScrollText(int x, int y, int width, int height, int dir)
{
	int destinationAddress;
	switch(dir)
	{
	case 1: // 上方向
		for(int scrollY = 1; scrollY < height; ++ scrollY)
		{
			for(int scrollX = 0; scrollX < width; ++ scrollX)
			{
				int sourceAddress = (y + scrollY) * this->textWidth + x + scrollX;
				int destinationAddress = sourceAddress - this->textWidth;
				this->textBuffer[destinationAddress] = this->textBuffer[sourceAddress];
			}
		}
		destinationAddress = (y + height - 1) * (x + this->textWidth);
		for(int scrollX = 0; scrollX < width; ++ scrollX)
		{
			this->textBuffer[destinationAddress + scrollX] = 0;
		}
		break;
	case 2: // 左方向
		for(int scrollY = 0; scrollY < height; ++ scrollY)
		{
			for(int scrollX = 1; scrollX < width; ++ scrollX)
			{
				int sourceAddress = (y + scrollY) * this->textWidth + x + scrollX;
				int destinationAddress = sourceAddress - 1;
				this->textBuffer[destinationAddress] = this->textBuffer[sourceAddress];
			}
		}
		destinationAddress = this->scrollRight - 1;
		for(int scrollY = 0; scrollY < height; ++ scrollY)
		{
			this->textBuffer[destinationAddress + scrollY * this->textWidth] = 0;
		}
		break;
	case 3: // 下方向
		for(int scrollY = height - 1; scrollY > 0; -- scrollY)
		{
			for(int scrollX = 0; scrollX < width; ++ scrollX)
			{
				int sourceAddress = (y + scrollY) * this->textWidth + x + scrollX;
				int destinationAddress = sourceAddress + this->textWidth;
				this->textBuffer[destinationAddress] = this->textBuffer[sourceAddress];
			}
		}
		destinationAddress = y * this->textWidth + x;
		for(int scrollX = 0; scrollX < width; ++ scrollX)
		{
			this->textBuffer[destinationAddress + scrollX] = 0;
		}
		break;
	case 4: // 右方向
		for(int scrollY = 0; scrollY < height; ++ scrollY)
		{
			for(int scrollX = width - 1; scrollX > 0; -- scrollX)
			{
				int sourceAddress = (y + scrollY) * this->textWidth + x + scrollX;
				int destinationAddress = sourceAddress + 1;
				this->textBuffer[destinationAddress] = this->textBuffer[sourceAddress];
			}
		}
		destinationAddress = y * this->textWidth + x;
		for(int scrollY = 0; scrollY < height; ++ scrollY)
		{
			this->textBuffer[destinationAddress + scrollY * this->textWidth] = 0;
		}
		break;
	}
	ReDrawText();
}

void Screen::ReDrawText(void)
{
	memset(this->textScreenBuffer, 0, static_cast<size_t>(screenWidth) * screenHeight);
	for(int y = 0; y < this->textHeight; ++ y)
	{
		for(int x = 0; x < this->textWidth; ++ x)
		{
			int address = y * this->textWidth + x;
			DrawFont(x, y, this->textBuffer[address], 0xFFFFFFFF);
		}
	}
}

void Screen::DrawPattern(int x, int y, unsigned char pattern, unsigned int color, unsigned int colorMask)
{
	for(int i = 0; i < 8; ++i)
	{
		int destinationAddress = this->screenWidth * y + x + 7 - i;
		if((destinationAddress < 0) || (destinationAddress >= this->screenBufferSize))
		{
			continue;
		}
		unsigned int patternColor = color * ((pattern >> i) & 1);
		unsigned int writeColor = colorMask == 0xFFFFFFFF ? patternColor : (this->screenBuffer[destinationAddress] & ~colorMask) | (patternColor & colorMask);
		this->screenBuffer[destinationAddress] = writeColor;
	}
}

void Screen::FillFrameBuffer(unsigned int color)
{
	for(int y = 0; y < SCREEN_HEIGHT; ++ y)
	{
		int sourceY = y / this->stretchHeight;
		int frameBufferAddress = (SCREEN_HEIGHT - y - 1) * SCREEN_WIDTH;
		for(int x = 0; x < SCREEN_WIDTH; ++ x)
		{
			int sourceX = x / this->stretchWidth;
			int sourceAddress = sourceY * this->screenWidth + sourceX;
			this->frameBuffer[frameBufferAddress + x] = color;
		}
	}
}

void Screen::FlipGraphic(unsigned int colorMask)
{
	for(int y = 0; y < SCREEN_HEIGHT; ++ y)
	{
		int sourceY = y / this->stretchHeight;
		int frameBufferAddress = (SCREEN_HEIGHT - y - 1) * SCREEN_WIDTH;
		for(int x = 0; x < SCREEN_WIDTH; ++ x)
		{
			int sourceX = x / this->stretchWidth;
			int sourceAddress = sourceY * this->screenWidth + sourceX;
			unsigned int writeColor = this->screenBuffer[sourceAddress] & colorMask;
			this->frameBuffer[frameBufferAddress + x] = writeColor;
		}
	}
}

void Screen::FlipText(unsigned int colorMask)
{
	for(int y = 0; y < SCREEN_HEIGHT; ++ y)
	{
		int sourceY = y / this->textStretchHeight;
		int frameBufferAddress = (SCREEN_HEIGHT - y - 1) * SCREEN_WIDTH;
		for(int x = 0; x < SCREEN_WIDTH; ++ x)
		{
			int sourceX = x / this->textStretchWidth;
			int sourceAddress = sourceY * this->screenWidth + sourceX;
			if (this->textScreenBuffer[sourceAddress] != 0)
			{
//				unsigned int writeColor = colorMask == 0xFFFFFFFF ? this->textScreenBuffer[sourceAddress] : (this->frameBuffer[frameBufferAddress + x] & ~colorMask) | (this->textScreenBuffer[sourceAddress] & colorMask);
				unsigned int writeColor = this->textScreenBuffer[sourceAddress] & colorMask;
				this->frameBuffer[frameBufferAddress + x] = writeColor;
			}
		}
	}
}

void Screen::FlipCursor(unsigned int colorMask)
{
	if(this->showCursor == true)
	{
		++ this->cursorCount;
		if(this->cursorCount >= cursorCountMax)
		{
			this->blinkCursor = 1 - this->blinkCursor;
			this->cursorCount = 0;
			this->cursorCount = 0;
		}
		if(this->blinkCursor == 1)
		{
			KeyBoard& keyBoard = KeyBoard::GetInstance();
			int cursorCharacter = 31;
			if(keyBoard.GetGrph() == true || keyBoard.GetKana() == true)
			{
				cursorCharacter = 147;
			}
			DrawCursorFrameBuffer(this->textX * 8 * this->textStretchWidth, this->textY * 8 * this->textStretchHeight, cursorCharacter, 0xFFFFFFFF, colorMask);
		}
	}
}

void Screen::DrawCursorFrameBuffer(int x, int y, int charcterNumber, unsigned int color, unsigned int colorMask)
{
	if(this->fontBuffer == NULL)
	{
		return;
	}
	int fontX = (charcterNumber / 16) * this->fontHeight;
	int fontY = (charcterNumber % 16) * this->fontWidth;
	unsigned int* fontBuffer = &(this->fontBuffer[fontY * this->fontBufferWidth + fontX]);
	int fontWidth = 8 * this->textStretchWidth;
	int fontHeight = 8 * this->textStretchHeight;
	for(int offsetY = 0; offsetY < fontHeight; ++ offsetY)
	{
		int fontAddressBase = (offsetY / this->textStretchHeight) * this->fontBufferWidth;
		int frameBufferAddress = (SCREEN_HEIGHT - (y + offsetY) - 1) * SCREEN_WIDTH;
		for(int offsetX = 0; offsetX < fontWidth; ++ offsetX)
		{
			int fontAddress = fontAddressBase + (offsetX / this->textStretchWidth);
			unsigned int writeColor = colorMask == 0xFFFFFFFF ? fontBuffer[fontAddress] : (this->frameBuffer[frameBufferAddress + x + offsetX] & ~colorMask) | (fontBuffer[fontAddress] & colorMask);
			this->frameBuffer[frameBufferAddress + x + offsetX] = writeColor;
		}
	}
}

void Screen::ShowCursor(void)
{
	this->showCursor = true;
}

void Screen::HideCursor(void)
{
	this->showCursor = false;
}

bool Screen::ControlCode(int character)
{
	KeyBoard& keyBoard = KeyBoard::GetInstance();
	bool controll = false;
	switch(character)
	{
	case 1:
		++ this->textY;
		if(this->textY > this->scrollBottom)
		{
			this->textY = this->scrollBottom;
		}
		controll = true;
		break;
	case 2:
		-- this->textY;
		if(this->textY < this->scrollTop)
		{
			this->textY = this->scrollTop;
		}
		controll = true;
		break;
	case 3:
		++ this->textX;
		if(this->textX > this->scrollRight)
		{
			this->textY = 0;
			++ this->textY;
			if(this->textY > this->scrollBottom)
			{
				this->textY = this->scrollBottom;
			}
		}
		controll = true;
		break;
	case 4:
		-- this->textX;
		if(this->textX < this->scrollLeft)
		{
			this->textX = this->scrollRight;
			-- this->textY;
			if(this->textY < this->scrollTop)
			{
				this->textY = this->scrollTop;
			}
		}
		controll = true;
		break;
	case 5:
		this->textX = this->scrollLeft;
		this->textY = this->scrollTop;
		controll = true;
		break;
	case 6:
		ClearText();
		controll = true;
		break;
	case 9:
		// GRPHモード
		keyBoard.SetGrph(true);
		controll = true;
		break;
	case 12:
		// かなモード
		keyBoard.SetKana(true);
		controll = true;
		break;
	case 14:
		// かな、GRPHモード解除
		keyBoard.SetKana(false);
		keyBoard.SetGrph(false);
		controll = true;
		break;
	case 15:
		// かな解除
		keyBoard.SetKana(false);
		controll = true;
		break;
	}
	return controll;
}
