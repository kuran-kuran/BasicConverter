#include <stdarg.h>
#include "DataManager.hpp"

DataManager::DataManager(void)
:dataIndex()
,datas()
,readIndex(0)
{
}

DataManager::~DataManager(void)
{
}

void DataManager::SetData(int number, std::vector<Data> data)
{
	if(number > 0)
	{
		this->dataIndex[number] = static_cast<int>(this->datas.size());
	}
	for(size_t i = 0; i < data.size(); ++i)
	{
		this->datas.push_back(data[i]);
	}
}

Data DataManager::Read(void)
{
	Data data = this->datas[this->readIndex];
	++ this->readIndex;
	return data;
}

dms::Variable DataManager::ReadVariable(void)
{
	Data data = Read();
	return data.GetVariable();
}

dms::String DataManager::ReadString(void)
{
	Data data = Read();
	return data.GetString();
}

void DataManager::Restore(int number)
{
	if(number == -1)
	{
		this->readIndex = 0;
	}
	else
	{
		for(std::map<int, int>::iterator iter = this->dataIndex.begin(); iter != this->dataIndex.end(); ++ iter)
		{
			if(iter->first >= number)
			{
				number = iter->first;
				break;
			}
		}
		this->readIndex = this->dataIndex[number];
	}
}
