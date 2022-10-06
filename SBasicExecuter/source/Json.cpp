#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stack>
#include "Json.hpp"

namespace dms
{
	Json::Json(void)
	:find_index(0)
	,data_pair_list()
	,json_type(TYPE_ARRAY)
	{
	}

	Json::Json(std::string data)
	:find_index(0)
	,data_pair_list()
	,json_type(TYPE_ARRAY)
	{
		Parse(data);
	}

	bool Json::Set(std::string data)
	{
		return Parse(data);
	}

	Json::~Json(void)
	{
	}

	std::string Json::operator [] (std::string key)
	{
		for(std::vector<DataPair>::size_type i = 0; i < this->data_pair_list.size(); ++ i)
		{
			if(this->data_pair_list[i].key == key)
			{
				return this->data_pair_list[i].data;
			}
		}
		return "";
	}

	std::string Json::operator [] (size_t index)
	{
		return this->data_pair_list[index].data;
	}

	void Json::FindReset(void)
	{
		this->find_index = 0;
	}

	bool Json::Find(std::string& key, std::string& data)
	{
		if(this->find_index >= this->data_pair_list.size())
		{
			return false;
		}
		key = this->data_pair_list[this->find_index].key;
		data = this->data_pair_list[this->find_index].data;
		++ this->find_index;
		return true;
	}

	size_t Json::Count(void)
	{
		return this->data_pair_list.size();
	}

	void Json::Clear(void)
	{
		this->find_index = 0;
		this->data_pair_list.clear();
	}

	void Json::SetData(std::string key, std::string data)
	{
		int index = GetKeyIndex(key);
		if(index != -1)
		{
			this->data_pair_list[index].data = data;
			this->data_pair_list[index].data_type = TYPE_TEXT;
			return;
		}
		DataPair data_pair;
		data_pair.key = key;
		data_pair.data = data;
		data_pair.data_type = TYPE_TEXT;
		this->data_pair_list.push_back(data_pair);
		this->json_type = TYPE_OBJECT;
	}

	void Json::SetData(std::string key, int data)
	{
		int index = GetKeyIndex(key);
		if(index != -1)
		{
			this->data_pair_list[index].data = IntText(data);
			this->data_pair_list[index].data_type = TYPE_NUMERIC;
			return;
		}
		DataPair data_pair;
		data_pair.key = key;
		data_pair.data = IntText(data);
		data_pair.data_type = TYPE_NUMERIC;
		this->data_pair_list.push_back(data_pair);
		this->json_type = TYPE_OBJECT;
	}

	void Json::SetData(std::string key, float data)
	{
		int index = GetKeyIndex(key);
		if(index != -1)
		{
			this->data_pair_list[index].data = FloatText(data);
			this->data_pair_list[index].data_type = TYPE_NUMERIC;
			return;
		}
		DataPair data_pair;
		data_pair.key = key;
		data_pair.data = FloatText(data);
		data_pair.data_type = TYPE_NUMERIC;
		this->data_pair_list.push_back(data_pair);
		this->json_type = TYPE_OBJECT;
	}

	void Json::SetData(std::string key, Json& data)
	{
		int index = GetKeyIndex(key);
		if(index != -1)
		{
			this->data_pair_list[index].data = data.Encode();
			if(this->data_pair_list[index].data[0] == '[')
			{
				this->data_pair_list[index].data_type = TYPE_ARRAY;
			}
			else
			{
				this->data_pair_list[index].data_type = TYPE_OBJECT;
			}
			return;
		}
		DataPair data_pair;
		data_pair.key = key;
		data_pair.data = data.Encode();
		if(data_pair.data[0] == '[')
		{
			data_pair.data_type = TYPE_ARRAY;
		}
		else
		{
			data_pair.data_type = TYPE_OBJECT;
		}
		this->data_pair_list.push_back(data_pair);
		this->json_type = TYPE_OBJECT;
	}

	void Json::SetData(std::string data)
	{
		size_t index = Count();
		DataPair data_pair;
		data_pair.key = IntText(static_cast<int>(index));
		data_pair.data = data;
		data_pair.data_type = TYPE_TEXT;
		this->data_pair_list.push_back(data_pair);
	}

	void Json::SetData(int data)
	{
		size_t index = Count();
		DataPair data_pair;
		data_pair.key = IntText(static_cast<int>(index));
		data_pair.data = IntText(data);
		data_pair.data_type = TYPE_NUMERIC;
		this->data_pair_list.push_back(data_pair);
	}

	void Json::SetData(float data)
	{
		size_t index = Count();
		DataPair data_pair;
		data_pair.key = IntText(static_cast<int>(index));
		data_pair.data = FloatText(data);
		data_pair.data_type = TYPE_NUMERIC;
		this->data_pair_list.push_back(data_pair);
	}

	void Json::SetData(Json& data)
	{
		size_t index = Count();
		DataPair data_pair;
		data_pair.key = IntText(static_cast<int>(index));
		data_pair.data = data.Encode();
		if(data_pair.data[0] == '[')
		{
			data_pair.data_type = TYPE_ARRAY;
		}
		else
		{
			data_pair.data_type = TYPE_OBJECT;
		}
		this->data_pair_list.push_back(data_pair);
	}

	std::string Json::Encode(void)
	{
		std::string result = "";
		if(this->json_type == TYPE_OBJECT)
		{
			result += "{";
			for(size_t i = 0; i < Count(); ++ i)
			{
				result += "\"";
				result += this->data_pair_list[i].key;
				result += "\"";
				result += ":";
				if(this->data_pair_list[i].data_type == TYPE_TEXT)
				{
					result += "\"";
					result += this->data_pair_list[i].data;
					result += "\"";
				}
				else
				{
					result += this->data_pair_list[i].data;
				}
				if(i != (Count() - 1))
				{
					result += ",";
				}
			}
			result += "}";
		}
		else
		{
			result += "[";
			for(size_t i = 0; i < Count(); ++ i)
			{
				if(this->data_pair_list[i].data_type == TYPE_TEXT)
				{
					result += "\"";
					result += this->data_pair_list[i].data;
					result += "\"";
				}
				else
				{
					result += this->data_pair_list[i].data;
				}
				if(i != (Count() - 1))
				{
					result += ",";
				}
			}
			result += "]";
		}
		return result;
	}

	std::string Json::DecodeEscape(std::string text)
	{
		text = Json::ReplaceAll(text, "\\\"", "\"");
		text = Json::ReplaceAll(text, "\\\\", "\\");
		text = Json::ReplaceAll(text, "\\n", "\n");
		text = Json::ReplaceAll(text, "\\r", "\r");
		text = Json::ReplaceAll(text, "\\t", "\t");
		return text;
	}

	int Json::Int(std::string text)
	{
		int result = atoi(text.c_str());
		return result;
	}

	float Json::Float(std::string text)
	{
		float result = static_cast<float>(atof(text.c_str()));
		return result;
	}

	std::string Json::IntText(int data)
	{
		char data_text[256];
#ifdef _WIN32
		sprintf_s(data_text, 256, "%d", data);
#else
		sprintf(data_text, "%d", data);
#endif
		return data_text;
	}

	std::string Json::FloatText(float data)
	{
		char data_text[256];
#ifdef _WIN32
		sprintf_s(data_text, 256, "%g", data);
#else
		sprintf(data_text, "%g", data);
#endif
		return data_text;
	}

	std::string Json::ReplaceAll(std::string text, std::string from, std::string to)
	{
		std::string::size_type pos = text.find(from);
		while(pos != std::string::npos)
		{
			text.replace(pos, from.size(), to);
			pos = text.find(from, pos + to.size());
		}
		return text;
	}

	int Json::GetKeyIndex(std::string key)
	{
		int result = -1;
		for(std::vector<DataPair>::size_type i = 0; i < this->data_pair_list.size(); ++ i)
		{
			if(this->data_pair_list[i].key == key)
			{
				result = static_cast<int>(i);
				break;
			}
		}
		return result;
	}

	Json::JsonType Json::GetDataType(char word, bool word_text_flag)
	{
		if(word == '[')
		{
			return TYPE_ARRAY;
		}
		if(word == '{')
		{
			return TYPE_OBJECT;
		}
		if(word_text_flag == true)
		{
			return TYPE_TEXT;
		}
		return TYPE_NUMERIC;
	}

	bool Json::Parse(std::string data)
	{
		std::string::size_type length = data.length();
		bool result = true;
		bool quotation = false;
		int bracket_type = 0;
		std::stack<int> bracket;
		DataPair data_pair = {};
		std::string word = "";
		bool word_text_flag = false;
		int index = 0;
		for(std::string::size_type i = 0; i < length; ++ i)
		{
			if(data[i] == '\"')
			{
				quotation = !quotation;
			}
			if(quotation == true)
			{
				if(bracket.size() == 1)
				{
					if(data[i] != '\"')
					{
						word += data[i];
						word_text_flag = true;
					}
				}
			}
			if(data[i] == '[')
			{
				if(bracket_type == 0)
				{
					bracket_type = 1;
				}
				bracket.push(1);
				if(bracket.size() == 1)
				{
					continue;
				}
			}
			if(data[i] == '{')
			{
				if(bracket_type == 0)
				{
					bracket_type = 2;
				}
				bracket.push(2);
			}
			if(data[i] == '\t')
			{
				continue;
			}
			if(data[i] == ' ')
			{
				continue;
			}
			if(data[i] == '\xA')
			{
				continue;
			}
			if(data[i] == '\xD')
			{
				continue;
			}
			if((quotation == false) && (data[i] == ':'))
			{
				if(bracket.size() == 1)
				{
//					printf("(%u) key = %s\n", bracket.size(), word.c_str());
					data_pair.key = word;
					word = "";
					this->json_type = TYPE_OBJECT;
					continue;
				}
			}
			if((quotation == false) && (data[i] == ','))
			{
				if(bracket.size() == 1)
				{
//					printf("data1 = %s\n", word.c_str());
					data_pair.data = word;
					data_pair.data_type = GetDataType(word[0], word_text_flag);
					if(bracket_type == 1)
					{
						data_pair.key = IntText(index);
						++ index;
					}
					this->data_pair_list.push_back(data_pair);
					word = "";
					data_pair.key.clear();
					data_pair.data.clear();
					continue;
				}
			}
			if((quotation == false) && (data[i] == ']'))
			{
				if(bracket.empty() == true)
				{
					result = false;
					break;
				}
				if(bracket.size() > 1)
				{
					word += data[i];
					word_text_flag = false;
				}
				int top_bracket_type = bracket.top();
				bracket.pop();
				if(top_bracket_type != 1)
				{
					result = false;
					break;
				}
			}
			else if((quotation == false) && (data[i] == '}'))
			{
				if(bracket.empty() == true)
				{
					result = false;
					break;
				}
				if(bracket.size() > 1)
				{
					word += data[i];
					word_text_flag = false;
				}
				int top_bracket_type = bracket.top();
				bracket.pop();
				if(top_bracket_type != 2)
				{
					result = false;
					break;
				}
			}
			else if(bracket.size() > 1)
			{
				word += data[i];
				word_text_flag = false;
			}
			else if((quotation == false) && (bracket_type == 1) && (bracket.empty() == false))
			{
				if(data[i] != '\"')
				{
					word += data[i];
					word_text_flag = false;
				}
			}
			else if((quotation == false) && (data_pair.key.empty() == false))
			{
				if(bracket.empty() == true)
				{
					break;
				}
				if(data[i] != '\"')
				{
					word += data[i];
					word_text_flag = false;
				}
			}
		}
		if(word.empty() == false)
		{
//			printf("data2 = %s\n", word.c_str());
			data_pair.data = word;
			data_pair.data_type = GetDataType(word[0], word_text_flag);
			if(bracket_type == 1)
			{
				data_pair.key = IntText(index);
				++ index;
			}
			this->data_pair_list.push_back(data_pair);
		}
		return result;
	}
};
