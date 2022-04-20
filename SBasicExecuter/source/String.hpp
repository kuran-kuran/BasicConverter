#ifndef STRING_HPP
#define STRING_HPP

#include <vector>

namespace dms
{
	class Variable;

	class String
	{
	public:
		static const size_t npos = -1;
		String(void);
		~String(void);
		String(const char* text);
		String(const char* text1, const char* text2);
		String(const String& text);
		String(size_t size, char character);
//		String(std::vector<char> text);
		String(std::vector<unsigned char> text);
		String& operator = (const char* text);
		String& operator = (const String& text);
//		String& operator = (std::vector<char> text);
		String& operator = (std::vector<unsigned char> text);
		const char& operator[](const size_t n) const;
		char& operator[](const size_t n);
		String operator + (const char* text);
		String operator + (const String& text);
		String& operator += (const char* text);
		String& operator += (const String& text);
		void push_back(const char c);
		bool operator == (const char* text);
		bool operator == (const String& text);
		bool operator != (const char* text);
		bool operator != (const String& text);
		bool empty(void) const;
		size_t size(void) const;
		size_t length(void) const;
		size_t capacity(void) const;
		const char& front(void) const;
		const char& back(void) const;
		char& front(void);
		char& back(void);
		String substr(const size_t index, const size_t size) const;
		String substr(const size_t index) const;
		const char* c_str() const;
		const char* data() const;
		const unsigned char* uc_str() const;
		size_t find(const char *c, const size_t index) const;
		void SetStoreCallback(void (*)(String* string));
		void clear(void);
	private:
		void (*callback)(String*);
		std::vector<char> buffer;
		size_t bufferSize;
	};
};

#endif
