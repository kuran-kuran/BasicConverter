#include <stdio.h>
#include <algorithm>
#include <iterator>
#include "FileData.hpp"
#include "SBasic.hpp"

SBasic::SBasic(void)
:buffer(NULL)
,linePointer(128)
{
}

SBasic::~SBasic(void)
{
}

void SBasic::Load(char* buffer)
{
	this->buffer = buffer;
	this->linePointer = 128;
}

void SBasic::LineReset()
{
	this->linePointer = 128;
}

bool SBasic::GetLine(std::vector<char>& receiveBuffer, int& number)
{
	receiveBuffer.clear();
	number = 0;
	int size = 0;
	if(this->linePointer == -1)
	{
		return false;
	}
	size = *reinterpret_cast<unsigned short*>(&this->buffer[this->linePointer]);
	if(size == 0)
	{
		this->linePointer = -1;
		return false;
	}
	number = *(unsigned short*)(&this->buffer[this->linePointer + 2]);
	const char* lineBuffer = &this->buffer[this->linePointer + 4];
	std::copy(lineBuffer, lineBuffer + size - 4, std::back_inserter(receiveBuffer));
	this->linePointer += size;
	return true;
}
