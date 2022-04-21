#ifndef STACK_HPP
#define STACK_HPP

#include <vector>
#include "Variable.hpp"

namespace dms
{
	class Stack
	{
	public:
		static const size_t intSize = 1;
		static const size_t pointerSize = sizeof(void*) / sizeof(int);
		static const size_t variableSize = (sizeof(double) + sizeof(int) * 2) / sizeof(int);
		Stack(void);
		~Stack(void);
		void PushInt(int data);
		void PushVariable(dms::Variable data);
		void PushPointer(void* pointer);
		void PopInt(void);
		void PopVariable(void);
		void PopPointer(void);
		void Pop(size_t size);
		int GetInt(size_t index);
		dms::Variable GetVariable(size_t index);
		void* GetPointer(size_t index);
		size_t GetSize(void);
		unsigned int& operator [] (size_t index);
	private:
		static const size_t doubleSize = sizeof(double) / sizeof(int);
		std::vector<unsigned int> buffer;
	};
};

#endif
