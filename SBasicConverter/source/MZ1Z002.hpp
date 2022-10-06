#ifndef MZ1Z002_HPP
#define MZ1Z002_HPP

#include <string>
#include <vector>
#include <map>

class MZ1Z002
{
public:
	struct Lexical
	{
		std::string command;
		std::string option;
	};
	MZ1Z002(void);
	~MZ1Z002(void);
	bool ConvertFile(const std::string filepath, const std::string outputFilepath = "");
	void Load(char* buffer);
private:
	enum
	{
		LINE_END = 0,
		SQUARE_BRACKETS_END,
		DELIMITER_END
	};
	bool GetLine(std::vector<char>& buffer, int& number);
	bool IsExistVariable(std::string command);
	bool IsVariableNameChar(unsigned char, bool first);
	bool IsFixedVariableNameChar(unsigned char byte, bool first);
	bool SetVariableName(std::string& variableName, std::vector<char>& result, bool array, int number);
	bool IsBracketCommand(std::string command);
	void PushBrackets(void);
	void AddBrackets(void);
	bool PopBrackets(void);
	bool IsBrackets(void);
	std::vector<char> PreConvertLine(const std::vector<char>& buffer, int number);
	bool Convert(const std::vector<char>& buffer, int number, int conditions);
	void ConvertSub(const std::vector<char>& buffer, Lexical& lexical, unsigned char byte, bool processedDelimiter, int number, int& subNumber);
	void AnalyzeCommand(Lexical& lexical, int number, int& subNumber, bool delimiter);
	void Delimiter(int number, int& subNumber);
	void AddColor(void);
	std::string Nop(const Lexical& lexical, bool delimiter);
	std::string Rem(const Lexical& lexical, bool delimiter);
	std::string Data(const Lexical& lexical, bool delimiter, int number);
	std::string DefKey(const Lexical& lexical, bool delimiter);
	std::string Read(const Lexical& lexical, bool delimiter);
	void Run(const Lexical& lexical, bool delimiter, int number, int& subNumber);
	std::string Print(const Lexical& lexical, bool delimiter);
	void EndPrint(int number, int& subNumber);
	std::string For(const Lexical& lexical, bool delimiter);
	std::string To(const Lexical& lexical, bool delimiter);
	std::string Step(const Lexical& lexical, bool delimiter);
	void EndFor(int number, int& subNumber);
	void Next(const Lexical& lexical, bool delimiter, int number, int& subNumber);
	std::string If(const Lexical& lexical, bool delimiter);
	void EndIf(int number, int& subNumber);
	std::string Then(const Lexical& lexical, bool delimiter);
	std::string Goto(const Lexical& lexical, bool delimiter);
	std::string Gosub(const Lexical& lexical, bool delimiter);
	std::string Return(const Lexical& lexical, bool delimiter);
	std::string Stop(const Lexical& lexical, bool delimiter);
	std::string End(const Lexical& lexical, bool delimiter);
	std::string On(const Lexical& lexical, bool delimiter);
	std::string Poke(const Lexical& lexical, bool delimiter);
	void Dim(const Lexical& lexical, bool delimiter, int number, int& subNumber);
	void EndDim(int number, int& subNumber);
	std::string DefFn(const Lexical& lexical, bool delimiter);
	void EndDefFn(int number, int& subNumber);
	void Input(const Lexical& lexical, bool delimiter, int number, int& subNumber);
	std::string Restore(const Lexical& lexical, bool delimiter);
	std::string Clr(const Lexical& lexical, bool delimiter);
	std::string Music(const Lexical& lexical, bool delimiter);
	std::string Tempo(const Lexical& lexical, bool delimiter);
	void CloseBracket(int number, int& subNumber);
	std::string Usr(const Lexical& lexical, bool delimiter);
	std::string Wopen(const Lexical& lexical, bool delimiter);
	std::string Ropen(const Lexical& lexical, bool delimiter);
	std::string Close(const Lexical& lexical, bool delimiter);
	std::string Mon(const Lexical& lexical, bool delimiter);
	std::string Limit(const Lexical& lexical, bool delimiter);
	std::string Cont(const Lexical& lexical, bool delimiter);
	std::string Get(const Lexical& lexical, bool delimiter);
	std::string Inp(const Lexical& lexical, bool delimiter);
	std::string Out(const Lexical& lexical, bool delimiter);
	std::string Cursor(const Lexical& lexical, bool delimiter);
	std::string Set(const Lexical& lexical, bool delimiter);
	std::string Reset(const Lexical& lexical, bool delimiter);
	std::string Line(const Lexical& lexical, bool delimiter);
	std::string Bline(const Lexical& lexical, bool delimiter);
	void Console(const Lexical& lexical, bool delimiter, int number, int& subNumber);
	void Graph(const Lexical& lexical, bool delimiter, int number, int& subNumber);
	std::string Position(const Lexical& lexical, bool delimiter);
	std::string Pattern(const Lexical& lexical, bool delimiter);
	std::string Auto(const Lexical& lexical, bool delimiter);
	std::string Image(const Lexical& lexical, bool delimiter);
	std::string Copy(const Lexical& lexical, bool delimiter);
	std::string Page(const Lexical& lexical, bool delimiter);
	std::string Boot(const Lexical& lexical, bool delimiter);
	std::string Klist(const Lexical& lexical, bool delimiter);
	std::string Change(const Lexical& lexical, bool delimiter);
	std::string Rew(const Lexical& lexical, bool delimiter);
	std::string Fast(const Lexical& lexical, bool delimiter);
	std::string BasicFunction(const Lexical& lexical, bool delimiter);
	std::string BasicStringFunction(const Lexical& lexical, bool delimiter);
	std::string BasicSign(const Lexical& lexical, bool delimiter);
	std::string Pi(const Lexical& lexical, bool delimiter);
	std::string Pos(const Lexical& lexical, bool delimiter);
	std::string Color(const Lexical& lexical, bool delimiter);
	std::string CColor(const Lexical& lexical, bool delimiter);
	std::string Circle(const Lexical& lexical, bool delimiter);
	std::string Box(const Lexical& lexical, bool delimiter);
	std::string Paint(const Lexical& lexical, bool delimiter);
	std::vector<std::string> SpritText(std::string text, std::string separator);
	std::string FixCcolorOption(std::string option);
	std::string FixColorOption(std::string option, bool atMark);
	bool CheckEncode(unsigned char byte);
	bool CheckEncodeAfter(unsigned char byte);
	std::string Encode(std::string text);
	std::string Space(void);
	std::string Number(int number, int subNumber);
	std::string Number(std::string number);
	std::string NumberList(std::string number);
	bool CheckReserved(std::string text);
	std::string ReplaceAll(const std::string& text, const std::string& target, const std::string& replacement);
	bool CheckBasicFunction(const std::string& text);
	bool CheckBasicStringFunction(const std::string& text);
	bool CheckBasicSign(const std::string& text);
	std::string ParseData(const std::string& data);
	std::string ChangeVariableArrayName(std::string variableName);
	std::string ChangeStringVariableName(std::string variableName, bool array);
	bool IsStringVariableName(std::string variableName);
	bool IsStringFixedVariableName(std::string variableName);
	std::string EraseSpace(const std::string& text);
	std::string Trim(const std::string& text, const char* trimCharacterList = " \t\v\r\n");
	std::vector<std::string> VariableList(const std::string& text);
	std::vector<std::string> DimVariableList(const std::string& text);
	std::string DeleteQuotationString(std::string& text);
	void MakeVariableList(std::string option);
	void ReplaceNumber(void);
	std::string ReplaceAll(std::string& replacedStr, std::string from, std::string to);
	bool FindVariableList(std::string variableName);
	bool FindDefFnFuncList(std::string variableName);
	std::string FixOption(std::string sourceCommand, std::string sourceOption, bool delimiter);
	std::string FixPrintOption(std::string sourceOption, bool delimiter, bool commaToPlus);
	std::string FixCircleOption(std::string sourceOption);
	std::string FixOptionNumber(std::string sourceOption);
	void DebugLog(std::string text);
	int space;
	std::vector<std::string> variableList;
	std::vector<std::string> dimVariableList;
	std::vector<std::string> deffnFuncList;
	std::map<std::string, std::string> program;
	std::map<std::string, std::string> data;
	std::vector<std::string> grobal;
	std::string result;
	std::string color;
	int ifStackCount;
	bool printFlag;
	bool printSemicolonFlag;
	int forStackCount;
	int forPhase;
	bool onFlag;
	std::string onVariable;
	bool defFnFlag;
	bool defKeyFlag;
	bool patternFlag;
	int patternSeparatorCount;
	bool closeBracesFlag;
	bool closeBracketFlag;
	bool addColorFlag;
	bool circleFlag;
	char* buffer;
	int linePointer;
	std::map<int, std::vector<char>> programBuffer;
	std::vector<int> bracketsCountList;
	size_t convertIndex;
	bool wMode;
	std::string debugLine;
};

#endif
