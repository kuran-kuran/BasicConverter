#ifndef FILEDATA_HPP
#define FILEDATA_HPP

#include <vector>
#include <string>

class FileData
{
public:
	FileData(void);
	~FileData(void);
	bool Load(const std::string& path);
	bool Save(const std::string& path);
	bool SaveAdd(const std::string& path);
	void Clear(void);
	void* GetBuffer(void);
	size_t GetBufferSize(void);
	void SetBuffer(void* buffer, size_t size);
private:
	std::vector<unsigned char> buffer;
};

#endif
