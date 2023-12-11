/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#ifndef LINUX
#define _CRT_SECURE_NO_WARNINGS

#include <corecrt_io.h>
#include <direct.h>

#endif

#include "application.h"
#include "common.h"
#include "file_system.h"
#include <boost/algorithm/string/replace.hpp>
#include <filesystem>

FileData::FileData(byte *data, size_t length, const char *name)
{
    if (data)
        m_pData = data;
    else
    {
        m_bOwnsData = true;
        m_pData     = new byte[length];
    }

    m_szLength = length;

    m_Name = name;
}

FileData::~FileData()
{
    if (m_bOwnsData)
        delete[] m_pData;
}

size_t FileData::Length()
{
    return m_szLength;
}

byte *FileData::Data()
{
    return m_pData;
}

std::string &FileData::Name()
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

std::string FileData::BaseName()
{
    return Application::GetFileSystem()->BaseNameFromPath(m_Name);
}

std::string FileData::DirName()
{
    return Application::GetFileSystem()->BaseDirectoryFromPath(m_Name);
}

void FileData::FlagOwnsData()
{
    m_bOwnsData = true;
}

IArchive::IArchive(const char *fileName)
{
    std::filesystem::path p = std::filesystem::path(fileName);
    auto                  c = std::filesystem::canonical(p);

    m_FileName = c.string();
    m_BaseName = c.filename().string();
}

IArchive::~IArchive()
{
}

FileData *IArchive::LoadFile(const char *name)
{
    return nullptr;
}

std::string &IArchive::ArchiveFileName()
{
    return m_FileName;
}

const char *IArchive::BaseFileName()
{
    return m_BaseName.c_str();
}

const std::vector<const char *> IArchive::AllFiles()
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

FileSystem *FileSystem::Instance() 
{
    return Application::GetFileSystem();
}

FileData *FileSystem::LoadFile(const char *fileName)
{
    FileData *pResult = nullptr;

    for (auto &it : m_vecArchiveProviders)
    {
        if (pResult = it->LoadFile(fileName))
            return pResult;
    }

    FILE *fp = fopen(fileName, "rb");

    if (!fp)
        return pResult;

    size_t length = 0;
    byte * data   = 0;

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    data   = new byte[length + 1];
    memset(data, 0, length + 1);

    rewind(fp);

    fread(data, 1, length, fp);
    fclose(fp);

    std::filesystem::path p = std::filesystem::path(fileName);
    auto                  c = std::filesystem::canonical(p);

    pResult = new FileData(data, length, c.string().c_str());
    pResult->FlagOwnsData();

    return pResult;
}

FileData *FileSystem::LoadFile(std::string &fileName)
{
    return LoadFile(fileName.c_str());
}

std::string FileSystem::BaseNameFromPath(std::string &fullPath)
{
    std::filesystem::path p = std::filesystem::path(fullPath);
    auto                  c = std::filesystem::canonical(p);

    return c.stem().string();
}

void FileSystem::ChangeCurrentDirectoryToFileDirectory(const std::string &fileName)
{
    std::filesystem::path p    = std::filesystem::path(fileName);
    auto                  c    = std::filesystem::canonical(p);
    std::string           path = c.parent_path().string();

#ifndef LINUX
    _chdir(path.c_str());
#else
    chdir(path.c_str());
#endif
}

std::string FileSystem::BaseDirectoryFromPath(const std::string &fileName)
{
    std::filesystem::path p           = std::filesystem::path(fileName);
    auto                  c           = std::filesystem::canonical(p);
    std::string           parent_path = c.parent_path().string();

    return parent_path;
}

std::string FileSystem::ExtensionFromPath(const std::string &filePath)
{
    std::filesystem::path p    = std::filesystem::path(filePath);
    auto                  c    = std::filesystem::canonical(p);
    std::string           path = c.extension().string();

    return path;
}

bool FileSystem::FileExists(std::string fileName)
{
#ifdef LINUX
    return access(fileName.c_str(), 0) == 0;
#else
    return _access(fileName.c_str(), 0) == 0;
#endif
}

int FileSystem::CopyFile(const char *srcPath, const char *dstPath)
{
    FILE *f1, *f2;

    f1 = fopen(srcPath, "rb");
    f2 = fopen(dstPath, "wb");

    char   buffer[BUFSIZ];
    size_t n;

    while ((n = fread(buffer, sizeof(char), sizeof(buffer), f1)) > 0)
    {
        if (fwrite(buffer, sizeof(char), n, f2) != n)
            return -1;
    }

    fclose(f1);
    fclose(f2);

    return 0;
}

std::string FileSystem::ExtractFilePath(const char *path)
{
    char *src;

    src = (char *)path + strlen(path) - 1;

    //
    // back up until a \ or the start
    //
    while (src != path && !PATHSEPARATOR(*(src - 1)))
        src--;

    std::string_view view = std::string_view(path, src - path);
    return std::string(view);
}

std::string FileSystem::ExtractFileBase(const char *path)
{
    char *src;

    src = (char *)path + strlen(path) - 1;

    while (src != path && !PATHSEPARATOR(*(src - 1)))
        src--;

    std::string result;

    while (*src && *src != '.')
    {
        result += *src++;
    }

    return result;
}

std::string FileSystem::ExtractFileExtension(const char *path)
{
    char *src;

    src = (char *)path + strlen(path) - 1;

    //
    // back up until a . or the start
    //
    while (src != path && *(src - 1) != '.')
        src--;

    if (src == path)
    {
        return "";
    }

    std::string result;

    while (*src)
        result += *src++;

    return result;
}

std::string FileSystem::ExtractFileName(const char *path)
{
    auto name = ExtractFileBase(path);
    auto ext  = ExtractFileExtension(path);

    return name + "." + ext;
}

FILE *FileSystem::OpenFileForWriting(std::string &fileName)
{
    FILE *fp = 0;

#ifdef LINUX
    fp = fopen(fileName.c_str());
#else
    fopen_s(&fp, fileName.c_str(), "wb");
#endif

    if (!fp)
        return nullptr;

    return fp;
}

void FileSystem::MakeDir(std::string path)
{
    std::filesystem::create_directory(path);
}

std::string FileSystem::SanitizeFileName(std::string name)
{
    std::string result;

    result.reserve(name.size());

    // \ / : * ? " < > |

    for (auto &ch : name)
    {
        switch (ch)
        {
        case '\\':
        case '/':
        case ':':
        case '*':
        case '?':
        case '"':
        case '<':
        case '>':
        case '|':
            result = '#';
            break;
        default:
            result += ch;
        }
    }
    return result;
}

IFileHandle *FileSystem::OpenFileHandle(const char *filePath)
{
    // OS-only for now
    return new FileHandleOS(filePath);
}

// ��������� ��� ����� �� �������
// ����������� ��� ���� ������� � ��� �� �������� ��� � �������� ����
// ��������� �������
//	{0} - ������� � ������� ��������� ����
//	{1} - ��� ����� ��� ����������
//	{2} - ���������� ����� (� ������ - ".txt")
//
//	������:
//		MakeTemplatePath("/home/user/file.obj","{0}.lm.png") -> /home/user/file.lm.png
std::string FileSystem::MakeTemplatePath(const char *fileName, const char *templ)
{
#ifndef LINUX

    std::filesystem::path p = std::filesystem::path(fileName);
    auto                  c = std::filesystem::canonical(p);

    std::string directory = c.parent_path().string();
    std::string baseName  = c.stem().string();
    std::string extension = c.extension().string();

    std::string result = std::vformat(templ, std::make_format_args(directory, baseName, extension));
    return result;
#endif
}

FileHandleOS::FileHandleOS(const char *fileName)
{
    m_pFileHandle = fopen(fileName, "rb");
    m_Name        = fileName;
}

FileHandleOS::~FileHandleOS()
{
    if (m_pFileHandle)
        fclose(m_pFileHandle);
}

size_t FileHandleOS::TotalSize()
{
    fseek(m_pFileHandle, 0, SEEK_END);
    size_t len = ftell(m_pFileHandle);
    fseek(m_pFileHandle, m_Offset, SEEK_SET);

    return len;
}

size_t FileHandleOS::Position()
{
    return m_Offset;
}

size_t FileHandleOS::Read(size_t elementSize, size_t elementsCount, void *destBuffer)
{
    size_t r = fread(destBuffer, elementSize, elementsCount, m_pFileHandle);
    m_Offset += r;
    return r;
}

bool FileHandleOS::Seek(size_t position, SeekOrigin origin)
{
    int r = 0;

    switch (origin)
    {
    case SeekOrigin::Start:
        r = fseek(m_pFileHandle, (long)position, SEEK_SET);
        break;
    case SeekOrigin::Relative:
        r = fseek(m_pFileHandle, (long)position, SEEK_CUR);
        break;
    case SeekOrigin::End:
        r = fseek(m_pFileHandle, (long)position, SEEK_END);
        break;
    default:
        break;
    }

    return !r;
}

FileHandleUncompressedArchive::FileHandleUncompressedArchive(FILE *fpArchive, size_t dataStart, size_t dataLength)
{
    m_pArchiveFile = fpArchive;
    m_DataStart    = dataStart;
    m_DataLength   = dataLength;

    _fseeki64(m_pArchiveFile, m_DataStart, SEEK_SET);
}

FileHandleUncompressedArchive::~FileHandleUncompressedArchive()
{
}

size_t FileHandleUncompressedArchive::Position()
{
    return m_Offset;
}

size_t FileHandleUncompressedArchive::Read(size_t elementSize, size_t elementsCount, void *destBuffer)
{
    size_t elementsRead = elementsCount;
    size_t bytesToRead = elementSize * elementsCount;
    size_t bytesLeft   = m_DataLength - (m_Offset - m_DataStart);

    if (bytesLeft < bytesToRead)
    {
        bytesToRead = bytesLeft;
        elementsRead = bytesToRead / elementSize;
    }

    fread(destBuffer, elementSize, elementsRead, m_pArchiveFile);

    return elementsRead;
}

bool FileHandleUncompressedArchive::Seek(size_t position, SeekOrigin origin)
{
    if (position > m_DataLength)
        return false;

    switch (origin)
    {
    case SeekOrigin::Start:
        m_Offset = position;
        break;
    case SeekOrigin::Relative:
        m_Offset += position;
        break;
    case SeekOrigin::End:
        m_Offset = m_DataLength - 1 - position;
        break;
    default:
        break;
    
    }
    
    fseek(m_pArchiveFile, m_Offset, SEEK_SET);

    return true;
}

size_t FileHandleUncompressedArchive::TotalSize()
{
    return m_DataLength;
}

const char *IFileHandle::FileName()
{
    return m_Name.c_str();
}
