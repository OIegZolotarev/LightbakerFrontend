/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/


#include "fs_folder_mount.h"



 FolderMount::FolderMount(const char *folder): IArchive(folder)
{
    // m_strNormalizedFullPath = FileSystem::MakeCanonicalPath(folder);
}

 FolderMount::~FolderMount()
{
}

FileData *FolderMount::LoadFile(const char *name)
{
    
    std::string fullPath = this->ArchiveFileName() + "/" + name;

    if (!FileSystem::FileExists(fullPath))
        return nullptr;


    return FileSystem::LoadFileFromOS(fullPath.c_str());
}
