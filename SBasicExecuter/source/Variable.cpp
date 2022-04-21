#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "String.hpp"
#include "Variable.hpp"

namespace dms
{
	Variable::Variable(void)
	:intValue(0)
	,doubleValue(0.0)
	,isDouble(false)
	{
	}

	Variable::Variable(int value)
	{
		*this = value;
	}

	Variable::Variable(double value)
	{
		*this = value;
	}

	Variable::Variable(const Variable& value)
	{
		*this = value;
	}

	Variable::~Variable(void)
	{
	}

	Variable::operator String() const
	{
		return ToString();
	}

	Variable::operator bool() const
	{
		if(this->isDouble == true)
		{
			return this->doubleValue != 0.0;
		}
		return this->intValue != 0;
	}

	Variable& Variable::operator = (int value)
	{
		this->intValue = value;
		this->doubleValue = 0.0;
		this->isDouble = false;
		return *this;
	}

	Variable& Variable::operator = (double value)
	{
		this->intValue = 0;
		this->doubleValue = value;
		this->isDouble = true;
		return *this;
	}

	Variable& Variable::operator = (const Variable& value)
	{
		this->isDouble = value.IsDouble();
		this->doubleValue = value.GetDouble();
		this->intValue = value.GetInt();
		return *this;
	}

	const Variable Variable::operator + (void)
	{
		return *this;
	}

	Variable& Variable::operator ++ (void)
	{
		if(this->isDouble)
		{
			this->doubleValue += 1.0;
		}
		else
		{
			++ this->intValue;
		}
		return *this;
	}

	const Variable Variable::operator ++ (int)
	{
		Variable result = *this;
		++(*this);
		return result;
	}

	const Variable Variable::operator + (int value) const
	{
		if(IsDouble() == true)
		{
			return Variable(this->doubleValue + static_cast<double>(value));
		}
		return Variable(this->intValue + value);
	}

	const Variable Variable::operator + (double value) const
	{
		if(IsDouble() == true)
		{
			return Variable(this->doubleValue + value);
		}
		return Variable(static_cast<double>(this->intValue) + value);
	}

	const Variable Variable::operator + (const Variable& value) const
	{
		if(value.IsDouble() == true)
		{
			return *this + value.GetDouble();
		}
		return *this + value.GetInt();
	}

	const dms::String Variable::operator + (const dms::String& text) const
	{
		return this->ToString() + text;
	}

	Variable& Variable::operator += (int value)
	{
		if(this->isDouble)
		{
			this->doubleValue += static_cast<double>(value);
		}
		else
		{
			this->intValue += value;
		}
		return *this;
	}

	Variable& Variable::operator += (double value)
	{
		if(this->isDouble)
		{
			this->doubleValue += value;
		}
		else
		{
			this->doubleValue = static_cast<double>(this->intValue) + value;
			this->isDouble = true;
		}
		return *this;
	}

	Variable& Variable::operator += (const Variable& value)
	{
		if(value.IsDouble() == true)
		{
			*this += value.GetDouble();
		}
		else
		{
			*this += value.GetInt();
		}
		return *this;
	}

	const Variable Variable::operator - (void) const
	{
		Variable result = *this;
		if(result.IsDouble() == true)
		{
			result = -result.GetDouble();
		}
		else
		{
			result = -result.GetInt();
		}
		return result;
	}

	Variable& Variable::operator -- (void)
	{
		if(this->isDouble)
		{
			this->doubleValue -= 1.0;
		}
		else
		{
			-- this->intValue;
		}
		return *this;
	}

	const Variable Variable::operator -- (int)
	{
		Variable result = *this;
		--(*this);
		return result;
	}

	const Variable Variable::operator - (int value) const
	{
		if(IsDouble() == true)
		{
			return Variable(this->doubleValue - static_cast<double>(value));
		}
		return Variable(this->intValue - value);
	}

	const Variable Variable::operator - (double value) const
	{
		if(IsDouble() == true)
		{
			return Variable(this->doubleValue - value);
		}
		return Variable(static_cast<double>(this->intValue) - value);
	}

	const Variable Variable::operator - (const Variable& value) const
	{
		if(value.IsDouble() == true)
		{
			return *this - value.GetDouble();
		}
		return *this - value.GetInt();
	}

	Variable& Variable::operator -= (int value)
	{
		if(this->isDouble)
		{
			this->doubleValue -= static_cast<double>(value);
		}
		else
		{
			this->intValue -= value;
		}
		return *this;
	}

	Variable& Variable::operator -= (double value)
	{
		if(this->isDouble)
		{
			this->doubleValue -= value;
		}
		else
		{
			this->doubleValue = static_cast<double>(this->intValue) - value;
			this->isDouble = true;
		}
		return *this;
	}

	Variable& Variable::operator -= (const Variable& value)
	{
		if(value.IsDouble() == true)
		{
			*this -= value.GetDouble();
		}
		else
		{
			*this -= value.GetInt();
		}
		return *this;
	}

	const Variable Variable::operator * (int value) const
	{
		if(IsDouble() == true)
		{
			return Variable(this->doubleValue * static_cast<double>(value));
		}
		return Variable(this->intValue * value);
	}

	const Variable Variable::operator * (double value) const
	{
		if(IsDouble() == true)
		{
			return Variable(this->doubleValue * value);
		}
		return Variable(static_cast<double>(this->intValue) * value);
	}

	const Variable Variable::operator * (const Variable& value) const
	{
		if(value.IsDouble() == true)
		{
			return *this * value.GetDouble();
		}
		return *this * value.GetInt();
	}

	Variable& Variable::operator *= (int value)
	{
		if(this->isDouble)
		{
			this->doubleValue *= static_cast<double>(value);
		}
		else
		{
			this->intValue *= value;
		}
		return *this;
	}

	Variable& Variable::operator *= (double value)
	{
		if(this->isDouble)
		{
			this->doubleValue *= value;
		}
		else
		{
			this->doubleValue = static_cast<double>(this->intValue) * value;
			this->isDouble = true;
		}
		return *this;
	}

	Variable& Variable::operator *= (const Variable& value)
	{
		if(value.IsDouble() == true)
		{
			*this *= value.GetDouble();
		}
		else
		{
			*this *= value.GetInt();
		}
		return *this;
	}

	const Variable Variable::operator / (int value) const
	{
		if(IsDouble() == true)
		{
			return Variable(this->doubleValue / static_cast<double>(value));
		}
		return Variable(static_cast<double>(this->intValue) / static_cast<double>(value));
	}

	const Variable Variable::operator / (double value) const
	{
		if(IsDouble() == true)
		{
			return Variable(this->doubleValue / value);
		}
		return Variable(static_cast<double>(this->intValue) / value);
	}

	const Variable Variable::operator / (const Variable& value) const
	{
		if(value.IsDouble() == true)
		{
			return *this / value.GetDouble();
		}
		return *this / value.GetInt();
	}

	Variable& Variable::operator /= (int value)
	{
		if(this->isDouble)
		{
			this->doubleValue /= static_cast<double>(value);
		}
		else
		{
			this->doubleValue = static_cast<double>(this->intValue) / static_cast<double>(value);
			this->isDouble = true;
		}
		return *this;
	}

	Variable& Variable::operator /= (double value)
	{
		if(this->isDouble)
		{
			this->doubleValue /= value;
		}
		else
		{
			this->doubleValue = static_cast<double>(this->intValue) / value;
			this->isDouble = true;
		}
		return *this;
	}

	Variable& Variable::operator /= (const Variable& value)
	{
		if(value.IsDouble() == true)
		{
			*this /= value.GetDouble();
		}
		else
		{
			*this /= value.GetInt();
		}
		return *this;
	}

	const Variable Variable::operator ^ (int value) const
	{
		double result = pow(GetDouble(), static_cast<double>(value));
		return result;
	}

	const Variable Variable::operator ^ (double value) const
	{
		double result = pow(GetDouble(), value);
		return result;
	}

	const Variable Variable::operator ^ (const Variable& value) const
	{
		double result = pow(GetDouble(), value.GetDouble());
		return result;
	}

	Variable& Variable::operator ^= (int value)
	{
		this->doubleValue = pow(GetDouble(), static_cast<double>(value));
		this->isDouble = true;
		return *this;
	}

	Variable& Variable::operator ^= (double value)
	{
		this->doubleValue = pow(GetDouble(), value);
		this->isDouble = true;
		return *this;
	}

	Variable& Variable::operator ^= (const Variable& value)
	{
		this->doubleValue = pow(GetDouble(), value.GetDouble());
		this->isDouble = true;
		return *this;
	}

	bool Variable::operator == (int value) const
	{
		if(isDouble == true)
		{
			return IsEqualDouble(this->doubleValue, static_cast<double>(value));
		}
		return intValue == value;
	}

	bool Variable::operator == (double value) const
	{
		return IsEqualDouble(this->doubleValue, static_cast<double>(value));
	}

	bool Variable::operator == (const Variable& value) const
	{
		if((this->isDouble == false) && (value.IsDouble() == false))
		{
			return this->intValue == value.GetInt();
		}
		else if((this->isDouble == false) && (value.IsDouble() == true))
		{
			return IsEqualDouble(static_cast<double>(this->intValue), value.GetDouble());
		}
		else if((this->isDouble == true) && (value.IsDouble() == false))
		{
			return IsEqualDouble(this->doubleValue, static_cast<double>(value.GetInt()));
		}
		return IsEqualDouble(this->doubleValue, value.GetDouble());
	}

	bool Variable::operator != (int value) const
	{
		if(isDouble == true)
		{
			return !IsEqualDouble(this->doubleValue, static_cast<double>(value));
		}
		return intValue != value;
	}

	bool Variable::operator != (double value) const
	{
		return !IsEqualDouble(this->doubleValue, static_cast<double>(value));
	}

	bool Variable::operator != (const Variable& value) const
	{
		if((this->isDouble == false) && (value.IsDouble() == false))
		{
			return this->intValue != value.GetInt();
		}
		else if((this->isDouble == false) && (value.IsDouble() == true))
		{
			return !IsEqualDouble(static_cast<double>(this->intValue), value.GetDouble());
		}
		else if((this->isDouble == true) && (value.IsDouble() == false))
		{
			return !IsEqualDouble(this->doubleValue, static_cast<double>(value.GetInt()));
		}
		return !IsEqualDouble(this->doubleValue, value.GetDouble());
	}


	bool Variable::operator < (int value) const
	{
		if(isDouble == true)
		{
			return this->doubleValue < static_cast<double>(value);
		}
		return this->intValue < value;
	}

	bool Variable::operator < (double value) const
	{
		if(isDouble == true)
		{
			return this->doubleValue < value;
		}
		return static_cast<double>(this->intValue) < value;
	}

	bool Variable::operator < (const Variable& value) const
	{
		if(value.IsDouble() == true)
		{
			return *this < value.GetDouble();
		}
		return *this < value.GetInt();
	}

	bool Variable::operator > (int value) const
	{
		if(isDouble == true)
		{
			return this->doubleValue > static_cast<double>(value);
		}
		return this->intValue > value;
	}

	bool Variable::operator > (double value) const
	{
		if(isDouble == true)
		{
			return this->doubleValue > value;
		}
		return static_cast<double>(this->intValue) > value;
	}

	bool Variable::operator > (const Variable& value) const
	{
		if(value.IsDouble() == true)
		{
			return *this > value.GetDouble();
		}
		return *this > value.GetInt();
	}

	bool Variable::operator <= (int value) const
	{
		if(isDouble == true)
		{
			return this->doubleValue <= static_cast<double>(value);
		}
		return this->intValue <= value;
	}

	bool Variable::operator <= (double value) const
	{
		if(isDouble == true)
		{
			return this->doubleValue <= value;
		}
		return static_cast<double>(this->intValue) <= value;
	}

	bool Variable::operator <= (const Variable& value) const
	{
		if(value.IsDouble() == true)
		{
			return *this <= value.GetDouble();
		}
		return *this <= value.GetInt();
	}

	bool Variable::operator >= (int value) const
	{
		if(isDouble == true)
		{
			return this->doubleValue >= static_cast<double>(value);
		}
		return this->intValue >= value;
	}

	bool Variable::operator >= (double value) const
	{
		if(isDouble == true)
		{
			return this->doubleValue >= value;
		}
		return static_cast<double>(this->intValue) >= value;
	}

	bool Variable::operator >= (const Variable& value) const
	{
		if(value.IsDouble() == true)
		{
			return *this >= value.GetDouble();
		}
		return *this >= value.GetInt();
	}

	int Variable::GetInt(void) const
	{
		if(this->isDouble)
		{
			return static_cast<int>(this->doubleValue);
		}
		return this->intValue;
	}

	double Variable::GetDouble(void) const
	{
		if(this->isDouble)
		{
			return this->doubleValue;
		}
		return static_cast<double>(this->intValue);
	}

	dms::String Variable::ToString() const
	{
		if(this->isDouble == true)
		{
			return DoubleToString(this->doubleValue);
		}
		return IntToString(this->intValue);
	}

	bool Variable::IsDouble(void) const
	{
		return this->isDouble;
	}

	bool Variable::IsEqualDouble(double value1, double value2)
	{
		if(fabs(value1 - value2) <= DBL_EPSILON * fmax(1, fmax(fabs(value1), fabs(value2))))
		{
			return true;
		}
		return false;
	}

	dms::String Variable::IntToString(int value)
	{
		char buffer[256];
		if(value > 0)
		{
			sprintf_s(buffer, 256, " %d", value);
		}
		else
		{
			sprintf_s(buffer, 256, "%d", value);
		}
		return buffer;
	}

	dms::String Variable::DoubleToString(double value)
	{
		char buffer[256];
		if(value > 0.0)
		{
			sprintf_s(buffer, 256, " %g", value);
		}
		else
		{
			sprintf_s(buffer, 256, "%g", value);
		}
		return buffer;
	}

	void Variable::clear(void)
	{
		this->intValue = 0;
		this->doubleValue = 0.0;
		this->isDouble = false;
	}
};
