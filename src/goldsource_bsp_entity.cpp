/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "goldsource_bsp_entity.h"
#include "text_utils.h"


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
				color[i] = std::stof(*it);
		}
    }

    m_vProperties.insert(kvData(key, value));
}

void BSPEntity::PopulateScene()
{
    auto scene = Application::GetMainWindow()->GetSceneRenderer()->GetScene();
    auto & classname = m_vProperties["classname"];

    if (classname == "light")
    {     
        auto newOrigin = origin;

        // јбсолютно ниху€ не пон€тно
        newOrigin.x = -origin.y;
        newOrigin.y = origin.z;
        newOrigin.z = origin.x;

        m_SceneEntity = scene->AddNewLight(newOrigin, LightTypes::Omni);
        
        auto ptr = m_SceneEntity.lock();

        if (ptr)
        {
            ptr->SetColor(color.xyz);
        }
    }
}


