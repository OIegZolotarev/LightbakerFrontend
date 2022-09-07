#pragma once

class FileData
{
	bool m_bOwnsData = false;
	byte* m_pData = 0;
	size_t m_szLength = 0;
	std::string m_Name;

	int m_nReferences = 1;
	size_t m_offset;
public:

	FileData(byte* data, size_t length, const char* name);
	~FileData();

	size_t Length();

	byte* Data();
	std::string& Name();

	void Ref();
	void UnRef();

	template<class T>
	T* Read(size_t nElements);
};

template<class T>
T* FileData::Read(size_t nElements)
{
	size_t elementSize = sizeof(T);

	size_t retOffset = m_offset;
	m_offset += elementSize * nElements;

	return (T*)&m_pData[retOffset];
}

class IArchive
{
protected:
	std::string m_FileName;
	std::string m_BaseName;

	std::vector<const char*> m_lstFiles;

	bool m_bHideFromUser = false;

public:
	IArchive(const char* fileName);
	virtual ~IArchive();

	virtual FileData* LoadFile(const char* name);
	std::string& ArchiveFileName();

	const char* BaseFileName();
	const std::vector<const char*> AllFiles();

	bool IsHidden();	
};

class FileSystem
{
	std::vector<IArchive*> m_vecArchiveProviders;

public:
	FileSystem();
	~FileSystem();

	FileData* LoadFile(const char* fileName);
};

