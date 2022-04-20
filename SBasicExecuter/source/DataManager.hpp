#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <vector>
#include <map>
#include "Data.hpp"

class DataManager
{
public:
	DataManager(void);
	~DataManager(void);
	void SetData(int number, std::vector<Data> data);
	Data Read(void);
	dms::Variable ReadVariable(void);
	dms::String ReadString(void);
	void Restore(int number);
private:
	std::map<int, int> dataIndex;
	std::vector<Data> datas;
	int readIndex;
};

#endif
