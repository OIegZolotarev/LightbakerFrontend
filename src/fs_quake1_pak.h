/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/


#pragma once

#include "fs_core.h"
#include "common.h"

typedef struct mq1pakfile_s 
{
    char name[56];
    uint32_t  offset;
    uint32_t length;
}mq1pakfile_t;

typedef struct mq1pakhdr_s 
{
    int32_t hdr;
    uint32_t files_offset;
    uint32_t files_length;
} mq1pakhdr_t;

class Quake1Pak: public IArchive
{
    FILE *                    m_pFileHandle;
    std::vector<mq1pakfile_t> m_vFiles;    
public:
    Quake1Pak(const char *fileName);
    ~Quake1Pak();

    FileData *LoadFile(const char *name) override;
};
