#ifndef DATA_HPP
#define DATA_HPP

#include "Variable.hpp"
#include "String.hpp"

class Data
{
public:
	Data(void);
	Data(int value);
	Data(double value);
	Data(const dms::String& value);
	Data(const char* value);
	Data& operator = (int value);
	Data& operator = (double value);
	Data& operator = (const dms::String& value);
	Data& operator = (const char* value);
	dms::String GetString() const;
	dms::Variable GetVariable() const;
private:
	dms::String value;
};

#endif
