#include "Stack.hpp"

namespace dms
{
	Stack::Stack(void)
	:buffer()
	{
	}

	Stack::~Stack(void)
	{
	}

	void Stack::PushInt(int data)
	{
		unsigned int* insertBuffer = reinterpret_cast<unsigned int*>(&data);
		this->buffer.insert(this->buffer.end(), insertBuffer, insertBuffer + 1);
	}

	void Stack::PushVariable(dms::Variable data)
	{
		// ƒtƒ‰ƒO
		int isDouble = data.IsDouble() ? 1 : 0;
		unsigned int* insertBuffer = reinterpret_cast<unsigned int*>(&isDouble);
		this->buffer.insert(this->buffer.end(), insertBuffer, insertBuffer + 1);
		// int•”
		PushInt(data.GetInt());
		// double•”
		double doubleData = data.GetDouble();
		insertBuffer = reinterpret_cast<unsigned int*>(&doubleData);
		this->buffer.insert(this->buffer.end(), insertBuffer, insertBuffer + Stack::doubleSize);
	}

	void Stack::PushPointer(void* pointer)
	{
		unsigned int* insertBuffer = reinterpret_cast<unsigned int*>(&pointer);
		this->buffer.insert(this->buffer.end(), insertBuffer, insertBuffer + Stack::pointerSize);
	}

	void Stack::PopInt(void)
	{
		this->Pop(Stack::intSize);
	}

	void Stack::PopVariable(void)
	{
		this->Pop(Stack::variableSize);
	}

	void Stack::PopPointer(void)
	{
		this->Pop(Stack::pointerSize);
	}

	void Stack::Pop(size_t size)
	{
		this->buffer.erase(this->buffer.end() - size, this->buffer.end());
	}

	int Stack::GetInt(size_t index)
	{
		size_t address = this->buffer.size() - index;
		int data = *reinterpret_cast<int*>(&this->buffer[address]);
		return data;
	}

	dms::Variable Stack::GetVariable(size_t index)
	{
		size_t address = this->buffer.size() - index;
		int isDouble = *reinterpret_cast<int*>(&this->buffer[address]);
		dms::Variable data;
		if(isDouble == 1)
		{
			address += (Stack::intSize + Stack::intSize);
			data = *reinterpret_cast<double*>(&this->buffer[address]);
		}
		else
		{
			address += Stack::intSize;
			data = *reinterpret_cast<int*>(&this->buffer[address]);
		}
		return data;
	}

	void* Stack::GetPointer(size_t index)
	{
		size_t address = this->buffer.size() - index;
		void* data = *reinterpret_cast<void**>(&this->buffer[address]);
		return data;
	}

	size_t Stack::GetSize(void)
	{
		return this->buffer.size();
	}

	unsigned int& Stack::operator [] (size_t index)
	{
		return this->buffer[this->buffer.size() - index - 1];
	}
};
