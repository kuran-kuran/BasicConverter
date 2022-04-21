#ifndef SBASIC_HPP
#define SBASIC_HPP

#include <vector>
#include <string>

class SBasic
{
public:
	SBasic(void);
	~SBasic(void);
	void Load(char* buffer);
	void LineReset();
	bool GetLine(std::vector<char>& buffer, int& number);
private:
	char* buffer;
	int linePointer;
};

#endif
