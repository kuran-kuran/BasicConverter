#include <stdio.h>
#include <iterator>
#include <algorithm>
#include "FileData.hpp"

FileData::FileData(void)
:buffer()
{
}

FileData::~FileData(void)
{
}

bool FileData::Load(const std::string& path)
{
	FILE* file = NULL;
	if(fopen_s(&file, path.c_str(), "rb") != 0)
	{
		return false;
	}
	long position;
	fseek(file, 0, SEEK_END);
	position = ftell(file);
	if(position == -1)
	{
		return false;
	}
	size_t filesize = static_cast<size_t>(position);
	fseek(file, 0, SEEK_SET);
	this->buffer.resize(filesize);
	fread(&this->buffer[0], 1, filesize, file);
	fclose(file);
	return true;
}

bool FileData::Save(const std::string& path)
{
	FILE* file = NULL;
	if(fopen_s(&file, path.c_str(), "wb") != 0)
	{
		return false;
	}
	fwrite(&this->buffer[0], 1, this->buffer.size(), file);
	fclose(file);
	return true;
}

bool FileData::SaveAdd(const std::string& path)
{
	FILE* file = NULL;
	if(fopen_s(&file, path.c_str(), "rb+") != 0)
	{
		if(fopen_s(&file, path.c_str(), "wb+") != 0)
		{
			return false;
		}
	}
	fseek(file, 0, SEEK_END);
	fwrite(&this->buffer[0], 1, this->buffer.size(), file);
	fclose(file);
	return true;
}

void FileData::Clear(void)
{
	this->buffer.clear();
}

void* FileData::GetBuffer(void)
{
	if(this->buffer.empty() == true)
	{
		return NULL;
	}
	return reinterpret_cast<void*>(&(this->buffer[0]));
}

size_t FileData::GetBufferSize(void)
{
	return this->buffer.size();
}

void FileData::SetBuffer(void* buffer, size_t size)
{
	this->buffer.clear();
	unsigned char* writeBuffer = reinterpret_cast<unsigned char*>(buffer);
	std::copy(writeBuffer, writeBuffer + size, std::back_inserter(this->buffer));
}
