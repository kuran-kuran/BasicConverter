#ifndef VARIABLE_HPP
#define VARIABLE_HPP

namespace dms
{
	class String;

	class Variable
	{
	public:
		Variable(void);
		Variable(int value);
		Variable(double value);
		Variable(const Variable& value);
		~Variable(void);
		operator String() const;
		Variable& operator = (int value);
		Variable& operator = (double value);
		Variable& operator = (const Variable& value);
		const Variable operator + (void);
		Variable& operator ++ (void);
		const Variable operator ++ (int);
		const Variable operator + (int value) const;
		const Variable operator + (double value) const;
		const Variable operator + (const Variable& value) const;
		const dms::String operator + (const dms::String& text) const;
		Variable& operator += (int value);
		Variable& operator += (double value);
		Variable& operator += (const Variable& value);
		const Variable operator - (void) const;
		Variable& operator -- (void);
		const Variable operator -- (int);
		const Variable operator - (int value) const;
		const Variable operator - (double value) const;
		const Variable operator - (const Variable& value) const;
		Variable& operator -= (int value);
		Variable& operator -= (double value);
		Variable& operator -= (const Variable& value);
		const Variable operator * (int value) const;
		const Variable operator * (double value) const;
		const Variable operator * (const Variable& value) const;
		Variable& operator *= (int value);
		Variable& operator *= (double value);
		Variable& operator *= (const Variable& value);
		const Variable operator / (int value) const;
		const Variable operator / (double value) const;
		const Variable operator / (const Variable& value) const;
		Variable& operator /= (int value);
		Variable& operator /= (double value);
		Variable& operator /= (const Variable& value);
		const Variable operator ^ (int value) const;
		const Variable operator ^ (double value) const;
		const Variable operator ^ (const Variable& value) const;
		Variable& operator ^= (int value);
		Variable& operator ^= (double value);
		Variable& operator ^= (const Variable& value);
		bool operator == (int value) const;
		bool operator == (double value) const;
		bool operator == (const Variable& value) const;
		bool operator != (int value) const;
		bool operator != (double value) const;
		bool operator != (const Variable& value) const;
		bool operator < (int value) const;
		bool operator < (double value) const;
		bool operator < (const Variable& value) const;
		bool operator > (int value) const;
		bool operator > (double value) const;
		bool operator > (const Variable& value) const;
		bool operator <= (int value) const;
		bool operator <= (double value) const;
		bool operator <= (const Variable& value) const;
		bool operator >= (int value) const;
		bool operator >= (double value) const;
		bool operator >= (const Variable& value) const;
		int GetInt(void) const;
		double GetDouble(void) const;
		dms::String ToString() const;
		bool IsDouble(void) const;
		void clear(void);
	private:
		static bool IsEqualDouble(double value1, double value2);
		static dms::String IntToString(int value);
		static dms::String DoubleToString(double value);
		int intValue;
		double doubleValue;
		bool isDouble;
	};
};

#endif
