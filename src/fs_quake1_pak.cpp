/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#define _CRT_SECURE_NO_WARNINGS

#include "application.h"
#include "common.h"
#include "fs_quake1_pak.h"
#include <direct.h>

 Quake1Pak::Quake1Pak(const char *fileName) : IArchive(fileName)
{
     m_pFileHandle = fopen(fileName, "rb");

     if (!m_pFileHandle)
         return;

     mq1pakhdr_t hdr;

     fread(&hdr, 1, sizeof(mq1pakhdr_t), m_pFileHandle);

     fseek(m_pFileHandle, hdr.files_offset, SEEK_SET);


     m_vFiles.resize(hdr.files_length / sizeof(mq1pakfile_t));

     fread(m_vFiles.data(), sizeof(mq1pakfile_t), m_vFiles.size(), m_pFileHandle);

 }

Quake1Pak::~Quake1Pak()
{
    FreeVector(m_vFiles);

    if (m_pFileHandle)
        fclose(m_pFileHandle);
}

FileData *Quake1Pak::LoadFile(const char *name)
{
    for (auto & it: m_vFiles)
    {
        if (!strcmp(it.name, name))
        {
            FileData * pResult = new FileData(nullptr, it.length + 1, name);            

            fseek(m_pFileHandle, it.offset, SEEK_SET);
            fread(pResult->Data(), it.length, 1, m_pFileHandle);

            pResult->Data()[it.length] = 0;

            return pResult;
        }
    }

    return nullptr;
}
