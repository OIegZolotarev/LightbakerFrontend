/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"
#include "fs_core.h"

class FolderMount : public IArchive
{
    // std::string m_strNormalizedFullPath;    

public:
    FolderMount(const char *folder);
    ~FolderMount();

    FileData *LoadFile(const char *name) override;
};
