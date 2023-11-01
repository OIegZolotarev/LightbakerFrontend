/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "common.h"
#include "persistent.h"
#include <nlohmann/json.hpp>
#include "application.h"
#include <string>
#include "igui_panel.h"
#include "ui_options_pages.h"


PersistentStorage::PersistentStorage(Application * appInstance)
{
	extern void RegisterOptions();
	extern uiOptionPage_t g_OptionsPages[(int)OptionsPage::Total];

	RegisterOptions();
	// TODO: calc this
	m_ApplicationProps.reserve(32);

	for (auto & it : g_OptionsPages)
	{
		for (auto& opt : it.properties)
		{
			m_ApplicationProps.push_back(&opt);
		}
	}

	//m_ApplicationProps.push_back(VariantValue((int)ApplicationSettings::ShowGround,		PropertiesTypes::Bool,		"Show ground", "Appeareance"));
	//m_ApplicationProps.push_back(VariantValue((int)ApplicationSettings::BackgroundColor1, PropertiesTypes::ColorRGBA,	"Background color", "Appeareance"));
	//m_ApplicationProps.push_back(VariantValue((int)ApplicationSettings::RebakeSceneAfterChanges, PropertiesTypes::Bool, "dynamically recompile lighting", "Compiling"));

	auto setting = GetSetting(ApplicationSettings::BackgroundColor1);
	setting->SetColorRGBA(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));

	setting = GetSetting(ApplicationSettings::RebakeSceneAfterChanges);
	setting->SetBool(true);

	LoadFromFile(appInstance);
}

void PersistentStorage::LoadFromFile(Application* appInstance)
{
	char persistent_file[1024];
	char* p = SDL_GetPrefPath("QuiteOldOrange", "LightBaker3000Frontend");
	sprintf_s(persistent_file, sizeof(persistent_file), "%s/persistent.json", p);

	FileData* fd = Application::Instance()->GetFileSystem()->LoadFile(persistent_file);

	if (!fd)
	{
		m_bFreshFile = true;
		return;
	}

	m_bFreshFile = false;
	
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

		if (j.contains("BakerSettings"))
		{
			appInstance->GetLightBakerApplication()->Settings()->FromJSON(j["BakerSettings"]);
		}
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

VariantValue* PersistentStorage::GetSetting(ApplicationSettings id)
{
	for (auto& it : m_ApplicationProps)
	{
		if (it->GetId() == (int)id)
			return it;
	}

	return nullptr;
}

bool PersistentStorage::GetSettingBool(ApplicationSettings id)
{
	VariantValue* p = GetSetting(id);
	assert(p);

	return p->GetAsBool();
}

nlohmann::json PersistentStorage::SerializeApplicationProperty(nlohmann::json& j, VariantValue* it)
{
	nlohmann::json prop_descriptor;

	prop_descriptor["id"] = it->GetId();
	prop_descriptor["display_name"] = it->DisplayName();
	prop_descriptor["type"] = it->GetType();
	

	switch (it->GetType())
	{
	case PropertiesTypes::Enum:
		prop_descriptor["value"] = it->GetEnumValue();
		break;
	case PropertiesTypes::Flags:
		prop_descriptor["value"] = it->GetFlags();
		break;
	case PropertiesTypes::Position:
		{
			nlohmann::json field;

			auto pos = it->GetPosition();

			field["x"] = pos[0];
			field["y"] = pos[1];
			field["z"] = pos[2];
			
			prop_descriptor["value"] = field;
		}
		break;
	case PropertiesTypes::ColorRGB:
	{
		nlohmann::json field;

		auto val = it->GetColorRGB();

		field["r"] = val[0];
		field["g"] = val[1];
		field["b"] = val[2];

		prop_descriptor["value"] = field;
	}
		break;
	case PropertiesTypes::ColorRGBA:
	{
		nlohmann::json field;

		auto val = it->GetColorRGBA();

		field["r"] = val[0];
		field["g"] = val[1];
		field["b"] = val[2];
		field["a"] = val[3];

		prop_descriptor["value"] = field;
	}
		break;
	case PropertiesTypes::Float:
		prop_descriptor["value"] = it->GetFloat();
		break;
	case PropertiesTypes::Angles:
	{
		nlohmann::json field;

		auto val = it->GetAngles();

		field["x"] = val[0];
		field["y"] = val[1];
		field["z"] = val[2];
		
		prop_descriptor["value"] = field;
	}
		break;
	case PropertiesTypes::Int:
		prop_descriptor["value"] = it->GetInt();
		break;
	case PropertiesTypes::SizeX:
		prop_descriptor["value"] = it->GetFloat();
		break;
	case PropertiesTypes::Bool:
		prop_descriptor["value"] = it->GetAsBool();
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

		VariantValue* descriptor = GetSetting((ApplicationSettings)id);

		if (!descriptor)
			continue;

		if (it["type"] != descriptor->GetType())
			continue;

		switch (descriptor->GetType())
		{
		case PropertiesTypes::Enum:
			//prop_descriptor["value"] = it.value.asEnum;
			descriptor->SetEnumValue(it["value"]);
			break;
		case PropertiesTypes::Flags:
			descriptor->SetFloat(it["value"]);
			break;
		case PropertiesTypes::Position:
		{
			nlohmann::json value = it["value"];

			glm::vec3 val;

			val[0] = value["x"];
			val[1] = value["y"];
			val[2] = value["z"];

			descriptor->SetPosition(val);
		}
		break;
		case PropertiesTypes::ColorRGB:
		{
			nlohmann::json value = it["value"];

			glm::vec3 val;

			val[0] = value["r"];
			val[1] = value["g"];
			val[2] = value["b"];

			descriptor->SetColorRGB(val);
		}
		break;
		case PropertiesTypes::ColorRGBA:
		{
			nlohmann::json value = it["value"];

			glm::vec4 val;

			val[0] = value["r"];
			val[1] = value["g"];
			val[2] = value["b"];
			val[3] = value["a"];

			descriptor->SetColorRGBA(val);
		}
		break;
		case PropertiesTypes::Float:
			//prop_descriptor["value"] = it.value.asFloat;
			descriptor->SetFloat(it["value"]);
			break;
		case PropertiesTypes::Angles:
		{
			nlohmann::json value = it["value"];

			glm::vec3 val;

			val[0] = value["x"];
			val[1] = value["y"];
			val[2] = value["z"];

			descriptor->SetAngles(val);
		}
		break;
		case PropertiesTypes::Int:
			descriptor->SetInt(it["value"]);
			break;
		case PropertiesTypes::SizeX:
			descriptor->SetFloat(it["value"]);
			break;
		case PropertiesTypes::Bool:
			descriptor->SetBool(it["value"]);
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

bool PersistentStorage::IsFreshFile()
{
	return m_bFreshFile;
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

		j["BakerSettings"] = Application::Instance()->GetLightBakerApplication()->Settings()->ToJSON();

		std::string data = j.dump(4);
		fprintf(fp, "%s", data.c_str());
		fclose(fp);
	}
	catch (std::exception& e)
	{
		e;
	}

	
}
