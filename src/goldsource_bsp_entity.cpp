/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "goldsource_bsp_entity.h"
#include "text_utils.h"
#include "wad_textures.h"
#include "goldsource_game_configuration.h"


using namespace GoldSource;

BSPEntity::BSPEntity()
{
    
}

BSPEntity::~BSPEntity()
{

}

void BSPEntity::SetKeyValue(std::string& key, std::string& value)
{
    if (key == "origin")
    {
        auto digits = TextUtils::SplitTextWhitespaces(value.c_str(), value.size());

        if (digits.size() == 3)
        {
            int i = 0;
            for (auto it = digits.begin(); it != digits.end(); it++, i++)
                origin[i] = std::stof(*it);
        }


    }
    else if (key == "_light")
    {
		auto digits = TextUtils::SplitTextWhitespaces(value.c_str(), value.size());

		if (digits.size() == 4)
		{
			int i = 0;
			for (auto it = digits.begin(); it != digits.end(); it++, i++)
				color[i] = std::stof(*it) / 255.f;
		}
    }
    else if (key == "_wad")
    {
        // E:\Games\Half-Life\valve\halflife.wad;
        // E:\Games\Half-Life\valve\liquids.wad;
        // E:\Games\Half-Life\valve\xeno.wad;
        // E:\Games\Half-Life\valve\decals.wad;
        // E:\Games\Hammer\ZHLT\zhlt.wad;

        auto wadFiles = TextUtils::SplitTextSimple(value.c_str(), value.length(), ';');


        for (auto & it : wadFiles)
		{
			auto baseName = FileSystem::Instance()->ExtractFileBase(it.c_str());


            // ��������� ���, ���� ������� ���� ������ ���������                       
            std::string fullPath = "D:/Games/Steam/steamapps/common/Half-Life/valve/" + baseName + ".wad";

            WADPool::Instance()->LoadWad(fullPath.c_str());

        }

    }

    m_vProperties.insert(kvData(key, value));
}

void BSPEntity::PopulateScene()
{
    auto scene = Application::GetMainWindow()->GetSceneRenderer()->GetScene();    
    auto & classname = m_vProperties["classname"];

    GameConfiguration *pConfig = scene->UsedGameConfiguration();

    if (typeid(pConfig) == typeid((HammerGameConfiguration*)0))
    {
        GoldSource::HammerGameConfiguration *hammerConfig = (HammerGameConfiguration *)pConfig;

        m_pFGDClass = hammerConfig->LookupFGDClass(classname);
    }

    if (classname == "light")
    {     
        m_SceneEntity = scene->AddNewLight(ConvertOriginToSceneSpace(), LightTypes::Omni, false);
        
        auto ptr = m_SceneEntity.lock();

        if (ptr)
        {
            ptr->SetColor(color.xyz);
        }

        ptr->CopyProperties(std::move(m_vProperties));
    }
    else if (classname == "light_environment")
    {
		m_SceneEntity = scene->AddNewLight(ConvertOriginToSceneSpace(), LightTypes::Direct, false);

		auto ptr = m_SceneEntity.lock();

		if (ptr)
		{
			ptr->SetColor(color.xyz);
		}

		ptr->CopyProperties(std::move(m_vProperties));
    }

    else
    {
		m_SceneEntity = scene->AddNewGenericEntity();

		auto ptr = m_SceneEntity.lock();

		if (ptr)
		{
			ptr->SetColor(color.xyz);
		}

		ptr->CopyProperties(std::move(m_vProperties));
        ptr->SetPosition(ConvertOriginToSceneSpace());
        ptr->FlagDataLoaded();
        
        ptr->SetMins(glm::vec3(-4, -4, -4));
        ptr->SetMaxs(glm::vec3(4, 4, 4));
    }

    
}

bool BSPEntity::UpdateProperties()
{
    auto ptr = m_SceneEntity.lock();

    if (!ptr)
        return false;

    m_vProperties = ptr->GetProperties();

	// ��������� ����� �� �������
	//newOrigin.x = -origin.y;
	//newOrigin.y = origin.z;
	//newOrigin.z = -origin.x;

    auto origin = ptr->GetPosition();

    std::string newOrigin = std::format("{0} {1} {2}", -origin.z, -origin.x, origin.y);

    m_vProperties["origin"] = newOrigin;

    if (ptr->IsLightEntity())
    {
        LightEntity* pLight = (LightEntity*)ptr.get();
        auto _light = ConvertLightColorAndIntensity(pLight);


        m_vProperties["_light"] = std::format("{0} {1} {2} {3}", _light.r, _light.g, _light.b, _light.a);
        
    }

    return true;
}



void BSPEntity::Export(FILE* fp)
{
    fprintf(fp, "{\n");

    for(auto kv: m_vProperties)
    {
        auto & key = kv.first;
        auto value = kv.second;


        // TODO: ��������� �������������
        //TextUtils::ReplaceAll(value, "\"", "\\"");

        fprintf(fp,"\"%s\" \"%s\"\n", key.c_str(), value.c_str());
    }

    fprintf(fp, "}\n");
}

glm::vec3 BSPEntity::ConvertOriginToSceneSpace()
{
	auto newOrigin = origin;

	// ��������� ����� �� �������
	newOrigin.x = -origin.y;
	newOrigin.y = origin.z;
	newOrigin.z = -origin.x;

    return newOrigin;

}

glm::vec3 BSPEntity::ConvertOriginFromSceneSpace(glm::vec3 origin)
{
    return glm::vec3{ -origin.z, -origin.x, origin.y };
}

glm::vec4 BSPEntity::ConvertLightColorAndIntensity(LightEntity * pEntity)
{
    auto color = pEntity->GetColor() * 255.f;
    auto intensity = pEntity->GetIntensity();

    return glm::vec4(color.r, color.g, color.b, intensity);
}

