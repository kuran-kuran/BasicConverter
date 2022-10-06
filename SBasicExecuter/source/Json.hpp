#ifndef LIBDMS_JSON_HPP
#define LIBDMS_JSON_HPP

#include <string>
#include <vector>

namespace dms
{
	class Json
	{
	public:
		Json(void);
		Json(std::string data);
		~Json(void);
		bool Set(std::string data);
		std::string operator [] (std::string key);
		std::string operator [] (size_t index);
		void FindReset(void);
		bool Find(std::string& key, std::string& data);
		size_t Count(void);
		void Clear(void);
		void SetData(std::string key, std::string data);
		void SetData(std::string key, int data);
		void SetData(std::string key, float data);
		void SetData(std::string key, Json& data);
		void SetData(std::string data);
		void SetData(int data);
		void SetData(float data);
		void SetData(Json& data);
		std::string Encode(void);
		static std::string DecodeEscape(std::string text);
		static int Int(std::string text);
		static float Float(std::string text);
		static std::string IntText(int data);
		static std::string FloatText(float data);
	private:
		enum JsonType
		{
			TYPE_ARRAY = 0,
			TYPE_OBJECT,
			TYPE_TEXT,
			TYPE_NUMERIC
		};
		struct DataPair
		{
			std::string key;
			std::string data;
			JsonType data_type;
		};
		std::vector<DataPair>::size_type find_index;
		std::vector<DataPair> data_pair_list;
		JsonType json_type;
		int GetKeyIndex(std::string key);
		JsonType GetDataType(char word, bool word_text_flag);
		bool Parse(std::string data);
		static std::string ReplaceAll(std::string text, std::string from, std::string to);
		Json(Json&);
		Json& operator = (Json&);
	};
};

#endif
