#include <stdlib.h>
#include <iterator>
#include "Variable.hpp"
#include "String.hpp"

namespace dms
{
	String::String(void)
	:bufferSize(0)
	,callback(NULL)
	{
		this->buffer.clear();
	}

	String::~String(void)
	{
	}

	String::String(const char* text)
	:bufferSize(0)
	,callback(NULL)
	{
		if((text == NULL) || ((text != NULL) && (text[0] == '\0')))
		{
			this->bufferSize = 0;
			this->buffer.push_back(0);
			return;
		}
		std::copy(text, text + strlen(text), std::back_inserter(this->buffer));
		this->bufferSize = this->buffer.size();
		this->buffer.push_back(0);
	}

	String::String(const char* text1, const char* text2)
	:bufferSize(0)
	,callback(NULL)
	{
		std::copy(text1, text2 + 1, std::back_inserter(this->buffer));
		this->bufferSize = this->buffer.size();
		this->buffer.push_back(0);
	}

	String::String(const String& text)
	:bufferSize(0)
	,callback(NULL)
	{
		*this = text;
	}

	String::String(size_t size, char character)
	:bufferSize(0)
	,callback(NULL)
	{
		this->bufferSize = size;
		for(size_t i = 0; i < size; ++ i)
		{
			this->buffer.push_back(character);
		}
		this->buffer.push_back(0);
	}

/*
	String::String(std::vector<char> text)
	:bufferSize(0)
	,callback(NULL)
	{
		this->buffer = text;
		this->bufferSize = this->buffer.size();
		this->buffer.push_back(0);
	}
*/

	String::String(std::vector<unsigned char> text)
	:bufferSize(0)
	,callback(NULL)
	{
		const unsigned char* textBuffer = &text[0];
		std::copy(textBuffer, textBuffer + text.size(), std::back_inserter(this->buffer));
		this->bufferSize = this->buffer.size();
		this->buffer.push_back(0);
	}

	String& String::operator = (const char* text)
	{
		if((text == NULL) || ((text != NULL) && (text[0] == '\0')))
		{
			this->bufferSize = 0;
			this->buffer.clear();
			this->buffer.push_back(0);
			return *this;
		}
		this->buffer.clear();
		std::copy(text, text + strlen(text), std::back_inserter(this->buffer));
		this->bufferSize = this->buffer.size();
		this->buffer.push_back(0);
		if(this->callback != NULL)
		{
			this->callback(this);
		}
		return *this;
	}

	String& String::operator = (const String& text)
	{
		this->buffer.clear();
		this->bufferSize = 0;
		if(text.empty())
		{
			return *this;
		}
		const char* textBuffer = &text[0];
		std::copy(textBuffer, textBuffer + text.size(), std::back_inserter(this->buffer));
		this->bufferSize = this->buffer.size();
		this->buffer.push_back(0);
		if(this->callback != NULL)
		{
			this->callback(this);
		}
		return *this;
	}

/*
	String& String::operator = (std::vector<char> text)
	{
		this->buffer.clear();
		const char* textBuffer = &text[0];
		std::copy(textBuffer, textBuffer + text.size(), std::back_inserter(this->buffer));
		this->bufferSize = this->buffer.size();
		this->buffer.push_back(0);
		return *this;
	}
*/

	String& String::operator = (std::vector<unsigned char> text)
	{
		this->buffer.clear();
		const unsigned char* textBuffer = &text[0];
		std::copy(textBuffer, textBuffer + text.size(), std::back_inserter(this->buffer));
		this->bufferSize = this->buffer.size();
		this->buffer.push_back(0);
		if(this->callback != NULL)
		{
			this->callback(this);
		}
		return *this;
	}

	const char& String::operator [] (const size_t n) const
	{
		return this->buffer[n];
	}

	char& String::operator [] (const size_t n)
	{
		return this->buffer[n];
	}

	String String::operator + (const char* text)
	{
		String result = *this;
		if(text != NULL)
		{
			result += text;
		}
		return result;
	}

	String String::operator + (const String& text)
	{
		String result = *this;
		if(text.empty() != true)
		{
			result += text;
		}
		return result;
	}

	String& String::operator += (const char* text)
	{
		if(this->buffer.empty() == false)
		{
			this->buffer.pop_back();
		}
		std::copy(text, text + strlen(text), std::back_inserter(this->buffer));
		this->bufferSize = this->buffer.size();
		this->buffer.push_back(0);
		return *this;
	}

	String& String::operator += (const String& text)
	{
		if(this->buffer.empty() == false)
		{
			this->buffer.pop_back();
		}
		std::copy(&text[0], &text[text.size()], std::back_inserter(this->buffer));
		this->bufferSize = this->buffer.size();
		this->buffer.push_back(0);
		return *this;
	}

	void String::push_back(const char c)
	{
		if(this->buffer.empty() == false)
		{
			this->buffer.pop_back();
		}
		this->buffer.push_back(c);
		this->bufferSize = this->buffer.size();
		this->buffer.push_back(0);
	}

	int String::operator == (const char* text) const
	{
		size_t size = this->size() < strlen(text) ? this->size() : strlen(text);
		return -(this->size() == strlen(text) && (memcmp(&this->buffer[0], text, size) == 0));
	}

	int String::operator == (const String& text) const
	{
		size_t size = this->size() < text.size() ? this->size() : text.size();
		return -(this->size() == text.size() && (memcmp(&this->buffer[0], text.c_str(), size) == 0));
	}

	int String::operator != (const char* text) const
	{
		size_t size = this->size() < strlen(text) ? this->size() : strlen(text);
		return -(this->size() != strlen(text) && (memcmp(&this->buffer[0], text, size) == 0));
	}

	int String::operator != (const String& text) const
	{
		size_t size = this->size() < text.size() ? this->size() : text.size();
		return -(this->size() != text.size() && (memcmp(&this->buffer[0], text.c_str(), size) == 0));
	}

	int String::operator < (const char* text) const
	{
		size_t size = this->size() < strlen(text) ? this->size() : strlen(text);
		return -(this->size() != strlen(text) && (memcmp(&this->buffer[0], text, size) < 0));
	}

	int String::operator < (const String& text) const
	{
		size_t size = this->size() < text.size() ? this->size() : text.size();
		return -(this->size() != text.size() && (memcmp(&this->buffer[0], text.c_str(), size) < 0));
	}

	int String::operator > (const char* text) const
	{
		size_t size = this->size() < strlen(text) ? this->size() : strlen(text);
		return -(this->size() != strlen(text) && (memcmp(&this->buffer[0], text, size) > 0));
	}

	int String::operator > (const String& text) const
	{
		size_t size = this->size() < text.size() ? this->size() : text.size();
		return -(this->size() != text.size() && (memcmp(&this->buffer[0], text.c_str(), size) > 0));
	}

	int String::operator <= (const char* text) const
	{
		size_t size = this->size() < strlen(text) ? this->size() : strlen(text);
		return -(this->size() != strlen(text) && (memcmp(&this->buffer[0], text, size) <= 0));
	}

	int String::operator <= (const String& text) const
	{
		size_t size = this->size() < text.size() ? this->size() : text.size();
		return -(this->size() != text.size() && (memcmp(&this->buffer[0], text.c_str(), size) <= 0));
	}

	int String::operator >= (const char* text) const
	{
		size_t size = this->size() < strlen(text) ? this->size() : strlen(text);
		return -(this->size() != strlen(text) && (memcmp(&this->buffer[0], text, size) >= 0));
	}

	int String::operator >= (const String& text) const
	{
		size_t size = this->size() < text.size() ? this->size() : text.size();
		return -(this->size() != text.size() && (memcmp(&this->buffer[0], text.c_str(), size) >= 0));
	}

	bool String::empty(void) const
	{
		if(this->bufferSize == 0)
		{
			return true;
		}
		return false;
	}

	size_t String::size(void) const
	{
		return this->bufferSize;
	}

	size_t String::length(void) const
	{
		return this->bufferSize;
	}

	size_t String::capacity(void) const
	{
		return this->buffer.capacity();
	}

	const char& String::front(void) const
	{
		return this->buffer[0];
	}

	const char& String::back(void) const
	{
		return this->buffer[this->buffer.size() - 1];
	}

	char& String::front(void)
	{
		return this->buffer[0];
	}

	char& String::back(void)
	{
		return this->buffer[this->buffer.size() - 1];
	}

	String String::substr(const size_t index, const size_t size) const
	{
		String result(&this->buffer[index], &this->buffer[index + size - 1]);
		return result;
	}

	String String::substr(const size_t index) const
	{
		String result;
		for(size_t i = index; i < this->bufferSize; ++ i)
		{
			result.push_back(this->buffer[i]);
		}
		return result;
	}

	const char* String::c_str() const
	{
		if(empty() == true)
		{
			return NULL;
		}
		return &this->buffer[0];
	}

	const unsigned char* String::uc_str() const
	{
		if(empty() == true)
		{
			return NULL;
		}
		return reinterpret_cast<const unsigned char*>(&this->buffer[0]);
	}

	const char* String::data() const
	{
		if(empty() == true)
		{
			return NULL;
		}
		return &this->buffer[0];
	}

	size_t String::find(const char *c, const size_t index) const
	{
		const char *pos = strstr(&this->buffer[index], c);
		if(pos == NULL)
		{
			return String::npos;
		}
		return pos - &this->buffer[0];
	}

	void String::SetStoreCallback(void (*callback)(String*))
	{
		this->callback = callback;
	}

	void String::clear(void)
	{
		this->bufferSize = 0;
		this->buffer.clear();
	}
};
