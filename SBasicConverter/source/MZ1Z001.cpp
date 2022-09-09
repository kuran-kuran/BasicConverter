#include <stdio.h>
#include <string>
#include <regex>
#include <vector>
#include <algorithm>
#include "Format.hpp"
#include "MZ1Z001.hpp"
#include "BasicCode.h"
#include "FileData.hpp"

MZ1Z001::MZ1Z001(void)
:space(4)
,variableList()
,dimVariableList()
,deffnFuncList()
,program()
,data()
,grobal()
,result("")
,ifStackCount(0)
,printFlag(false)
,printSemicolonFlag(false)
,forStackCount(0)
,forPhase(0)
,onFlag(false)
,onVariable()
,defFnFlag(false)
,defKeyFlag(false)
,patternFlag(false)
,patternSeparatorCount(0)
,closeBracesFlag(false)
,closeBracketFlag(false)
,addColorFlag(false)
,buffer(NULL)
,linePointer(128)
,programBuffer()
,bracketsCountList()
,convertIndex(0)
,wMode(1)
,debugLine("")
{
}

MZ1Z001::~MZ1Z001(void)
{
}

// データをバッファに格納
void MZ1Z001::Load(char* buffer)
{
	this->buffer = buffer;
	this->linePointer = 128;
	std::vector<char> lineBuffer;
	int number;
	while(GetLine(lineBuffer, number) == true)
	{
		this->programBuffer[number] = lineBuffer;
	}
}

// 1行分のデータを取得
bool MZ1Z001::GetLine(std::vector<char>& receiveBuffer, int& number)
{
	receiveBuffer.clear();
	number = 0;
	int size = 0;
	if(this->linePointer == -1)
	{
		return false;
	}
	size = *reinterpret_cast<unsigned short*>(&this->buffer[this->linePointer]);
	if(size == 0)
	{
		this->linePointer = -1;
		return false;
	}
	number = *(unsigned short*)(&this->buffer[this->linePointer + 2]);
	const char* lineBuffer = &this->buffer[this->linePointer + 4];
	std::copy(lineBuffer, lineBuffer + size - 4, std::back_inserter(receiveBuffer));
	this->linePointer += size;
	return true;
}

// コンバート
bool MZ1Z001::ConvertFile(const std::string filepath, const std::string outputFilepath)
{
	this->space = 4;
	this->variableList.clear();
	this->program.clear();
	this->data.clear();
	this->result = "";
	FileData fileData;
	if(fileData.Load(filepath) == false)
	{
		return false;
	}
	Load(reinterpret_cast<char*>(fileData.GetBuffer()));
	// 事前データ更新
	for(auto iter = this->programBuffer.begin(); iter != this->programBuffer.end(); ++ iter)
	{
		int lineNumber = iter->first;
		std::vector<char> lineBuffer = iter->second;
		this->programBuffer[lineNumber] = PreConvertLine(lineBuffer, lineNumber);
	}
	// コンバート
	for(auto iter = this->programBuffer.begin(); iter != this->programBuffer.end(); ++ iter)
	{
		int lineNumber = iter->first;
		std::vector<char> lineBuffer = iter->second;
		this->convertIndex = 0;
		Convert(lineBuffer, lineNumber, LINE_END);
	}
	// 使用変数一覧
	for(size_t i = 0; i < this->variableList.size(); ++i)
	{
		DebugLog(Format("%s\n", this->variableList[i].c_str()).c_str());
	}
	// ヘッダファイル出力
	std::string writeHeaderPath = filepath;
	writeHeaderPath.erase(writeHeaderPath.size() - 4, 4);
	writeHeaderPath += ".hpp";
	if(outputFilepath.empty() != true)
	{
		writeHeaderPath = outputFilepath;
		writeHeaderPath.erase(writeHeaderPath.size() - 4, 4);
		writeHeaderPath += ".hpp";
	}
	// Define
	std::string writeData;
	writeData += "#ifndef BASIC_HPP\n";
	writeData += "#define BASIC_HPP\n\n";
	writeData += "// Program line define\n";
	int number = 0;
	for(auto iter = this->program.begin(); iter != this->program.end(); ++ iter)
	{
		std::string numberText = "#define L";
		numberText += iter->first.c_str();
		writeData += numberText;
		writeData += " ";
		writeData += Format("%d", number);
		writeData += "\n";
		++ number;
	}
	writeData += "// Data line define\n";
	for(auto iter = this->data.begin(); iter != this->data.end(); ++ iter)
	{
		std::string numberText = "#define D";
		numberText += iter->first.c_str();
		writeData += numberText;
		writeData += " ";
		writeData += Format("%d", number);
		writeData += "\n";
		++ number;
	}
	writeData += "\n";
	writeData += "#endif\n";
	// ファイル書き込み
	FileData writeHeaderFileData;
	writeHeaderFileData.SetBuffer(&writeData[0], writeData.size());
	writeHeaderFileData.Save(writeHeaderPath);

	// プログラム出力
	std::string writeProgramPath = filepath;
	writeProgramPath.erase(writeProgramPath.size() - 4, 4);
	writeProgramPath += ".cpp";
	if(outputFilepath.empty() != true)
	{
		writeProgramPath = outputFilepath;
	}
	// include
	writeData.clear();
	writeData += "#include \"Command.hpp\"\n";
	writeData += "#include \"" + writeHeaderPath + "\"\n";
	writeData += "\n";
	// グローバル
	writeData += "// Grobal\n";
	writeData += "Executer* executer = Executer::GetInstance();\n";
	writeData += "bool end = false;\n";
	bool strTI = false;
	for(size_t i = 0; i < this->grobal.size(); ++ i)
	{
		writeData += this->grobal[i];
		writeData += "\n";
		if(this->grobal[i].find("strTI") != std::string::npos)
		{
			strTI = true;
		}
	}
	if(strTI == false)
	{
		writeData += "dms::String strTI;\n";
	}
	writeData += "\n";
	// Clr関数
	writeData += "// Initialize variable\n";
	writeData += "void Clr(void)\n{\n";
	for(size_t i = 0; i < this->variableList.size(); ++ i)
	{
		writeData += "\t";
		writeData += this->variableList[i];
		writeData += ".clear();";
		writeData += "\n";
	}
	for(size_t i = 0; i < this->dimVariableList.size(); ++ i)
	{
		writeData += "\t";
		writeData += this->dimVariableList[i];
		writeData += ".clear();";
		writeData += "\n";
	}
	writeData += "}\n";
	writeData += "\n";
	// データ
	writeData += "// Data\n";
	writeData += "void SetupData(void)\n{\n";
	for(auto iter = this->data.begin(); iter != this->data.end(); ++ iter)
	{
		if(iter->second.empty() == true)
		{
			continue;
		}
		writeData += "\t";
		writeData += iter->second;
		writeData += ";\n";
	}
	writeData += "}\n";
	writeData += "\n";
	// プログラム
	writeData += "// Program\n";
	ReplaceNumber();
	for(auto iter = this->program.begin(); iter != this->program.end(); ++ iter)
	{
		std::string numberText = iter->first.c_str();
		std::string programText = iter->second.c_str();
		std::string addText = std::string("void l") + numberText + "() { " + programText + " }";
		writeData += addText;
		writeData += "\n";
	}
	writeData += "\n";
	// 関数配列
	writeData += "std::vector<void (*)(void)> lineList =\n{\n";
	int writeCount = 0;
	for(auto iter = this->program.begin(); iter != this->program.end(); ++ iter)
	{
		std::string numberText = "\tl";
		numberText += iter->first.c_str();
		writeData += numberText;
		writeData += ",";
		++ writeCount;
		if(writeCount == 16)
		{
			writeData += "\n";
			writeCount = 0;
		}
	}
	if(writeCount != 0)
	{
		writeData += "\n";
	}
	writeData += "};\n";
	writeData += "\n";
	// 制御プログラム
	writeData += "\n";
	writeData += "void SetTimeCallback(dms::String* string)\n";
	writeData += "{\n";
	writeData += "\tSetTime(*string);\n";
	writeData += "}\n";
	writeData += "\n";
	writeData += "void MainLoop_Setup(unsigned int* frameBuffer)\n";
	writeData += "{\n";
	writeData += "\texecuter->Initialize(lineList, frameBuffer);\n";
	writeData += "\tSetupData();\n";
	writeData += "\tstrTI.SetStoreCallback(SetTimeCallback);\n";
	writeData += "}\n";
	writeData += "\n";
	writeData += "void MainLoop_Finalize()\n";
	writeData += "{\n";
	writeData += "}\n";
	writeData += "\n";
	writeData += "void MainLoop_Loop()\n";
	writeData += "{\n";
	writeData += "\tif(end == true)\n";
	writeData += "\t{\n";
	writeData += "\t\treturn;\n";
	writeData += "\t}\n";
	writeData += "\tend = executer->ExecuteLoop();\n";
	writeData += "}\n";
	writeData += "\n";
	writeData += "void MainLoop_NoWaitLoop()\n";
	writeData += "{\n";
	writeData += "\texecuter->ExecuteNoWaitLoop();\n";
	writeData += "}\n";
	writeData += "\n";
	writeData += "void MainLoop_Flip()\n";
	writeData += "{\n";
	writeData += "\tstrTI.SetStoreCallback(NULL);\n";
	writeData += "\tstrTI = Time();\n";
	writeData += "\tstrTI.SetStoreCallback(SetTimeCallback);\n";
	writeData += "\texecuter->Flip();\n";
	writeData += "}\n";
	// ファイル書き込み
	FileData writeProgramFileData;
	writeProgramFileData.SetBuffer(&writeData[0], writeData.size());
	writeProgramFileData.Save(writeProgramPath);
	return true;
}

// 後ろに変数が続く可能性のある命令か
bool MZ1Z001::IsExistVariable(std::string command)
{
	std::vector<std::string> commandTable =
	{
		"NOP", "READ", "PRINT", "LET", "FOR", "TO", "STEP", "IF",
		"THEN", "DIM", "DEF FN", "INPUT", "MUSIC", "TEMPO", "USR(", "WOPEN",
		"ROPEN", "LIMIT", "GET", "INP@", "OUT@", "CURSOR", "SET", "RESET",
		"LINE", "BLINE", "POSITION", "PATTERN", "IMAGE/P", "PAGE/P", "π"
	};
	auto iter = std::find(commandTable.begin(), commandTable.end(), command);
	size_t index = std::distance(commandTable.begin(), iter);
	if(index != commandTable.size())
	{
		return true;
	}
	if(CheckBasicFunction(command) == true)
	{
		return true;
	}
	if(CheckBasicStringFunction(command) == true)
	{
		return true;
	}
	if(CheckBasicSign(command) == true)
	{
		return true;
	}
	return false;
}

// 修正前の変数名かどうか(文字列には$がついている)
bool MZ1Z001::IsVariableNameChar(unsigned char byte, bool first)
{
	if(byte >= 'A' && byte <= 'Z')
	{
		return true;
	}
	if(byte == '$')
	{
		return true;
	}
	if(first == false)
	{
		if(byte >= '0' && byte <= '9')
		{
			return true;
		}
	}
	return false;
}

// 修正後の変数名かどうか(文字列には$のかわりにstrが付いている)
bool MZ1Z001::IsFixedVariableNameChar(unsigned char byte, bool first)
{
	if(byte >= 'A' && byte <= 'Z')
	{
		return true;
	}
	if((byte == 's') || (byte == 't') || (byte == 'r'))
	{
		return true;
	}
	if(first == false)
	{
		if(byte >= '0' && byte <= '9')
		{
			return true;
		}
	}
	return false;
}

// 変数を登録する、変数名を変更する
// 登録出来たらtrueを返す
bool MZ1Z001::SetVariableName(std::string& variableName, std::vector<char>& result, bool array, int number)
{
	// 空なら何もしない
	if(variableName.empty() == true)
	{
		return false;
	}
	if(variableName[0] == '$')
	{
		variableName.clear();
		return false;
	}
	bool string = false;
	std::string originalVariableName = variableName;
	if(IsStringVariableName(originalVariableName) == true)
	{
		// 文字列変数または文字列配列だったら変数名を修正する
		variableName = ChangeStringVariableName(originalVariableName, array);
		// result内の変数名を修正する
		result.erase(result.end() - originalVariableName.size(), result.end());
		for(size_t i = 0; i < variableName.size(); ++ i)
		{
			result.push_back(variableName[i]);
		}
		string = true;
	}
	else if(array == true)
	{
		// 配列だったら変数名を修正する
		variableName = ChangeVariableArrayName(originalVariableName);
		// result内の変数名を修正する
		result.erase(result.end() - originalVariableName.size(), result.end());
		for(size_t i = 0; i < variableName.size(); ++ i)
		{
			result.push_back(variableName[i]);
		}
	}
	// DEF FN関数かどうか
	bool defFn = false;
	if(originalVariableName.substr(0,2) == "FN")
	{
		defFn = true;
	}
	// 配列変数は追加しない
	if((array == true) || (defFn == true))
	{
		variableName.clear();
		return true;
	}
	bool find = false;
	if(this->variableList.empty() == false)
	{
		for(size_t i = 0; i < this->variableList.size(); ++ i)
		{
			if(this->variableList[i] == variableName)
			{
				find = true;
				break;
			}
		}
	}
	if(find == false)
	{
		this->variableList.push_back(variableName);
		if(array == false)
		{
			if(string == true)
			{
				this->grobal.push_back("dms::String " + variableName + ";");
			}
			else
			{
				this->grobal.push_back("dms::Variable " + variableName + ";");
			}
		}
	}
	variableName.clear();
	return true;
}

bool MZ1Z001::IsBracketCommand(std::string command)
{
	if(command.find("(") == std::string::npos)
	{
		return false;
	}
	return true;
}

// (カッコの始まりを登録する
void MZ1Z001::PushBrackets(void)
{
	this->bracketsCountList.push_back(0);
	for(size_t i = 0; i < this->bracketsCountList.size(); ++ i)
	{
		++ this->bracketsCountList[i];
	}
}

// (カッコを見つけたらインクリメントする
void MZ1Z001::AddBrackets(void)
{
	for(size_t i = 0; i < this->bracketsCountList.size(); ++ i)
	{
		++ this->bracketsCountList[i];
	}
}

// )カッコを見つけたらデクリメントして0ならtrueを返す
// trueの場合は登録したカッコ(に対応した)ガッコが見つかった
bool MZ1Z001::PopBrackets(void)
{
	for(size_t i = 0; i < this->bracketsCountList.size(); ++ i)
	{
		-- this->bracketsCountList[i];
		if(this->bracketsCountList[i] == 0)
		{
			this->bracketsCountList[i] = -999;
			return true;
		}
	}
	return false;
}

// []カッコ処理中か
bool MZ1Z001::IsBrackets(void)
{
	for(size_t i = 0; i < this->bracketsCountList.size(); ++ i)
	{
		if(this->bracketsCountList[i] > 0)
		{
			return true;
		}
	}
	return false;
}

// 配列のカッコを()から[]に変換する
// 文字列変数名を$マーク付きからstr付きに修正する
std::vector<char> MZ1Z001::PreConvertLine(const std::vector<char>& buffer, int number)
{
	std::vector<char> result;
	std::string variable;
	bool searchVariable = true;
	int phase = 1;	// 0=direct, 1=code, 2=code80, 3=codeB2
	bool defFn = false;
	bool pattern = false;
	bool remFlag = false;
	bool dataFlag = false;
	int patternSeparatorCount = 0;
	int patternBracketsCount = 0;
	this->bracketsCountList.clear();
	int bracketsCount = 0;
	unsigned char beforeByte = 0;
	bool first = true;
	bool firstVariable = false;
	bool defKey = false;
	unsigned char* buf = (unsigned char*)&buffer[0];
	if(number == 9140)
	{
		int a = 0;
	}
	for(size_t i = 0; i < buffer.size(); ++ i)
	{
		unsigned char byte;
		byte = static_cast<unsigned char>(buffer[i]);
		if(byte == 0x0D)
		{
			if(phase == 0)
			{
				// 改行前に"が閉じられてないときは"を足す
				result.push_back('\"');
				if(dataFlag == false)
				{
					result.push_back('_');
					result.push_back('s');
				}
			}
			SetVariableName(variable, result, false, number);
			result.push_back(byte);
			dataFlag = false;
			remFlag = false;
			defKey = false;
			break;
		}
		if((phase == 1) && (byte == ':'))
		{
			SetVariableName(variable, result, false, number);
			result.push_back(byte);
			searchVariable = true;
			defFn = false;
			pattern = false;
			firstVariable = false;
			first = true;
			continue;
		}
		if((phase == 1) && (byte == ';'))
		{
			if(SetVariableName(variable, result, false, number) == true)
			{
				firstVariable = first;
				first = false;
			}
			result.push_back(byte);
			searchVariable = true;
			continue;
		}
		if((phase == 1) && (byte == '['))
		{
			// 色指定は判別のために[]から{}に変えておく
			result.push_back('{');
		}
		else if((phase == 1) && (byte == ']'))
		{
			// 色指定は判別のために[]から{}に変えておく
			result.push_back('}');
		}
		else if((phase == 1) && (byte == 0x80) && (remFlag == false) && (dataFlag == false))
		{
			if(SetVariableName(variable, result, false, number) == true)
			{
				firstVariable = first;
				first = false;
			}
			result.push_back(byte);
			phase = 2;
		}
		else if((phase == 1) && (byte == 0xB2) && (remFlag == false) && (dataFlag == false))
		{
			if(SetVariableName(variable, result, false, number) == true)
			{
				firstVariable = first;
				first = false;
			}
			result.push_back(byte);
			phase = 3;
		}
		else if(byte == '\"')
		{
			if (phase == 0)
			{
				if(SetVariableName(variable, result, false, number) == true)
				{
					firstVariable = first;
					first = false;
				}
				result.push_back(byte);
				if(dataFlag == false)
				{
					result.push_back('_');
					result.push_back('s');
				}
				phase = 1;
			}
			else
			{
				if(SetVariableName(variable, result, false, number) == true)
				{
					firstVariable = first;
					first = false;
				}
				result.push_back(byte);
				phase = 0;
			}
		}
		else
		{
			switch(phase)
			{
			case 0:
				if(SetVariableName(variable, result, false, number) == true)
				{
					firstVariable = first;
					first = false;
				}
				result.push_back(byte);
				break;
			case 1:
				if((code_xx[byte] != 0) && (remFlag == false) && (dataFlag == false))
				{
					if(SetVariableName(variable, result, false, number) == true)
					{
						firstVariable = first;
					}
					first = false;
					searchVariable = IsExistVariable(code_xx[byte]);
					if(IsBracketCommand(code_xx[byte]) == true)
					{
						AddBrackets();
						++ bracketsCount;
					}
					result.push_back(byte);
				}
				else
				{
					if(searchVariable == true)
					{
						if((IsVariableNameChar(byte, variable.empty()) == true) && (defFn == false))
						{
							variable += Format("%c", byte);
						}
						else if((variable.empty() == false) && (variable.substr(0,2) != "FN"))
						{
							bool array = false;
							if(byte == '(')
							{
								array = true;
							}
							if(SetVariableName(variable, result, array, number) == true)
							{
								if(bracketsCount <= 0)
								{
									firstVariable = first;
									first = false;
								}
							}
							if(byte == '(')
							{
								byte = '[';
								PushBrackets();
								++ bracketsCount;
							}
						}
					}
					if(byte == ')')
					{
						if(PopBrackets() == true)
						{
							byte = ']';
						}
						-- bracketsCount;
					}
					// 配列アクセス[#,#]を[#][#]に変換する
					if((IsBrackets() == true) && (byte == ','))
					{
						result.push_back(']');
						result.push_back('[');
						break;
					}
					if((pattern == true) && (byte == ',') && (patternBracketsCount == bracketsCount))
					{
						std::string line(buffer.begin(), buffer.end());
						++ patternSeparatorCount;
						if(patternSeparatorCount % 2 == 0)
						{
							result.push_back(':');
							result.push_back('\x80');
							result.push_back('\xB2');
							byte = ' ';
						}
					}
					if(byte == '=')
					{
						if((byte == '=') && (defKey == false) && ((firstVariable == false) || (bracketsCount > 0)))
						{
							result.push_back('=');
						}
						if(defFn == true)
						{
							firstVariable = false;
						}
					}
					result.push_back(byte);
				}
				break;
			case 2:
				if(code_80_xx[byte] != 0)
				{
					if(code_80_xx[byte] == "REM")
					{
						remFlag = true;
					}
					if(code_80_xx[byte] == "DEF FN")
					{
						defFn = true;
					}
					if(code_80_xx[byte] == "DATA")
					{
						dataFlag = true;
					}
					if(code_80_xx[byte] == "PATTERN")
					{
						pattern = true;
						patternSeparatorCount = 0;
						patternBracketsCount = bracketsCount;
					}
					if(code_80_xx[byte] == "DEF KEY(")
					{
						defKey = true;
					}
					if(SetVariableName(variable, result, false, number) == true)
					{
						firstVariable = first;
					}
					first = false;
					if(code_80_xx[byte] == "FOR")
					{
						firstVariable = false;
						first = true;
					}
					if(code_80_xx[byte] == "THEN")
					{
						firstVariable = false;
						first = true;
					}
					if(code_80_xx[byte] == "LET")
					{
						firstVariable = false;
						first = true;
					}
					if(code_80_xx[byte] == "DEF FN")
					{
						firstVariable = true;
						first = false;
					}
					searchVariable = IsExistVariable(code_80_xx[byte]);
					if(IsBracketCommand(code_80_xx[byte]) == true)
					{
						AddBrackets();
						++ bracketsCount;
					}
					result.push_back(byte);
				}
				else
				{
					if(SetVariableName(variable, result, false, number) == true)
					{
						firstVariable = first;
						first = false;
					}
					result.push_back(byte);
				}
				phase = 1;
				break;
			case 3:
				if(code_B2_xx[byte] != 0)
				{
					if(SetVariableName(variable, result, false, number) == true)
					{
						firstVariable = first;
					}
					first = false;
					searchVariable = IsExistVariable(code_B2_xx[byte]);
					if(IsBracketCommand(code_B2_xx[byte]) == true)
					{
						AddBrackets();
						++ bracketsCount;
					}
					result.push_back(byte);
				}
				else
				{
					if(SetVariableName(variable, result, false, number) == true)
					{
						firstVariable = first;
						first = false;
					}
					result.push_back(byte);
				}
				phase = 1;
				break;
			}
		}
	}
	return result;
}

// conditions
// LINE_END: 行が終わるまで
// SQUARE_BRACKETS_END: ]が検出されるまで
// DELIMITER_END: :か行の最後まで
bool MZ1Z001::Convert(const std::vector<char>& buffer, int number, int conditions)
{
	this->space = 4;
	Lexical lexical = { "NOP", "" };
	int phase = 1;	// 0=direct, 1=code, 2=code80, 3=codeB2
	int subNumber = 1;
	if(conditions == LINE_END)
	{
		this->debugLine = Format("%5u ", number);
	}
	this->printFlag = false;
	this->forPhase = 0;
	this->defFnFlag = false;
	this->closeBracketFlag = false;
	if(number == 9005)
	{
		int a = 0;
	}
	bool encodeAfter = false;
	bool remFlag = false;
	bool processedDelimiter = false;
	bool dataFlag = false;
	while(this->convertIndex < buffer.size())
	{
		size_t i = this->convertIndex;
		unsigned char byte;
		byte = static_cast<unsigned char>(buffer[i]);
		bool last = false;
		if(i + 1 < buffer.size() && ((buffer[i + 1] == 0x0D) || (buffer[i + 1] == ':')))
		{
			last = true;
		}
		if(byte == 0x0D)
		{
			AnalyzeCommand(lexical, number, subNumber, true);
			if(conditions == DELIMITER_END)
			{
				break;
			}
			Delimiter(number, subNumber);
			processedDelimiter = true;
			remFlag = false;
			lexical = {"NOP", ""};
			this->defKeyFlag = false;
			break;
		}
		if(((phase == 1) || (this->defKeyFlag == true)) && (byte == ':'))
		{
			AnalyzeCommand(lexical, number, subNumber, true);
			this->debugLine += Format("%c", byte);
			if(conditions == DELIMITER_END)
			{
				++ this->convertIndex;
				break;
			}
			Delimiter(number, subNumber);
			processedDelimiter = true;
			lexical = {"NOP", ""};
			this->defKeyFlag = false;
			this->patternFlag = false;
			this->patternSeparatorCount = 0;
			++ this->convertIndex;
			continue;
		}
		if((phase == 1) && (byte == '{'))
		{
			this->debugLine += Format("%c", byte);
			this->wMode = 0;
			this->color = "";
			++ this->convertIndex;
			Convert(buffer, number, SQUARE_BRACKETS_END);
			continue;
		}
		else if((conditions == SQUARE_BRACKETS_END) && (phase == 1) && (byte == '}'))
		{
			this->debugLine += Format("%c", byte);
			this->wMode = 0;
			AnalyzeCommand(lexical, number, subNumber, false);
			this->color = this->result;
			this->result = "";
			++ this->convertIndex;
			break;
		}
		if((phase == 1) && (byte == ';') && (this->printFlag == true) && (last == false))
		{
			byte = '+';
		}
		if((phase == 1) && (byte == 0x80) && (remFlag == false) && (dataFlag == false))
		{
			phase = 2;
		}
		else if((phase == 1) && (byte == 0xB2) && (remFlag == false) && (dataFlag == false))
		{
			phase = 3;
		}
		else if(byte == '\"')
		{
			if (phase == 0)
			{
				phase = 1;
			}
			else
			{
				phase = 0;
			}
			this->debugLine += Format("%c", byte);
			lexical.option += Format("%c", byte);
		}
		else
		{
			int execPhase = phase;
			if(this->defKeyFlag == true)
			{
				execPhase = 0;
			}
			switch(execPhase)
			{
			case 0:
				if(CheckEncode(byte) == true)
				{
					this->debugLine += Format("\\x%02X", byte);
					lexical.option += Format("\\x%02X", byte);
					encodeAfter = true;
				}
				else
				{
					if((encodeAfter == true) && (CheckEncodeAfter(byte) == true))
					{
						this->debugLine += Format("\\x%02X", byte);
						lexical.option += Format("\\x%02X", byte);
					}
					else
					{
						this->debugLine += Format("%c", byte);
						lexical.option += Format("%c", byte);
					}
					encodeAfter = false;
				}
				break;
			case 1:
				if((code_xx[byte] != 0) && (remFlag == false) && (dataFlag == false))
				{
					AnalyzeCommand(lexical, number, subNumber, false);
					lexical = {code_xx[byte], ""};
					this->debugLine += Format("%s", code_xx[byte]);
					encodeAfter = false;
				}
				else
				{
					this->debugLine += Format("%c", byte);
					lexical.option += Format("%c", byte);
					encodeAfter = false;
				}
				break;
			case 2:
				if(code_80_xx[byte] != 0)
				{
					if(code_80_xx[byte] == "REM")
					{
						remFlag = true;
					}
					if(code_80_xx[byte] == "DEF KEY(")
					{
						this->defKeyFlag = true;
					}
					if(code_80_xx[byte] == "DATA")
					{
						dataFlag = true;
					}
					if(code_80_xx[byte] == "COLOR")
					{
						ConvertSub(buffer, lexical, byte, processedDelimiter, number, subNumber);
						phase = 1;
						break;
					}
					else if(code_80_xx[byte] == "CCOLOR")
					{
						ConvertSub(buffer, lexical, byte, processedDelimiter, number, subNumber);
						phase = 1;
						break;
					}
					else if(code_80_xx[byte] == "GRAPH")
					{
						ConvertSub(buffer, lexical, byte, processedDelimiter, number, subNumber);
						phase = 1;
						break;
					}
					else if(code_80_xx[byte] == "CIRCLE")
					{
						this->circleFlag = true;
						ConvertSub(buffer, lexical, byte, processedDelimiter, number, subNumber);
						this->circleFlag = false;
						phase = 1;
						break;
					}
					else if(code_80_xx[byte] == "PAINT")
					{
						ConvertSub(buffer, lexical, byte, processedDelimiter, number, subNumber);
						phase = 1;
						break;
					}
					else if(code_80_xx[byte] == "BOX")
					{
						ConvertSub(buffer, lexical, byte, processedDelimiter, number, subNumber);
						phase = 1;
						break;
					}
					AnalyzeCommand(lexical, number, subNumber, false);
					lexical = {code_80_xx[byte], ""};
					this->debugLine += Format("%s", code_80_xx[byte]);
					encodeAfter = false;
				}
				else
				{
					this->debugLine += Format("%c", byte);
					lexical.option += Format("%c", byte);
					encodeAfter = false;
				}
				phase = 1;
				break;
			case 3:
				if(code_B2_xx[byte] != 0)
				{
					AnalyzeCommand(lexical, number, subNumber, false);
					lexical = {code_B2_xx[byte], ""};
					this->debugLine += Format("%s", code_B2_xx[byte]);
					encodeAfter = false;
				}
				else
				{
					this->debugLine += Format("%c", byte);
					lexical.option += Format("%c", byte);
					encodeAfter = false;
				}
				phase = 1;
				break;
			}
		}
		++ this->convertIndex;
	}
	if((conditions == LINE_END) && (processedDelimiter == false))
	{
		Delimiter(number, subNumber);
	}
	if(conditions == LINE_END)
	{
		DebugLog(Format("%s\n", this->debugLine.c_str()).c_str());
		this->debugLine = "";
	}
	return true;
}

void MZ1Z001::ConvertSub(const std::vector<char>& buffer, Lexical& lexical, unsigned char byte, bool processedDelimiter, int number, int& subNumber)
{
	this->debugLine += Format("%s", code_80_xx[byte]);
	++ this->convertIndex;
	Convert(buffer, number, DELIMITER_END);
	lexical = {code_80_xx[byte], this->result};
	this->result = "";
	AnalyzeCommand(lexical, number, subNumber, false);
	Delimiter(number, subNumber);
	processedDelimiter = true;
	lexical = {"NOP", ""};
	-- this->convertIndex;
}

void MZ1Z001::AnalyzeCommand(Lexical& lexical, int number, int& subNumber, bool delimiter)
{
	if (lexical.command == "NOP" && lexical.option == "")
	{
		return;
	}
	// オプションを修正する
	lexical.option = FixOption(lexical.command, lexical.option, delimiter);
	// 変数リスト作成
	if(lexical.command == "NOP")
	{
		this->result += Nop(lexical, delimiter);
	}
	else if (lexical.command == "REM")
	{
		this->result += Rem(lexical, delimiter);
	}
	else if (lexical.command == "DATA")
	{
		this->result += Data(lexical, delimiter, number);
	}
	else if (lexical.command == "DEF KEY(")
	{
		this->result += DefKey(lexical, delimiter);
	}
	else if (lexical.command == "READ")
	{
		this->result += Read(lexical, delimiter);
	}
	else if (lexical.command == "RUN")
	{
		Run(lexical, delimiter, number, subNumber);
		return;
	}
	else if (lexical.command == "PRINT")
	{
		this->result += Print(lexical, delimiter);
	}
	else if (lexical.command == "LET")
	{
		this->result += Nop(lexical, delimiter);
	}
	else if (lexical.command == "FOR")
	{
		this->result += For(lexical, delimiter);
	}
	else if (lexical.command == "TO")
	{
		this->result += To(lexical, delimiter);
	}
	else if (lexical.command == "STEP")
	{
		this->result += Step(lexical, delimiter);
	}
	else if (lexical.command == "NEXT")
	{
		Next(lexical, delimiter, number, subNumber);
		return;
	}
	else if (lexical.command == "IF")
	{
		this->result += If(lexical, delimiter);
	}
	else if(lexical.command == "THEN")
	{
		if(this->ifStackCount > 0)
		{
			EndIf(number, subNumber);
		}
		this->result = Then(lexical, delimiter);
	}
	else if(lexical.command == "GOTO")
	{
		if(this->ifStackCount > 0)
		{
			EndIf(number, subNumber);
		}
		this->result += Goto(lexical, delimiter);
	}
	else if(lexical.command == "GOSUB")
	{
		if(this->ifStackCount > 0)
		{
			EndIf(number, subNumber);
		}
		this->result += Gosub(lexical, delimiter);
	}
	else if(lexical.command == "RETURN")
	{
		this->result += Return(lexical, delimiter);
	}
	else if (lexical.command == "STOP")
	{
		this->result += Stop(lexical, delimiter);
	}
	else if (lexical.command == "END")
	{
		this->result += End(lexical, delimiter);
	}
	else if (lexical.command == "ON")
	{
		this->result += On(lexical, delimiter);
	}
	else if (lexical.command == "POKE")
	{
		this->result += Poke(lexical, delimiter);
	}
	else if (lexical.command == "DIM")
	{
		Dim(lexical, delimiter, number, subNumber);
		return;
	}
	else if (lexical.command == "DEF FN")
	{
		this->result += DefFn(lexical, delimiter);
	}
	else if (lexical.command == "INPUT")
	{
		Input(lexical, delimiter, number, subNumber);
		return;
	}
	else if (lexical.command == "RESTORE")
	{
		this->result += Restore(lexical, delimiter);
	}
	else if (lexical.command == "CLR")
	{
		this->result += Clr(lexical, delimiter);
	}
	else if (lexical.command == "MUSIC")
	{
		this->result += Music(lexical, delimiter);
	}
	else if (lexical.command == "TEMPO")
	{
		this->result += Tempo(lexical, delimiter);
	}
	else if (lexical.command == "USR(")
	{
		this->result += Usr(lexical, delimiter);
	}
	else if (lexical.command == "WOPEN")
	{
		this->result += Wopen(lexical, delimiter);
	}
	else if (lexical.command == "ROPEN")
	{
		this->result += Ropen(lexical, delimiter);
	}
	else if (lexical.command == "CLOSE")
	{
		this->result += Close(lexical, delimiter);
	}
	else if (lexical.command == "MON")
	{
		this->result += Mon(lexical, delimiter);
	}
	else if (lexical.command == "LIMIT")
	{
		this->result += Limit(lexical, delimiter);
	}
	else if (lexical.command == "CONT")
	{
		this->result += Cont(lexical, delimiter);
	}
	else if (lexical.command == "GET")
	{
		this->result += Get(lexical, delimiter);
	}
	else if (lexical.command == "INP@")
	{
		this->result += Inp(lexical, delimiter);
	}
	else if (lexical.command == "OUT@")
	{
		this->result += Out(lexical, delimiter);
	}
	else if (lexical.command == "CURSOR")
	{
		this->result += Cursor(lexical, delimiter);
	}
	else if (lexical.command == "SET")
	{
		this->result += Set(lexical, delimiter);
	}
	else if (lexical.command == "RESET")
	{
		this->result += Reset(lexical, delimiter);
	}
	else if (lexical.command == "LINE")
	{
		this->result += Line(lexical, delimiter);
	}
	else if (lexical.command == "BLINE")
	{
		this->result += Bline(lexical, delimiter);
	}
	else if (lexical.command == "CONSOLE")
	{
		Console(lexical, delimiter, number, subNumber);
		return;
	}
	else if (lexical.command == "GRAPH")
	{
		Graph(lexical, delimiter, number, subNumber);
		return;
	}
	else if (lexical.command == "POSITION")
	{
		this->result += Position(lexical, delimiter);
	}
	else if (lexical.command == "PATTERN")
	{
		this->result += Pattern(lexical, delimiter);
	}
	else if (lexical.command == "AUTO")
	{
		this->result += Auto(lexical, delimiter);
	}
	else if (lexical.command == "IMAGE/P")
	{
		this->result += Image(lexical, delimiter);
	}
	else if (lexical.command == "COPY/P")
	{
		this->result += Copy(lexical, delimiter);
	}
	else if (lexical.command == "PAGE/P")
	{
		this->result += Page(lexical, delimiter);
	}
	else if (lexical.command == "BOOT")
	{
		this->result += Boot(lexical, delimiter);
	}
	else if (lexical.command == "KLIST")
	{
		this->result += Klist(lexical, delimiter);
	}
	else if (lexical.command == "CHANGE")
	{
		this->result += Change(lexical, delimiter);
	}
	else if (lexical.command == "REW")
	{
		this->result += Rew(lexical, delimiter);
	}
	else if (lexical.command == "FAST")
	{
		this->result += Fast(lexical, delimiter);
	}
	else if(CheckBasicFunction(lexical.command) == true)
	{
		this->result += BasicFunction(lexical, delimiter);
	}
	else if(CheckBasicStringFunction(lexical.command) == true)
	{
		this->result += BasicStringFunction(lexical, delimiter);
	}
	else if(CheckBasicSign(lexical.command) == true)
	{
		this->result += BasicSign(lexical, delimiter);
	}
	else if(lexical.command == "π")
	{
		this->result += Pi(lexical, delimiter);
	}
	else if(lexical.command == "POS")
	{
		this->result += Pos(lexical, delimiter);
	}
	else if(lexical.command == "COLOR")
	{
		this->result += Color(lexical, delimiter);
	}
	else if(lexical.command == "CCOLOR")
	{
		this->result += CColor(lexical, delimiter);
	}
	else if(lexical.command == "CIRCLE")
	{
		this->result += Circle(lexical, delimiter);
	}
	else if(lexical.command == "BOX")
	{
		this->result += Box(lexical, delimiter);
	}
	else if(lexical.command == "PAINT")
	{
		this->result += Paint(lexical, delimiter);
	}
	//else
#if false
	{
		this->result += "\t\t/* ";
		for (size_t i = 0; i < lexicalList.size(); ++i)
		{
			this->result += Format("([%s] [%s]) ", lexicalList[i].command.c_str(), lexicalList[i].option.c_str());
		}
		if (delimiter == true)
		{
			this->result += ":";
		}
		this->result += " */";
	}
#endif
}

void MZ1Z001::Delimiter(int number, int& subNumber)
{
	if(this->closeBracesFlag == true)
	{
		this->result += "}";
		this->closeBracesFlag = false;
	}
	if(this->addColorFlag == true)
	{
		AddColor();
	}
	if(this->printFlag == true)
	{
		EndPrint(number, subNumber);
	}
	else if(this->forPhase > 0)
	{
		EndFor(number, subNumber);
	}
	else if(this->defFnFlag == true)
	{
		EndDefFn(number, subNumber);
	}
	else if(this->closeBracketFlag == true)
	{
		CloseBracket(number, subNumber);
	}
	else
	{
		this->program[Number(number, subNumber)] = this->result + ";";
		this->result = "";
		++ subNumber;
	}
}

void MZ1Z001::AddColor(void)
{
	if(this->color.empty() == false)
	{
		this->result += ",";
		// 重ね合わせからWを取る
		std::string colorText = FixColorOption(this->color, false);
		this->result += colorText;
	}
	this->color = "";
	this->addColorFlag = false;
}

std::string MZ1Z001::Nop(const Lexical& lexical, bool delimiter)
{
	std::string text = lexical.option;
	if(text.empty() == true)
	{
		return "";
	}
	std::string result = FixOptionNumber(lexical.option);
	return result;
}

std::string MZ1Z001::Rem(const Lexical& lexical, bool delimiter)
{
	std::string comment = Encode(lexical.option);
	std::string result = "/* " + comment + " */";
	return result;
}

std::string MZ1Z001::Data(const Lexical& lexical, bool delimiter, int number)
{
	std::string data = ParseData(lexical.option);
	std::string result = "SetData(D" + Number(number, 1) + ", " + data + ")";
	this->data[Number(number, 1)] = result;
	return "";
}

std::string MZ1Z001::DefKey(const Lexical& lexical, bool delimiter)
{
	std::string result = "DefKey(";
	std::string option = Trim(lexical.option);
	size_t pos = option.find_first_of(')');
	option = option.replace(pos, 1, ",");
	pos = option.find_first_of('=');
	option = option.replace(pos, 1, "\"");
	option += "\"";
	result += option.substr(0, pos + 1);
	bool encodeAfter = false;
	for(size_t i = pos + 1; i < option.size(); ++ i )
	{
		if((CheckEncode(option[i]) == true) && (option[i] != '\\'))
		{
			result += Format("\\x%02X", static_cast<unsigned char>(option[i]));
			encodeAfter = true;
		}
		else
		{
			if((encodeAfter == true) && (CheckEncodeAfter(option[i]) == true))
			{
				result += Format("\\x%02X", static_cast<unsigned char>(option[i]));
			}
			else
			{
				result += option[i];
			}
			encodeAfter = false;
		}
	}
	result += ")";
	return result;
}

std::string MZ1Z001::Read(const Lexical& lexical, bool delimiter)
{
	// Readする変数リスト作成
	std::vector<std::string> variableList = VariableList(lexical.option);
	std::string result = "";
	for(size_t i = 0; i < variableList.size(); ++ i)
	{
		std::string str = "str";
		if((variableList[i].size() > str.size()) && std::equal(std::begin(str), std::end(str), std::begin(variableList[i])))
		{
			std::string variableName = ChangeStringVariableName(variableList[i], false);
			result += (variableName + "=ReadString()");
		}
		else
		{
			result += (variableList[i] + "=ReadVariable()");
		}
		if(i < variableList.size() - 1)
		{
			result += "; ";
		}
	}
	return result;
}

void MZ1Z001::Run(const Lexical& lexical, bool delimiter, int number, int& subNumber)
{
	this->program[Number(number, subNumber)] = "Clr();";
	++ subNumber;
	this->program[Number(number, subNumber)] = "Run();";
	++ subNumber;
	this->result = "";
}

std::string MZ1Z001::Print(const Lexical& lexical, bool delimiter)
{
	this->printFlag = true;
	this->printSemicolonFlag = false;
	std::string result;
	std::string option = lexical.option;
	if((delimiter == true) && (option.empty() == true))
	{
		option = "\"\"";
	}
	option = FixPrintOption(option, delimiter, true);
	if((option[0] == '\"') && (option[option.size() - 1] == '\"'))
	{
		result = "Print(" + option + "_s";
	}
	else
	{
		result = "Print(" + option;
	}
	if((option.empty() == false) && (option[option.size() - 1] == ';'))
	{
		this->printSemicolonFlag = true;
	}
	return result;
}

void MZ1Z001::EndPrint(int number, int& subNumber)
{
	// Print文中の最後の;は;を消してフラグを立てる
	if((this->result.empty() == false) && (this->result[this->result.size() - 1] == ';'))
	{
		this->result = this->result.substr(0, this->result.size() - 1);
		this->printSemicolonFlag = true;
	}
	if(this->printSemicolonFlag == true)
	{
		this->result += ", false);";
		this->program[Number(number, subNumber)] = this->result;
	}
	else
	{
		this->result += ", true);";
		this->program[Number(number, subNumber)] = this->result;
	}
	++ subNumber;
	this->result = "";
	this->printFlag = false;
	this->printSemicolonFlag = false;
}

std::string MZ1Z001::For(const Lexical& lexical, bool delimiter)
{
	++ this->forStackCount;
	this->forPhase = 1;
	std::string result = "For(";
	std::string option = FixOptionNumber(lexical.option);
	size_t pos = option.find("=", 0);
	if(pos != std::string::npos)
	{
		std::string variable = option.substr(0, pos);
		variable = Trim(variable);
		result += ("&" + variable + ", ");
		option = option.substr(pos + 1);
		option = Trim(option);
		result += option;
	}
	return result;
}

std::string MZ1Z001::To(const Lexical& lexical, bool delimiter)
{
	this->forPhase = 2;
	std::string result = ", ";
	result += Nop(lexical, delimiter);
	return result;
}

std::string MZ1Z001::Step(const Lexical& lexical, bool delimiter)
{
	this->forPhase = 3;
	std::string result = ", ";
	result += Nop(lexical, delimiter);
	return result;
}

void MZ1Z001::EndFor(int number, int& subNumber)
{
	if(this->forPhase == 1)
	{
		// エラー
	}
	else if(this->forPhase == 2)
	{
		// to
		this->result += ", 1);";
		this->program[Number(number, subNumber)] = this->result;
	}
	else if(this->forPhase == 3)
	{
		// step
		this->result += ");";
		this->program[Number(number, subNumber)] = this->result;
	}
	++ subNumber;
	this->result = "";
	this->forPhase = 0;
}

void MZ1Z001::Next(const Lexical& lexical, bool delimiter, int number, int& subNumber)
{
	std::vector<std::string> variableList = VariableList(lexical.option);
	if(variableList.size() == 0)
	{
		std::string result = "Next(NULL);";
		this->program[Number(number, subNumber)] = result;
		++ subNumber;
	}
	else
	{
		for(size_t i = 0; i < variableList.size(); ++ i)
		{
			std::string result = "Next(&" + variableList[i] + ");";
			this->program[Number(number, subNumber)] = result;
			++ subNumber;
		}
	}
}

std::string MZ1Z001::If(const Lexical& lexical, bool delimiter)
{
	++ this->ifStackCount;
	std::string result = "If(";
	std::string option = FixOptionNumber(lexical.option);
	std::string::size_type pos = 0;
	result += Trim(option);
	return result;
}

void MZ1Z001::EndIf(int number, int& subNumber)
{
	this->result += ", {L_NEXT_COMMAND}, {L_NEXT_LINE});";
	this->program[Number(number, subNumber)] = this->result;
	++ subNumber;
	this->result = "";
	-- this->ifStackCount;
}

std::string MZ1Z001::Then(const Lexical& lexical, bool delimiter)
{
	std::string result = "";
	std::string option = Trim(lexical.option);
	if(option.empty() == true)
	{
		return result;
	}
	if(std::all_of(option.cbegin(), option.cend(), isdigit) == true)
	{
		result += ("Goto(L" + Number(option) + ")");
	}
	else
	{
		result += FixOptionNumber(lexical.option);
	}
	return result;
}

std::string MZ1Z001::Goto(const Lexical& lexical, bool delimiter)
{
	std::string result;
	if(this->onFlag == true)
	{
		result = "OnGoto(" + this->onVariable + ", ";
		result += NumberList(lexical.option);
		result += ")";
		this->onFlag = false;
	}
	else
	{
		result = "Goto(L" + Number(lexical.option) + ")";
	}
	return result;
}

std::string MZ1Z001::Gosub(const Lexical& lexical, bool delimiter)
{
	std::string result;
	if(this->onFlag == true)
	{
		result = "OnGosub(" + this->onVariable + ", ";
		result += NumberList(lexical.option);
		result += ")";
		this->onFlag = false;
	}
	else
	{
		result = "Gosub(L" + Number(lexical.option) + ")";
	}
	return result;
}

std::string MZ1Z001::Return(const Lexical& lexical, bool delimiter)
{
	std::string result = "Return()";
	return result;
}

std::string MZ1Z001::Stop(const Lexical& lexical, bool delimiter)
{
	std::string result = "Stop()";
	return result;
}

std::string MZ1Z001::End(const Lexical& lexical, bool delimiter)
{
	std::string result = "End()";
	return result;
}

std::string MZ1Z001::On(const Lexical& lexical, bool delimiter)
{
	this->onFlag = true;
	this->onVariable = FixOptionNumber(lexical.option);
	return "";
}

std::string MZ1Z001::Poke(const Lexical& lexical, bool delimiter)
{
	std::string result = "Poke(" + FixOptionNumber(lexical.option);
	this->closeBracketFlag = true;
	return result;
}

void MZ1Z001::Dim(const Lexical& lexical, bool delimiter, int number, int& subNumber)
{
	std::vector<std::string> variableList = DimVariableList(lexical.option);
	for(size_t i = 0; i < variableList.size(); ++ i)
	{
		std::string arraySize1;
		std::string arraySize2;
		int phase = 0;
		size_t bracket = variableList[i].find("[");
		for(size_t j = bracket + 1; j < variableList[i].size(); ++ j)
		{
			if(variableList[i][j] == ']')
			{
				++ phase;
				continue;
			}
			if(phase == 0)
			{
				arraySize1 += variableList[i][j];
			}
			else if(phase == 1)
			{
				if(variableList[i][j] == '[')
				{
					continue;
				}
				arraySize2 += variableList[i][j];
			}
		}
		std::string variableName = variableList[i].substr(0, bracket);
		std::string variableType;
		std::string str = "str";
		if((variableList[i].size() > str.size()) && std::equal(std::begin(str), std::end(str), std::begin(variableList[i])))
		{
			// 文字列配列
			variableType = "dms::String";
		}
		else
		{
			variableType = "dms::Variable";
		}
		if(arraySize2.empty() == true)
		{
			// 1次元配列
			this->result = "dms::Array<" + variableType + ", " + arraySize1 + "+1> " + variableName + ";";
		}
		else
		{
			// 2次元配列
			this->result = "dms::Array<dms::Array<" + variableType + ", " + arraySize2 + "+1>, " + arraySize1 + "+1> " + variableName + ";";
		}
		// grobalに登録
		this->grobal.push_back(this->result);
		this->dimVariableList.push_back(variableName);
	}
	this->result = "";
}

std::string MZ1Z001::DefFn(const Lexical& lexical, bool delimiter)
{
	std::string result = "dms::Variable FN";
	std::string option = FixOptionNumber(lexical.option);
	size_t pos = option.find("(", 0);
	std::string funcName = option.substr(0, pos);
	result += funcName;
	option = option.substr(pos);
	option = "(dms::Variable " + option.substr(1);
	pos = option.find("=", 0);
	if(pos != std::string::npos)
	{
		std::string funcOption = option.substr(0, pos);
		result += Trim(funcOption);
		result += " { return ";
		if(pos + 1 < option.size())
		{
			option = option.substr(pos + 1);
			result += Trim(option);
		}
	}
	this->defFnFlag = true;
	this->deffnFuncList.push_back("FN" + funcName);
	return result;
}

void MZ1Z001::EndDefFn(int number, int& subNumber)
{
	this->result += "; };";
	this->grobal.push_back(this->result);
	this->result = "";
	this->defFnFlag = false;
}

void MZ1Z001::Input(const Lexical& lexical, bool delimiter, int number, int& subNumber)
{
	std::string option = Trim(lexical.option);
	bool showQuestion = true;
	if(option[0] == '\"')
	{
		std::string printText = option;
		size_t pos = option.find('\"', 1);
		if(pos != std::string::npos)
		{
			printText = printText.substr(1, pos - 1);
			this->result = "Print(\"" + printText + "\"_s, false);";
			this->program[Number(number, subNumber)] = this->result;
			++ subNumber;
			showQuestion = false;
		}
		pos = option.find(';');
		if(pos != std::string::npos)
		{
			option = option.substr(pos + 1);
		}
	}
	std::vector<std::string> variableList = VariableList(option);
	for(size_t i = 0; i < variableList.size(); ++ i)
	{
		this->result = "Input(&" + variableList[i];
		if(showQuestion == true)
		{
			this->result += ", true);";
		}
		else
		{
			this->result += ", false);";
		}
		this->program[Number(number, subNumber)] = this->result;
		++ subNumber;
	}
	this->result = "";
}

std::string MZ1Z001::Restore(const Lexical& lexical, bool delimiter)
{
	std::string result;
	if(lexical.option.empty() == true)
	{
		result = "Restore({DATA_TOP})";
	}
	else
	{
		result = "Restore(D" + Number(lexical.option) + ")";
		std::string result = "";
		if(this->data.find(Number(lexical.option)) == this->data.end())
		{
			this->data[Number(lexical.option)] = result;
		}
	}
	return result;
}

std::string MZ1Z001::Clr(const Lexical& lexical, bool delimiter)
{
	std::string result = "Clr()";
	return result;
}

std::string MZ1Z001::Music(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixPrintOption(lexical.option, delimiter, true);
	std::string result = "Music(" + option;
	return result;
}

std::string MZ1Z001::Tempo(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Tempo(" + option;
	return result;
}

void MZ1Z001::CloseBracket(int number, int& subNumber)
{
	this->result += ");";
	this->program[Number(number, subNumber)] = this->result;
	++ subNumber;
	this->result = "";
	this->closeBracketFlag = false;
}

std::string MZ1Z001::Usr(const Lexical& lexical, bool delimiter)
{
//	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Usr(" + option;
	return result;
}

std::string MZ1Z001::Wopen(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Wopen(" + option;
	return result;
}

std::string MZ1Z001::Ropen(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Ropen(" + option;
	return result;
}

std::string MZ1Z001::Close(const Lexical& lexical, bool delimiter)
{
	std::string result = "Close()";
	return result;
}

std::string MZ1Z001::Mon(const Lexical& lexical, bool delimiter)
{
	std::string result = "Mon()";
	return result;
}

std::string MZ1Z001::Limit(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	size_t pos = option.find("MAX");
	if(pos != std::string::npos)
	{
		option = option.replace(pos, 3, "-1");
	}
	std::string result = "Limit(" + option;
	return result;
}

std::string MZ1Z001::Cont(const Lexical& lexical, bool delimiter)
{
	std::string result = "Cont()";
	return result;
}

std::string MZ1Z001::Get(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = Trim(lexical.option);
	std::string result = "Get(&" + option;
	return result;
}

std::string MZ1Z001::Inp(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Inp(" + option;
	return result;
}

std::string MZ1Z001::Out(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Out(" + option;
	return result;
}

std::string MZ1Z001::Cursor(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Cursor(" + option;
	return result;
}

std::string MZ1Z001::Set(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	this->addColorFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Set(" + option;
	return result;
}

std::string MZ1Z001::Reset(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	this->addColorFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Reset(" + option;
	return result;
}

std::string MZ1Z001::Line(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	this->closeBracesFlag = true;
	this->addColorFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Line({" + option;
	return result;
}

std::string MZ1Z001::Bline(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	this->closeBracesFlag = true;
	this->addColorFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Bline({" + option;
	return result;
}

void MZ1Z001::Console(const Lexical& lexical, bool delimiter, int number, int& subNumber)
{
	std::string option = Trim(lexical.option);
	int phase = 0;
	std::string command;
	std::string value1;
	std::string value2;
	for(size_t i = 0; i < option.size(); ++ i)
	{
		bool last = (i == option.size() - 1);
		if(command == "S")
		{
			if(option[i] == ' ')
			{
				continue;
			}
			if(isdigit(option[i]) != 0)
			{
				if(phase == 0)
				{
					value1 += option[i];
				}
				else
				{
					value2 += option[i];
				}
			}
			else if(option[i] == ',')
			{
				++ phase;
				continue;
			}
			if((isdigit(option[i]) == 0) || (last == true))
			{
				this->result = "ScrollYRange(" + value1 + "," + value2 + ");";
				this->program[Number(number, subNumber)] = FixOptionNumber(this->result);
				++ subNumber;
				value1.clear();
				value2.clear();
				command.clear();
			}
		}
		if(command == "C")
		{
			if(isdigit(option[i]) != 0)
			{
				value1 += option[i];
			}
			if(value1 == "40")
			{
				this->result = "SetTextStretchWidth(2);";
				this->program[Number(number, subNumber)] = FixOptionNumber(this->result);
				++ subNumber;
				value1.clear();
				value2.clear();
				command.clear();
			}
			else if(value1 == "80")
			{
				this->result = "SetTextStretchWidth(1);";
				this->program[Number(number, subNumber)] = FixOptionNumber(this->result);
				++ subNumber;
				value1.clear();
				value2.clear();
				command.clear();
			}
		}
		if(command == "G")
		{
			if(option[i] == 'H')
			{
				this->result = "SetStretchWidth(1);";
			}
			else if(option[i] == 'N')
			{
				this->result = "SetStretchWidth(2);";
			}
			this->program[Number(number, subNumber)] = FixOptionNumber(this->result);
			++ subNumber;
			value1.clear();
			value2.clear();
			command.clear();
		}
		if(command == "R")
		{
			// グリーンディスプレイにグラフィックを表示しない
			this->result = "SetReverse(true);";
			this->program[Number(number, subNumber)] = this->result;
			++ subNumber;
			value1.clear();
			value2.clear();
			command.clear();
		}
		if(command == "N")
		{
			// グリーンディスプレイにグラフィックを表示する
			this->result = "SetReverse(false);";
			this->program[Number(number, subNumber)] = this->result;
			++ subNumber;
			value1.clear();
			value2.clear();
			command.clear();
		}
		if(command == "P")
		{
			this->wMode = 0;
			// グリーンディスプレイにグラフィックを表示しない
			this->result = "EnableGraphicsGreenDisplay(false);";
			this->program[Number(number, subNumber)] = this->result;
			++ subNumber;
			value1.clear();
			value2.clear();
			command.clear();
		}
		if(command == "M")
		{
			this->wMode = 0;
			// グリーンディスプレイにグラフィックを表示する
			this->result = "EnableGraphicsGreenDisplay(true);";
			this->program[Number(number, subNumber)] = this->result;
			++ subNumber;
			value1.clear();
			value2.clear();
			command.clear();
		}
		if(command.empty() == true)
		{
			if(option[i] == ',')
			{
				continue;
			}
			else if(option[i] == 'S')
			{
				command = "S";
				phase = 0;
			}
			else if(option[i] == 'C')
			{
				command = "C";
			}
			else if(command == "R")
			{
				command = "R";
			}
			else if(command == "N")
			{
				command = "N";
			}
			else if(option[i] == 'G')
			{
				command = "G";
			}
			else if(option[i] == 'P')
			{
				command = "P"; // グリーンディスプレイにグラフィックを表示しない
			}
			else if(option[i] == 'M')
			{
				command = "M"; // グリーンディスプレイにグラフィックを表示する
			}
		}
	}
	this->result = "";
}

void MZ1Z001::Graph(const Lexical& lexical, bool delimiter, int number, int& subNumber)
{
	std::string command;
	std::string page;
	std::vector<std::string> optionList = SpritText(lexical.option, ",");
	for(size_t optionIndex = 0; optionIndex < optionList.size(); ++ optionIndex)
	{
		std::string option = optionList[optionIndex];
		for(size_t i = 0; i < option.size(); ++ i)
		{
			bool last = (i == option.size() - 1);
			if(option[i] == ' ')
			{
				continue;
			}
			if(command == "O")
			{
				if(isdigit(option[i]) != 0)
				{
					page += option[i];
				}
				if(isdigit(option[i]) == 0 || last == true)
				{
					this->result = "GraphOutput(O_" + page + ");";
					this->program[Number(number, subNumber)] = this->result;
					++ subNumber;
					page.clear();
					command.clear();
				}
			}
			if(command == "I")
			{
				if(isdigit(option[i]) != 0)
				{
					page += option[i];
					this->result = "GraphInput(" + page + ");";
					this->program[Number(number, subNumber)] = FixOptionNumber(this->result);
					++ subNumber;
					page.clear();
					command.clear();
				}
			}
			if(command.empty() == true)
			{
				if(option[i] == ',')
				{
					continue;
				}
				else if(option[i] == 'O')
				{
					command = "O";
				}
				else if(option[i] == 'I')
				{
					command = "I";
				}
				else if(option[i] == 'C')
				{
					if((i == 0) && (option.size() > 1))
					{
						option.erase(0, 1);
						this->result = "ClearGraph(" + option + ");";
					}
					else
					{
						this->result = "ClearGraph();";
					}
					this->program[Number(number, subNumber)] = FixOptionNumber(this->result);
					++ subNumber;
				}
				else if(option[i] == 'F')
				{
					if((i == 0) && (option.size() > 1))
					{
						option.erase(0, 1);
						this->result = "FillGraph(" + option + ");";
					}
					else
					{
						this->result = "FillGraph();";
					}
					this->program[Number(number, subNumber)] = FixOptionNumber(this->result);
					++ subNumber;
				}
			}
		}
	}
	this->result = "";
}

std::string MZ1Z001::Position(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Position(" + option;
	return result;
}

std::string MZ1Z001::Pattern(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	this->patternFlag = false;
	this->patternSeparatorCount = 0;
	this->addColorFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Pattern(" + option;
	return result;
}

std::string MZ1Z001::Auto(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Image(" + option;
	return result;
}

std::string MZ1Z001::Image(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Image(" + option;
	return result;
}

std::string MZ1Z001::Copy(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Copy(" + option;
	return result;
}

std::string MZ1Z001::Page(const Lexical& lexical, bool delimiter)
{
	this->closeBracketFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::string result = "Page(" + option;
	return result;
}

std::string MZ1Z001::Boot(const Lexical& lexical, bool delimiter)
{
	std::string result = "Boot()";
	return result;
}

std::string MZ1Z001::Klist(const Lexical& lexical, bool delimiter)
{
	std::string result = "Klist()";
	return result;
}

std::string MZ1Z001::Change(const Lexical& lexical, bool delimiter)
{
	std::string result = "Change()";
	return result;
}

std::string MZ1Z001::Rew(const Lexical& lexical, bool delimiter)
{
	std::string result = "Rew()";
	return result;
}

std::string MZ1Z001::Fast(const Lexical& lexical, bool delimiter)
{
	std::string result = "Fast()";
	return result;
}

std::string MZ1Z001::BasicFunction(const Lexical& lexical, bool delimiter)
{
	std::string functionName = lexical.command;
	std::transform(functionName.begin(), functionName.end(), functionName.begin(), tolower);
	std::transform(functionName.begin(), functionName.begin() + 1, functionName.begin(), toupper);
	std::string result = functionName + FixOptionNumber(lexical.option);
	return result;
}

std::string MZ1Z001::BasicStringFunction(const Lexical& lexical, bool delimiter)
{
	std::string functionName = lexical.command;
	std::transform(functionName.begin(), functionName.end(), functionName.begin(), tolower);
	std::transform(functionName.begin(), functionName.begin() + 1, functionName.begin(), toupper);
	size_t pos = functionName.find("$(");
	if(pos != std::string::npos)
	{
		functionName = functionName.replace(pos, 2, "(");
	}
	std::string option = lexical.option;
	option = FixOptionNumber(option);
	std::string result = functionName + option;
	return result;
}

std::string MZ1Z001::BasicSign(const Lexical& lexical, bool delimiter)
{
	std::string command = lexical.command;
	if(command == "<>")
	{
		command = "!=";
	}
	else if(command == "><")
	{
		command = "!=";
	}
	else if(command == "=<")
	{
		command = "<=";
	}
	else if(command == "=>")
	{
		command = ">=";
	}
	std::string result = " " + command + " " + FixOptionNumber(lexical.option);
	return result;
}

std::string MZ1Z001::Pi(const Lexical& lexical, bool delimiter)
{
	std::string result = "Pi()" + FixOptionNumber(lexical.option);
	return result;
}

std::string MZ1Z001::Pos(const Lexical& lexical, bool delimiter)
{
	std::string command = lexical.option.substr(0, 1).c_str();
	std::transform(command.begin(), command.end(), command.begin(), tolower);
	std::string option = lexical.option.substr(1);
	std::string result = "Pos" + command +"()" + FixOptionNumber(option);
	return result;
}

std::string MZ1Z001::Color(const Lexical& lexical, bool delimiter)
{
	this->wMode = 0;
	this->closeBracketFlag = true;
	this->addColorFlag = true;
	std::string option = FixColorOption(lexical.option, true);
	std::string result = "Color(" + option;
	return result;
}

std::string MZ1Z001::CColor(const Lexical& lexical, bool delimiter)
{
	this->wMode = 0;
	this->closeBracketFlag = true;
	std::string option = FixCcolorOption(lexical.option);
	std::string result = "CColor(" + option;
	return result;
}

std::string MZ1Z001::Circle(const Lexical& lexical, bool delimiter)
{
	this->wMode = 0;
	this->closeBracketFlag = true;
	this->addColorFlag = true;
	std::string option = FixCircleOption(lexical.option);
	std::string result = "Circle(" + option;
	return result;
}


std::string MZ1Z001::Box(const Lexical& lexical, bool delimiter)
{
	this->wMode = 0;
	this->closeBracketFlag = true;
	this->addColorFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::vector<std::string> parameters = SpritText(option, ",");
	std::string boxOption = parameters[0] + "," + parameters[1] + "," + parameters[2] + "," + parameters[3];
	if(parameters.size() > 4)
	{
		std::string fill = EraseSpace(parameters[4]);
		if(fill[0] == 'F')
		{
			fill.erase(0, 1);
			if(fill.size() == 0)
			{
				boxOption += ",0_n"; // 塗りつぶしは枠と同じ色
			}
			else
			{
				boxOption += fill; // 塗りつぶし色指定
				boxOption += "_n";
			}
		}
	}
	else
	{
		boxOption += ",-1_n"; // 塗りつぶしはしない枠のみ
	}
	std::string result = "Box(" + boxOption;
	return result;
}

std::string MZ1Z001::Paint(const Lexical& lexical, bool delimiter)
{
	this->wMode = 0;
	this->closeBracketFlag = true;
	this->addColorFlag = true;
	std::string option = FixOptionNumber(lexical.option);
	std::vector<std::string> parameters = SpritText(option, ",");
	std::string paintOption = parameters[0] + "," + parameters[1];
	if(parameters.size() > 2)
	{
		paintOption += ",{";
		for(size_t i = 2; i < parameters.size(); ++ i)
		{
			paintOption += parameters[i];
			if(i < parameters.size() - 1)
			{
				paintOption += ",";
			}
		}
		paintOption += "}";
	}
	else
	{
		paintOption += ",{}";
	}
	std::string result = "Paint(" + paintOption;
	return result;
}

std::vector<std::string> MZ1Z001::SpritText(std::string text, std::string separator)
{
	int bracketsCount = 0;
	bool quotation = false;
	std::vector<std::string> spritTextList;
	std::string spritText = "";
	for(size_t i = 0; i < text.size(); ++ i)
	{
		unsigned char byte = text[i];
		if(byte == '\"')
		{
			if(quotation == true)
			{
				quotation = false;
			}
			else
			{
				quotation = true;
			}
		}
		if(quotation == false)
		{
			if(byte == '(')
			{
				++ bracketsCount;
			}
			else if(byte == ')')
			{
				-- bracketsCount;
			}
			if(bracketsCount == 0)
			{
				if(byte == separator[0])
				{
					spritTextList.push_back(spritText);
					spritText = "";
					continue;
				}
			}
		}
		spritText.push_back(byte);
	}
	if(spritText.empty() == false)
	{
		spritTextList.push_back(spritText);
	}
	return spritTextList;
}

std::string MZ1Z001::FixCcolorOption(std::string option)
{
	std::string fixOption = "";
	std::vector<std::string> spritOptionList = SpritText(option, ",");
	if(spritOptionList.size() > 0)
	{
		std::string fixText = EraseSpace(spritOptionList[0]);
		if(fixText[0] == '@')
		{
			fixOption += "1";
			fixText.erase(0, 1);
		}
		else
		{
			fixOption += "0";
		}
		if(fixText.empty() == true)
		{
			fixOption += ", -1";
		}
		else
		{
			fixOption += ", ";
			fixOption += fixText;
		}
		if(spritOptionList.size() > 1)
		{
			fixOption += ", ";
		}
	}
	if(spritOptionList.size() > 1)
	{
		std::string fixText = EraseSpace(spritOptionList[1]);
		fixOption += fixText;
	}
	fixOption = FixOptionNumber(fixOption);
	return fixOption;
}

// colorCommand true: Color文解析, false: []に囲まれた色指定
std::string MZ1Z001::FixColorOption(std::string option, bool colorCommand)
{
	int bracketsCount = 0;
	bool quotation = false;
	size_t atMarkIndex = 256;
	size_t oIndex = 256;
	size_t wIndex = 256;
	std::string fixOption = "";
	std::vector<std::string> spritOptionList = SpritText(option, ",");
	// @ and ColorNumber
	size_t optionIndex = 0;
	if(spritOptionList.size() > optionIndex)
	{
		std::string fixText = EraseSpace(spritOptionList[optionIndex]);
		if(colorCommand == true)
		{
			if(fixText[0] == '@')
			{
				fixOption += "1";
				fixText.erase(0, 1);
			}
			else
			{
				fixOption += "0";
			}
			fixOption += ", ";
		}
		if(fixText.empty() == true)
		{
			fixOption += "-1";
		}
		else
		{
			fixOption += fixText;
		}
		if(spritOptionList.size() > optionIndex + 1)
		{
			fixOption += ", ";
		}
	}
	++ optionIndex;
	if(colorCommand == true)
	{
		// O
		if(spritOptionList.size() > optionIndex)
		{
			std::string fixText = EraseSpace(spritOptionList[optionIndex]);
			if(fixText[0] == 'O')
			{
				fixText.erase(0, 1);
			}
			if(fixText.empty() == true)
			{
				fixOption += "-1";
			}
			else
			{
				fixOption += fixText;
			}
			if(spritOptionList.size() > optionIndex + 1)
			{
				fixOption += ", ";
			}
		}
		++ optionIndex;
	}
	// W
	if(spritOptionList.size() > optionIndex)
	{
		std::string fixText = EraseSpace(spritOptionList[optionIndex]);
		if(fixText[0] == 'W')
		{
			fixText.erase(0, 1);
		}
		if(fixText.empty() == true)
		{
			fixOption += "-1";
		}
		else
		{
			fixOption += fixText;
		}
	}
	else
	{
		fixOption += ",0";
	}
	fixOption = FixOptionNumber(fixOption);
	return fixOption;
}

bool MZ1Z001::CheckEncode(unsigned char byte)
{
	if(byte < 0x20)
	{
		return true;
	}
	if(byte >= 0x7F && byte <= 0x9F)
	{
		return true;
	}
	if(byte == 0xA0 || byte == 0xA4 || byte == 0xA5)
	{
		return true;
	}
	if(byte > 0xE0 && byte <= 0xFF)
	{
		return true;
	}
	if(byte == 0x5C)
	{
		return true;
	}
	return false;
}

bool MZ1Z001::CheckEncodeAfter(unsigned char byte)
{
	if(byte >= 'A' && byte <= 'F')
	{
		return true;
	}
	if(byte >= 'a' && byte <= 'f')
	{
		return true;
	}
	if(byte >= '0' && byte <= '9')
	{
		return true;
	}
	return false;
}

std::string MZ1Z001::Encode(std::string text)
{
	std::string option = text;
	std::string result;
	bool encodeAfter = false;
	for(size_t i = 0; i < option.size(); ++ i )
	{
		if(CheckEncode(option[i]) == true)
		{
			result += Format("\\x%02X", static_cast<unsigned char>(option[i]));
			encodeAfter = true;
		}
		else
		{
			if((encodeAfter == true) && (CheckEncodeAfter(option[i]) == true))
			{
				result += Format("\\x%02X", static_cast<unsigned char>(option[i]));
			}
			else
			{
				result += option[i];
			}
			encodeAfter = false;
		}
	}
	return result;
}

std::string MZ1Z001::Space(void)
{
	if(this->space == 0)
	{
		return "";
	}
	return std::string(this->space, ' ');
}

std::string MZ1Z001::Number(int number, int subNumber)
{
	return Format("%05d_%02d", number, subNumber);
}

std::string MZ1Z001::Number(std::string number)
{
	int intNumber = atoi(number.c_str());
	return Number(intNumber, 1);
}

std::string MZ1Z001::NumberList(std::string number)
{
	std::string separator = ",";
	size_t separatorLength = separator.length();
	size_t offset = 0;
	std::vector<std::string> numberList;
	while(1)
	{
		size_t pos = number.find(separator, offset);
		if (pos == std::string::npos) {
			std::string variableName = EraseSpace(number.substr(offset));
			numberList.push_back(variableName);
			break;
		}
		std::string variableName = EraseSpace(number.substr(offset, pos - offset));
		numberList.push_back(variableName);
		offset = pos + separatorLength;
	}
	std::string result = "{";
	for(size_t i = 0; i < numberList.size(); ++ i)
	{
		result += ("L" + Number(numberList[i]));
		if(i < numberList.size() - 1)
		{
			result += ", ";
		}
	}
	result += "}";
	return result;
}

bool MZ1Z001::CheckReserved(std::string text)
{
	std::vector<std::string> disableList = {"TI$", "CSRV", "CSRH", "POSV", "POSH", "SIZE"};
	for(size_t i = 0; i < disableList.size(); ++ i)
	{
		if(disableList[i] == text)
		{
			return true;
		}
	}
	return false;
}

std::string MZ1Z001::ReplaceAll(const std::string& text, const std::string& target, const std::string& replacement)
{
	std::string result = text;
	std::string::size_type pos = 0;
	while((pos = result.find(target, pos)) != std::string::npos)
	{
		result.replace(pos, target.length(), replacement);
		pos += replacement.length();
	}
	return result;
}

bool MZ1Z001::CheckBasicFunction(const std::string& text)
{
	std::vector<std::string> functionList = {"RND(", "SIN(", "COS(", "TAN(", "ATN(", "EXP(", "INT(", "LOG(", "LN(", "ABS(", "SGN(", "SQR(", "ASC(", "VAL(", "POINT(", "LEN(", "PEEK(", "TAB("};
	for(size_t i = 0; i < functionList.size(); ++ i)
	{
		if(functionList[i] == text)
		{
			return true;
		}
	}
	return false;
}

bool MZ1Z001::CheckBasicStringFunction(const std::string& text)
{
	std::vector<std::string> functionList = {"LEFT$(", "RIGHT$(", "MID$(", "CHR$(", "STR$(", "SPACE$(", "STRING$(", "CHARACTER$("};
	for (size_t i = 0; i < functionList.size(); ++i)
	{
		if (functionList[i] == text)
		{
			return true;
		}
	}
	return false;
}

// 不等号かどうか
bool MZ1Z001::CheckBasicSign(const std::string& text)
{
	std::vector<std::string> functionList = { "><", "<>", "=<", "<=", "=>", ">=", ">", "<"};
	for (size_t i = 0; i < functionList.size(); ++i)
	{
		if (functionList[i] == text)
		{
			return true;
		}
	}
	return false;
}

// データを文字列のarrayに変換する
std::string MZ1Z001::ParseData(const std::string& data)
{
	std::string sourceData = data;
	if(data.empty() == true)
	{
		return "{}";
	}
	if(sourceData[sourceData.size() -1] == ',')
	{
		sourceData.erase(-- sourceData.end());
	}
	std::string result = "{\"";
	bool doubleQuotation = false;
	bool encodeAfter = false;
	for(size_t i = 0; i < sourceData.size(); ++i)
	{
		unsigned char byte = static_cast<unsigned char>(sourceData[i]);
		if (doubleQuotation == true)
		{
			if(byte == '\"')
			{
				doubleQuotation = false;
				continue;
			}
			if(CheckEncode(byte) == true)
			{
				result += Format("\\x%02X", byte);
				encodeAfter = true;
			}
			else
			{
				if(encodeAfter == true)
				{
					result += Format("\\x%02X", byte);
				}
				else
				{
					result += Format("%c", byte);
				}
				encodeAfter = false;
			}
		}
		else
		{
			if(byte == '\"')
			{
				doubleQuotation = true;
				continue;
			}
			if(byte == ',')
			{
				result += "\"_s,\"";
				continue;
			}
			if(CheckEncode(byte) == true)
			{
				result += Format("\\x%02X", byte);
				encodeAfter = true;
			}
			else
			{
				if(encodeAfter == true)
				{
					result += Format("\\x%02X", byte);
				}
				else
				{
					result += Format("%c", byte);
				}
				encodeAfter = false;
			}
		}
	}
	result += "\"_s}";
	return result;
}

// 文字列変数名を変更する
// A$ => strA
std::string MZ1Z001::ChangeVariableArrayName(std::string variableName)
{
	variableName = "array" + variableName;
	return variableName;
}

// 文字列変数名を変更する
// A$ => strA
std::string MZ1Z001::ChangeStringVariableName(std::string variableName, bool array)
{
	std::string prefix = "";
	size_t dollarPos = variableName.find_last_of('$');
	if(dollarPos != std::string::npos)
	{
		prefix = "str";
		if(array == true)
		{
			prefix += "Array";
		}
		variableName = variableName.erase(dollarPos);
		variableName = prefix + variableName;
	}
	else
	{
		if(array == true)
		{
			prefix += "array";
		}
		variableName = prefix + variableName;
	}
	return variableName;
}

// 文字列変数名($付き)かどうか
bool MZ1Z001::IsStringVariableName(std::string variableName)
{
	size_t dollarPos = variableName.find_last_of('$');
	if(dollarPos == 0)
	{
		return false;
	}
	if(dollarPos != std::string::npos)
	{
		return true;
	}
	return false;
}

// 文字列変数名(str付き)かどうか
bool MZ1Z001::IsStringFixedVariableName(std::string variableName)
{
	size_t strPos = variableName.find("str");
	if(strPos != std::string::npos)
	{
		return true;
	}
	return false;
}

// 文字列からスペースを削除する
std::string MZ1Z001::EraseSpace(const std::string& text)
{
	std::string result = "";
	bool quotation = false;
	for(size_t i = 0; i < text.size(); ++ i)
	{
		unsigned char byte = text[i];
		if(byte == '\"')
		{
			if(quotation == true)
			{
				quotation = false;
			}
			else
			{
				quotation = true;
			}
		}
		if((quotation == true) || ((quotation == false) && (byte != ' ')))
		{
			result += byte;
		}
	}
	return result;
}

// 前後のスペースを削除する
std::string MZ1Z001::Trim(const std::string& text, const char* trimCharacterList)
{
	std::string result;
	std::string::size_type left = text.find_first_not_of(trimCharacterList);
	if(left != std::string::npos)
	{
		std::string::size_type right = text.find_last_not_of(trimCharacterList);
		result = text.substr(left, right - left + 1);
	}
	return result;
}

// ,で区切られた変数リストを取得する
std::vector<std::string> MZ1Z001::VariableList(const std::string& text)
{
	std::string separator = ",";
	size_t separatorLength = separator.length();
	size_t offset = 0;
	std::vector<std::string> variableList;
	while(1)
	{
		size_t pos = text.find(separator, offset);
		if (pos == std::string::npos) {
			std::string variableTemp = text.substr(offset);
			if(variableTemp.empty() == true)
			{
				break;
			}
			std::string variableName = EraseSpace(variableTemp);
			variableList.push_back(variableName);
			break;
		}
		std::string variableName = EraseSpace(text.substr(offset, pos - offset));
		variableList.push_back(variableName);
		offset = pos + separatorLength;
	}
	return variableList;
}

// Dim関数用,で区切られた変数リストを取得する
std::vector<std::string> MZ1Z001::DimVariableList(const std::string& text)
{
	std::vector<std::string> variableList = VariableList(text);
	// 2次元配列を修復する
	std::vector<std::string> resultVariableList;
	size_t destinationIndex = 0;
	for(size_t i = 0; i < variableList.size(); ++ i)
	{
		size_t length = variableList[i].size();
		if(variableList[i].find("rray") != std::string::npos)
		{
			resultVariableList.push_back(variableList[i]);
			++ destinationIndex;
		}
		else
		{
			resultVariableList[destinationIndex - 1] += ",";
			resultVariableList[destinationIndex - 1] += variableList[i];
		}
	}
	return resultVariableList;
}

// ダブルクォーテーションで囲まれた文字列をスペースで埋める
std::string MZ1Z001::DeleteQuotationString(std::string& text)
{
	std::string result = "";
	bool quotation = false;
	for(size_t i = 0; i < text.size(); ++ i)
	{
		if(text[i] == '\"')
		{
			quotation = !quotation;
		}
		if(quotation == false)
		{
			result += text[i];
		}
		else
		{
			result += " ";
		}
	}
	return result;
}

void MZ1Z001::ReplaceNumber(void)
{
	// 次の命令の行番号と次の行番号を抽出
	auto dataBegin = this->data.begin();
	std::string dataTopNumber = "Unknown";
	if(this->data.empty() != true)
	{
		dataTopNumber = dataBegin->first;
	}
	std::string nextNumber;
	std::string nextCommandNumber;
	for(auto iter = this->program.rbegin(); iter != this->program.rend(); ++ iter)
	{
		//printf("%s %s\n", iter->first.c_str(), iter->second.c_str());
		std::string number = iter->first;
		std::string command = iter->second;
		// Restore文のDATA_TOPを入れ替える
		size_t dataTop = command.find("{DATA_TOP}");
		if(dataTop != std::string::npos)
		{
			command = ReplaceAll(command, "{DATA_TOP}", "L" + dataTopNumber);
			this->program[number] = command;
		}
		// If文のL_NEXT_COMMANDとL_NEXT_LINEを入れ替える
		//printf("number: %s, nextNumber: %s, nextCommandNumber: %s\n", number.c_str(), nextNumber.c_str(), nextCommandNumber.c_str());
		size_t next1 = command.find("{L_NEXT_COMMAND}");
		if(next1 != std::string::npos)
		{
			command = ReplaceAll(command, "{L_NEXT_COMMAND}", "L" + nextCommandNumber);
			this->program[number] = command;
		}
		size_t next2 = command.find("{L_NEXT_LINE}");
		if(next1 != std::string::npos)
		{
			command = ReplaceAll(command, "{L_NEXT_LINE}", "L" + nextNumber);
			this->program[number] = command;
		}
		if(iter->first.find("_01") != std::string::npos)
		{
			nextNumber = iter->first;
		}
		nextCommandNumber = iter->first;
	}
}

std::string MZ1Z001::ReplaceAll(std::string &replacedStr, std::string from, std::string to)
{
	size_t pos = replacedStr.find(from);
	size_t toLen = to.length();
	if(from.empty() == true)
	{
		return replacedStr;
	}
	while((pos = replacedStr.find(from, pos)) != std::string::npos)
	{
		replacedStr.replace(pos, from.length(), to);
		pos += toLen;
	}
	return replacedStr;
}

// 変数リストに含まれるかどうか
bool MZ1Z001::FindVariableList(std::string variableName)
{
	for(size_t i = 0; i < this->variableList.size(); ++ i)
	{
		if(this->variableList[i] == variableName)
		{
			return true;
		}
	}
	for(size_t i = 0; i < this->dimVariableList.size(); ++ i)
	{
		if(this->dimVariableList[i] == variableName)
		{
			return true;
		}
	}
	return false;
}

bool MZ1Z001::FindDefFnFuncList(std::string variableName)
{
	for(size_t i = 0; i < this->deffnFuncList.size(); ++ i)
	{
		if(this->deffnFuncList[i] == variableName)
		{
			return true;
		}
	}
	return false;
}

// オプションを修正する
std::string MZ1Z001::FixOption(std::string sourceCommand, std::string sourceOption, bool delimiter)
{
	std::string option = Trim(sourceOption);
	return option;
}

// Print文のセミコロンを+に変換する
std::string MZ1Z001::FixPrintOption(std::string sourceOption, bool delimiter, bool commaToPlus)
{
	std::string result;
	std::string option = FixOptionNumber(sourceOption);
	int phase = 0; // 0=nop, 1=direct
	for(size_t i = 0; i < option.size(); ++ i)
	{
		bool last = (i == option.size() - 1);
		unsigned char byte = static_cast<unsigned char>(option[i]);
		if(byte == '\"')
		{
			phase = 1 - phase;
		}
		else if(byte == '[')
		{
			phase = 1 - phase;
		}
		else if(byte == ']')
		{
			phase = 1 - phase;
		}
		if(phase == 0)
		{
			if((byte == ';') || ((byte == ',') && (commaToPlus == true)))
			{
				if((last == true) && (delimiter == true))
				{
					this->printSemicolonFlag = true;
					continue;
				}
				else
				{
					if((i > 0) && (option[i - 1] == ';'))
					{
						continue;
					}
					byte = '+';
				}
			}
		}
		if(byte > 0)
		{
			result += Format("%c", byte);
		}
	}
	return result;
}

std::string MZ1Z001::FixCircleOption(std::string sourceOption)
{
	std::string result;
	std::string option = FixOptionNumber(sourceOption);
	std::vector<std::string> spritText = SpritText(option, ",");
	for(size_t i = 0; i < 7; ++ i)
	{
		if(i < spritText.size())
		{
			result += spritText[i];
		}
		else if(i == 6)
		{
			result += "0_n";
		}
		else
		{
			result += "-1_n";
		}
		if(i < 6)
		{
			result += ",";
		}
	}
	return result;
}

// 数値リテラルに_nまたは_fをつける
// $####も16進数の数値リテラル扱いにして\xを先頭につける
std::string MZ1Z001::FixOptionNumber(std::string sourceOption)
{
	std::string result;
	std::string option = Trim(sourceOption);
	int phase = 0; // 0=nop, 1=direct
	bool first = true;
	std::string number;
	bool enableMinus = false;
	bool dollar = false;
	for(size_t i = 0; i < option.size(); ++ i)
	{
		bool last = (i == option.size() - 1);
		unsigned char byte = static_cast<unsigned char>(option[i]);
		unsigned char nextByte = last ? 0 : static_cast<unsigned char>(option[i + 1]);
		if(byte == '\"')
		{
			phase = 1 - phase;
		}
		if(phase == 0)
		{
			bool variableOrNumber = false;
			if(byte == '$')
			{
				dollar = true;
				continue;
			}
			if(dollar == false)
			{
				if(IsFixedVariableNameChar(byte, first) == true)
				{
					// 変数中
					first = false;
					variableOrNumber = true;
					enableMinus = false;
				}
				else if((byte >= '0') && (byte <= '9') || (byte == '.') || ((enableMinus == true) && (number.empty() == true) && byte == '-' && (nextByte >= '0') && (nextByte <= '9')))
				{
					// 数値
					number += Format("%c", byte);
					variableOrNumber = true;
					byte = 0;
				}
			}
			else
			{
				if((byte >= '0') && (byte <= '9') || (byte >= 'A') && (byte <= 'F'))
				{
					// 16進数の数値
					number += Format("%c", byte);
					variableOrNumber = true;
					byte = 0;
				}
				else
				{
					// 数値じゃない
					first = false;
					variableOrNumber = false;
					enableMinus = false;
				}
			}
			if((variableOrNumber == false) || (last == true))
			{
				if((number.empty() == false) && (number != "-"))
				{
					// 数値を発見
//					printf("**1** %s\n", number.c_str());
					if((byte > 0) && (variableOrNumber == true))
					{
						result += Format("%c", byte);
						byte = 0;
					}
					if(dollar == true)
					{
						result += "0x";
						dollar = false;
					}
					result += number;
					if((byte != '_') && (nextByte != 'n'))
					{
						if(number.find(".") == std::string::npos)
						{
							result += "_n";
						}
						else
						{
							result += "_f";
						}
					}
				}
				number.clear();
				enableMinus = true;
				first = true;
			}
		}
		if(byte > 0)
		{
			result += Format("%c", byte);
		}
		if(number.empty() == false)
		{
			int a = 0;
		}
	}
	return result;
}

void MZ1Z001::DebugLog(std::string text)
{
	FileData file;
	file.SetBuffer(&text[0], text.size());
	file.SaveAdd("Converter.log");
}
