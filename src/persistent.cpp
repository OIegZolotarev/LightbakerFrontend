#include "common.h"
#include "persistent.h"
#include <nlohmann/json.hpp>
#include "application.h"
#include <string>
#include "igui_panel.h"


PersistentStorage::PersistentStorage()
{
	m_ApplicationProps.push_back(propsData_t((int)ApplicationSettings::ShowGround,		PropertiesTypes::Bool,		"Show ground", "Appeareance"));
	m_ApplicationProps.push_back(propsData_t((int)ApplicationSettings::BackgroundColor, PropertiesTypes::ColorRGBA,	"Background color", "Appeareance"));

	auto setting = GetSetting(ApplicationSettings::BackgroundColor);
	setting->value.asColorRGBA = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);

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

		if (j.contains("ApplicationSettings"))
			ParseApplicationSettings(j["ApplicationSettings"]);
// 	
// 		std::list<nlohmann::json> panels_state;
// 
// 		for (auto& it : PanelsId::_values())
// 		{
// 			if (it._value == PanelsId::None)
// 				continue;
// 
// 			if (it._value == PanelsId::MaxPanels)
// 				continue;
// 
// 			nlohmann::json panel_desc;
// 			panel_desc["Valid"] = m_bPanelsValid[it._value];
// 			panel_desc["Id"] = it._to_string();
// 
// 			panels_state.push_back(panel_desc);
// 		}
// 
// 		j["PanelsState"] = panels_state;

		if (j.contains("PanelsState"))
		{
			auto items = j["PanelsState"];

			for (auto it : items)
			{
				PanelsId id = PanelsId::_from_string(it["Id"].get<std::string>().c_str());
				bool isValid = it["Valid"];

				m_bPanelsValid[id._value] = isValid;

			}
		}

	}
	catch (std::exception& e)
	{
		e;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Exception while parsing persistent data", e.what(), 0);
		//Application::EPICFAIL(e.what());
	}

	delete fd;
}

PersistentStorage::~PersistentStorage()
{
	SaveToFile();
}

void PersistentStorage::PushMRUFile(const char* fileName)
{
	auto p = std::filesystem::path(fileName);
	auto f = std::filesystem::canonical(p);

	std::string canonicalFileName = { f.string() };

	for (auto& it : m_lstMRUFiles)
	{
		if (!_stricmp(it.c_str(), canonicalFileName.c_str()))
			return;
	}

	m_lstMRUFiles.push_front(canonicalFileName);

	while (m_lstMRUFiles.size() > 10)
		m_lstMRUFiles.pop_back();
}

std::list<std::string>& PersistentStorage::GetMRUFiles()
{
	return m_lstMRUFiles;
}

propsData_t* PersistentStorage::GetSetting(ApplicationSettings id)
{
	for (auto& it : m_ApplicationProps)
	{
		if (it.id == (int)id)
			return &it;
	}

	return nullptr;
}

nlohmann::json PersistentStorage::SerializeApplicationProperty(nlohmann::json& j, propsData_t& it)
{
	nlohmann::json prop_descriptor;

	prop_descriptor["id"] = it.id;
	prop_descriptor["display_name"] = it.display_name;
	prop_descriptor["type"] = it.type;
	

	switch (it.type)
	{
	case PropertiesTypes::Enum:
		prop_descriptor["value"] = it.value.asEnum;
		break;
	case PropertiesTypes::Flags:
		prop_descriptor["value"] = it.value.asFlags;
		break;
	case PropertiesTypes::Position:
		{
			nlohmann::json field;

			field["x"] = it.value.asPosition[0];
			field["y"] = it.value.asPosition[1];
			field["z"] = it.value.asPosition[2];
			
			prop_descriptor["value"] = field;
		}
		break;
	case PropertiesTypes::ColorRGB:
	{
		nlohmann::json field;

		field["r"] = it.value.asColorRGB[0];
		field["g"] = it.value.asColorRGB[1];
		field["b"] = it.value.asColorRGB[2];

		prop_descriptor["value"] = field;
	}
		break;
	case PropertiesTypes::ColorRGBA:
	{
		nlohmann::json field;

		field["r"] = it.value.asColorRGBA[0];
		field["g"] = it.value.asColorRGBA[1];
		field["b"] = it.value.asColorRGBA[2];
		field["a"] = it.value.asColorRGBA[3];

		prop_descriptor["value"] = field;
	}
		break;
	case PropertiesTypes::Float:
		prop_descriptor["value"] = it.value.asFloat;
		break;
	case PropertiesTypes::Angles:
	{
		nlohmann::json field;

		field["x"] = it.value.asAngles[0];
		field["y"] = it.value.asAngles[1];
		field["z"] = it.value.asAngles[2];
		
		prop_descriptor["value"] = field;
	}
		break;
	case PropertiesTypes::Int:
		prop_descriptor["value"] = it.value.asInt;
		break;
	case PropertiesTypes::SizeX:
		prop_descriptor["value"] = it.value.asFloat;
		break;
	case PropertiesTypes::Bool:
		prop_descriptor["value"] = it.value.asBool;
		break;
	default:
		break;

	}

	return prop_descriptor;
}

void PersistentStorage::ParseApplicationSettings(nlohmann::json j)
{
	for (auto& it : j)
	{
		int id = it["id"];

		propsData_t* descriptor = GetSetting((ApplicationSettings)id);

		if (!descriptor)
			continue;

		if (it["type"] != descriptor->type)
			continue;

		switch (descriptor->type)
		{
		case PropertiesTypes::Enum:
			//prop_descriptor["value"] = it.value.asEnum;
			descriptor->value.asEnum = it["value"];
			break;
		case PropertiesTypes::Flags:
			descriptor->value.asFloat = it["value"];
			break;
		case PropertiesTypes::Position:
		{
			nlohmann::json value = it["value"];

			descriptor->value.asPosition[0] = value["x"];
			descriptor->value.asPosition[1] = value["y"];
			descriptor->value.asPosition[2] = value["z"];
		}
		break;
		case PropertiesTypes::ColorRGB:
		{
			nlohmann::json value = it["value"];

			descriptor->value.asColorRGB[0] = value["r"];
			descriptor->value.asColorRGB[1] = value["g"];
			descriptor->value.asColorRGB[2] = value["b"];
		}
		break;
		case PropertiesTypes::ColorRGBA:
		{
			nlohmann::json value = it["value"];

			descriptor->value.asColorRGBA[0] = value["r"];
			descriptor->value.asColorRGBA[1] = value["g"];
			descriptor->value.asColorRGBA[2] = value["b"];
			descriptor->value.asColorRGBA[3] = value["a"];
		}
		break;
		case PropertiesTypes::Float:
			//prop_descriptor["value"] = it.value.asFloat;
			descriptor->value.asFloat = it["value"];
			break;
		case PropertiesTypes::Angles:
		{
			nlohmann::json value = it["value"];

			descriptor->value.asAngles[0] = value["x"];
			descriptor->value.asAngles[1] = value["y"];
			descriptor->value.asAngles[2] = value["z"];
		}
		break;
		case PropertiesTypes::Int:
			descriptor->value.asInt = it["value"];
			break;
		case PropertiesTypes::SizeX:
			descriptor->value.asFloat = it["value"];
			break;
		case PropertiesTypes::Bool:
			descriptor->value.asBool = it["value"];
			break;
		default:
			break;

		}

		//prop_descriptor["display_name"] = it.display_name;
		//prop_descriptor["type"] = it.type;
	}
}

bool PersistentStorage::IsPanelAtValidPosition(PanelsId id)
{
	return m_bPanelsValid[id._value];
}

void PersistentStorage::FlagPanelIsAtValidPosition(PanelsId id)
{
	m_bPanelsValid[id._value] = true;
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
		
		FILE* fp = 0;
		
		fopen_s(&fp, persistent_file, "wb");

		if (!fp)
			return;

		std::list<nlohmann::json> props;

		for (auto& it : m_ApplicationProps)
		{
			props.push_back(SerializeApplicationProperty(j,it));
		}

		j["ApplicationSettings"] = props;

		std::list<nlohmann::json> panels_state;

		for (auto& it : PanelsId::_values())
		{
			if (it._value == PanelsId::None)
				continue;

			if (it._value == PanelsId::MaxPanels)
				continue;

			nlohmann::json panel_desc;
			panel_desc["Valid"] = m_bPanelsValid[it._value];
			panel_desc["Id"] = it._to_string();

			panels_state.push_back(panel_desc);
		}

		j["PanelsState"] = panels_state;


		std::string data = j.dump(4);
		fprintf(fp, "%s", data.c_str());
		fclose(fp);
	}
	catch (std::exception& e)
	{
		e;
	}

	
}
