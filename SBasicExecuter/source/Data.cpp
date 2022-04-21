#include <stdlib.h>
#include "Format.hpp"
#include "Data.hpp"

Data::Data(void)
:value("")
{
}

Data::Data(int value)
{
	this->value = dms::Format("%d", value);
}

Data::Data(double value)
{
	this->value = dms::Format("%f", value);
}

Data::Data(const dms::String& value)
{
	this->value = value;
}

Data::Data(const char* value)
{
	this->value = value;
}

Data& Data::operator = (int value)
{
	this->value = dms::Format("%d", value);
	return *this;
}

Data& Data::operator = (double value)
{
	this->value = dms::Format("%f", value);
	return *this;
}

Data& Data::operator = (const dms::String& value)
{
	this->value = value;
	return *this;
}

Data& Data::operator = (const char* value)
{
	this->value = value;
	return *this;
}

dms::String Data::GetString() const
{
	return this->value;
}

dms::Variable Data::GetVariable() const
{
	char* check;
	dms::Variable result;
	int intValue = static_cast<int>(strtol(this->value.c_str(), &check, 10));
	if(*check == '\0')
	{
		result = intValue;
	}
	else
	{
		double doubleValue = strtod(this->value.c_str(), &check);
		if (*check != '\0')
		{
			result = doubleValue;
		}
	}
	return result;
}
