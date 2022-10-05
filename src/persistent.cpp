#include "common.h"
#include "persistent.h"
#include <nlohmann/json.hpp>
#include "application.h"
#include <string>


PersistentStorage::PersistentStorage()
{
	LoadFromFile();
}

void PersistentStorage::LoadFromFile()
{
	char persistent_file[1024];
	char* p = SDL_GetPrefPath("QuiteOldOrange", "LightBaker3000Frontend");
	sprintf_s(persistent_file, sizeof(persistent_file), "%s/persistent.json", p);

	FileData* fd = Application::Instance()->GetFileSystem()->LoadFile(persistent_file);

	if (!fd)
		return;
	
	try
	{
		std::string json_data = std::string(std::string_view((char*)fd->Data(),fd->Length()));
		nlohmann::json j = nlohmann::json::parse(json_data);

		if (j.contains("MRU"))
		{
			auto MRU = j["MRU"];
			
			for (auto items : MRU)
			{
				m_lstMRUFiles.push_back(items.get<std::string>());
			}
		}

	}
	catch (std::exception& e)
	{
		e;
		Application::EPICFAIL(e.what());
	}

	delete fd;
}

PersistentStorage::~PersistentStorage()
{
	SaveToFile();
}

void PersistentStorage::PushMRUFile(const char* fileName)
{
	for (auto& it : m_lstMRUFiles)
	{
		if (!_stricmp(it.c_str(), fileName))
			return;
	}

	m_lstMRUFiles.push_front(fileName);

	while (m_lstMRUFiles.size() > 10)
		m_lstMRUFiles.pop_back();
}

std::list<std::string>& PersistentStorage::GetMRUFiles()
{
	return m_lstMRUFiles;
}

void PersistentStorage::SaveToFile()
{
	char persistent_file[1024];
	char* p = SDL_GetPrefPath("QuiteOldOrange", "LightBaker3000Frontend");
	sprintf_s(persistent_file, sizeof(persistent_file), "%s/persistent.json", p);

	try
	{		
		nlohmann::json j;		
		j["MRU"] = m_lstMRUFiles;

		std::string data = j.dump(4);

		FILE* fp = 0;
		
		fopen_s(&fp, persistent_file, "wb");

		if (!fp)
			return;

		fprintf(fp, "%s", data.c_str());
		fclose(fp);
	}
	catch (std::exception& e)
	{
		e;
	}

	
}
