#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "MZ1Z002.hpp"

static const char* const NAME = "MZ-80B/2000/2200用のMZTファイルをCPPファイルに変換するプログラム";
static const char* const VERSION = "1.0.0";
static const char* const FILENAME = "SBasicConverter";
static const unsigned int OPTION_HELP   = 0x00000001;

int main(int argc, char* argv[])
{
	unsigned int option = 0;
	std::string path1; // = "KORO.mzt";
	std::string path2;
	int i;
	// オプション取得
	if(path1.empty() == true)
	{
		if(argc > 1)
		{
			for(i = 1; i < argc; i ++)
			{
				if((argv[i][0] == '/') || (argv[i][0] == '-'))
				{
					if(_strnicmp(&argv[i][1], "HELP", 4) == 0)
					{
						option |= OPTION_HELP;
					}
					else if(_strnicmp(&argv[i][1], "?", 1) == 0)
					{
						option |= OPTION_HELP;
					}
				}
				else
				{
					if(path1.empty())
					{
						path1 = argv[i];
					}
					else if(path2.empty())
					{
						path2 = argv[i];
					}
				}
			}
		}
		else
		{
			option |= OPTION_HELP;
		}
	}
	// タイトル表示
	std::cout << NAME << " version." << VERSION << "\n";
	// オプション表示
	if(option & OPTION_HELP)
	{
		std::cout << "Usage : " << NAME << " [option] [input mzt filename] [output cpp filename]" << std::endl;
		std::cout << std::endl;
		std::cout << "Option : /HELP            Display help message." << std::endl;
		std::cout << std::endl;
		return -1;
	}
	if(path1.empty() == true)
	{
		std::cout << "Invalid folder name." << std::endl;
		return -1;
	}
	if(path2.empty() == true)
	{
		path2 = "Basic.cpp";
	}
	// コンバートする
	std::cout << "Converte " << path1 << " to " << path2 << std::endl;
	MZ1Z002 converter;
	converter.ConvertFile(path1.c_str(), path2.c_str());
	std::cout << "Complete" << std::endl;
}
