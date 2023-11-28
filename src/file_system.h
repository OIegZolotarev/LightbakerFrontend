/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#pragma once

// FileData - general file wrapper class
//
class FileData
{
    bool   m_bOwnsData = false;
    byte * m_pData     = 0;
    size_t m_szLength  = 0;

    std::string m_Name;

    int    m_nReferences = 1;
    size_t m_offset;

    friend class FileSystem;
    void FlagOwnsData();

public:

    FileData(byte *data, size_t length, const char *name);
    ~FileData();

    size_t Length();

    byte *       Data();
    std::string &Name();

    void Ref();
    void UnRef();

    template <class T> T *Read(size_t nElements);

    std::string BaseName();
    std::string DirName();
};

enum class SeekOrigin
{
    Start,
    Relative,
    End
};

class IFileHandle
{
protected:
    size_t m_Offset = 0;
    std::string m_Name;

public:
    IFileHandle()         = default;
    virtual ~IFileHandle() = default;

    virtual size_t TotalSize()                                                      = 0;
    virtual size_t Read(size_t elementSize, size_t elementsCount, void *destBuffer) = 0;
    virtual bool   Seek(size_t position, SeekOrigin origin) = 0;
    virtual size_t Position() = 0;

        const char *FileName();
};

class FileHandleOS: public IFileHandle
{
    FILE *m_pFileHandle = nullptr;

public:
    FileHandleOS(const char* fileName);
    ~FileHandleOS();

    size_t TotalSize();
    size_t Position() override;
    size_t Read(size_t elementSize, size_t elementsCount, void *destBuffer) override;
    bool Seek(size_t position, SeekOrigin origin) override;


};

class FileHandleUncompressedArchive: public IFileHandle
{
    FILE *m_pArchiveFile = nullptr;
    
    size_t m_DataStart = 0;
    size_t m_DataLength = 0;
public:
    FileHandleUncompressedArchive(FILE *fpArchive, size_t dataStart, size_t dataLength);
    ~FileHandleUncompressedArchive();

    size_t Position() override;

    size_t Read(size_t elementSize, size_t elementsCount, void *destBuffer) override;

    bool Seek(size_t position, SeekOrigin origin) override;

    size_t TotalSize() override;
};

class IArchive
{
protected:
    std::string m_FileName;
    std::string m_BaseName;

    std::vector<const char *> m_lstFiles;

    bool m_bHideFromUser = false;

public:
    IArchive(const char *fileName);
    virtual ~IArchive();

    virtual FileData *LoadFile(const char *name);
    std::string &     ArchiveFileName();

    const char *                    BaseFileName();
    const std::vector<const char *> AllFiles();

    bool IsHidden();
};

class FileSystem
{
    std::vector<IArchive *> m_vecArchiveProviders;

public:
    FileSystem();
    ~FileSystem();

    static FileSystem *Instance();

    [[nodiscard]] FileData *LoadFile(const char *fileName);
    [[nodiscard]] FileData *LoadFile(std::string &fileName);

    void ChangeCurrentDirectoryToFileDirectory(const std::string &fileName);

    // Return file name without extension from path, file must exist!
    // Examples:
    //		/home/user/path/to/file.txt -> file
    //		C:\User\Documents\file.txt -> file
    std::string BaseNameFromPath(std::string &path);

    // Returns parent path from full file path, file must exist!
    // Examples:
    //      /home/user/path/to/file.txt -> /home/user/path/to/
    //      C:\User\Documents\file.txt -> C:\User\Documents\ 
	std::string BaseDirectoryFromPath(const std::string& fileName);

    // Return file extension with dot, file must exist!
    // Examples:
    //      /home/user/path/to/file.txt -> .txt
    //      C:\User\Documents\file.bsp -> .bsp
    std::string ExtensionFromPath(const std::string &path);

    // ��������� ��� ����� �� �������
    // ����������� ��� ���� ������� � ��� �� �������� ��� � �������� ����
    // ��������� �������
    //	{0} - ������� � ������� ��������� ����
    //	{1} - ��� ����� ��� ����������
    //	{2} - ���������� ����� (� ������ - ".txt")
    //
    //	������:
    //		MakeTemplatePath("/home/user/file.obj","{0}.lm.png") -> /home/user/file.lm.png
    std::string MakeTemplatePath(const char *fileName, const char *templ);

    static bool FileExists(std::string fileName);

    int CopyFile(const char *srcPath, const char *dstPath);

// Версии для несуществующих путей
#define PATHSEPARATOR(c) ((c) == '\\' || (c) == '/')

    static std::string ExtractFilePath(const char *path);

    static std::string ExtractFileBase(const char *path);

    static std::string ExtractFileExtension(const char *path);

    static std::string         ExtractFileName(const char *path);
    static [[nodiscard]] FILE *OpenFileForWriting(std::string &fileName);
    static void                MakeDir(std::string path);

    static std::string SanitizeFileName(std::string result);


    IFileHandle *OpenFileHandle(const char *filePath);
};
