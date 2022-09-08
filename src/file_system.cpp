#if WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "common.h"
#include "file_system.h"
#include <filesystem>

FileData::FileData(byte* data, size_t length, const char* name)
{
	if (data)
		m_pData = data;
	else
	{
		m_bOwnsData = true;
		m_pData = new byte[length];
	}

	m_szLength = length;

	m_Name = name;
}

FileData::~FileData()
{
	if (m_bOwnsData) delete[] m_pData;
}

size_t FileData::Length()
{
	return m_szLength;
}

byte* FileData::Data()
{
	return m_pData;
}

std::string& FileData::Name()
{
	return m_Name;
}

void FileData::Ref()
{
	m_nReferences++;
}

void FileData::UnRef()
{
	m_nReferences--;
	if (!m_nReferences)
		delete this;
}

IArchive::IArchive(const char* fileName)
{
	std::filesystem::path p = std::filesystem::path(fileName);
	auto c = std::filesystem::canonical(p);

	m_FileName = { c.u8string() };
	m_BaseName = { c.filename().u8string() };
}

IArchive::~IArchive()
{

}

FileData* IArchive::LoadFile(const char* name)
{
	return nullptr;
}

std::string& IArchive::ArchiveFileName()
{
	return m_FileName;
}

const char* IArchive::BaseFileName()
{
	return m_BaseName.c_str();
}

const std::vector<const char*> IArchive::AllFiles()
{
	return m_lstFiles;
}

bool IArchive::IsHidden()
{
	return m_bHideFromUser;
}

FileSystem::FileSystem()
{

}

FileSystem::~FileSystem()
{

}

FileData* FileSystem::LoadFile(const char* fileName)
{
	FileData* pResult = nullptr;

	for (auto& it : m_vecArchiveProviders)
	{
		if (pResult = it->LoadFile(fileName))
			return pResult;
	}

	FILE* fp = fopen(fileName,"rb");

	if (!fp)
		return pResult;

	size_t length = 0;
	byte* data = 0;

	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	data = new byte[length];
	rewind(fp);
	
	fread(data, 1, length,fp);
	fclose(fp);

	pResult = new FileData(data,length,fileName);

	return pResult;
}

std::string FileSystem::BaseName(std::string& fullPath)
{
	std::filesystem::path p = std::filesystem::path(fullPath);
	auto c = std::filesystem::canonical(p);
		
	return { c.stem().u8string() };
}

std::string FileSystem::ParentPath(std::string& fullPath)
{
	std::filesystem::path p = std::filesystem::path(fullPath);
	auto c = std::filesystem::canonical(p);

	return { c.parent_path().u8string() };
}
