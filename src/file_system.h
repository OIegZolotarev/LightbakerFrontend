/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

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

	std::string BaseName();
	std::string DirName();
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

	[[nodiscard]] FileData* LoadFile(const char* fileName);
	[[nodiscard]] FileData* LoadFile(std::string& fileName);
	
	std::string ParentPath(std::string& param1);
	void ChangeCurrentDirectoryToFileDirectory(const std::string& fileName);

	// ���������� ��� ����� ��� ���������� ��� �������� ����:
	// /home/user/path/to/file.txt -> file
	// C:\User\Documents\file.txt -> file
	std::string BaseNameFromPath(std::string& path);

	// ���������� ������� � ������� ��������� ����:
	// /home/user/path/to/file.txt -> /home/user/path/to/
	// C:\User\Documents\file.txt -> C:\User\Documents\ 
	std::string BaseDirectoryFromPath(const std::string& fileName);

	// ���������� ���������� ����� � ���� � ������:
	// /home/user/path/to/file.txt -> .txt
	// C:\User\Documents\file.bsp -> .bsp
	std::string ExtensionFromPath(const std::string& path);

	// ��������� ��� ����� �� �������
	// ����������� ��� ���� ������� � ��� �� �������� ��� � �������� ����
	// ��������� �������
	//	{0} - ������� � ������� ��������� ����
	//	{1} - ��� ����� ��� ����������
	//	{2} - ���������� ����� (� ������ - ".txt")
	// 
	//	������:
	//		MakeTemplatePath("/home/user/file.obj","{0}.lm.png") -> /home/user/file.lm.png
	std::string MakeTemplatePath(const char* fileName, const char* templ);

	bool FileExists(std::string fileName);

	int CopyFile(const char* srcPath, const char* dstPath);
	
};

