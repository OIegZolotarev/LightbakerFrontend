/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "imgui_popups.h"
#include "io_scene.h"
#include "io_scene_lm2.h"
#include "popup_file_dialog.h"

const char *SceneIO::Description()
{
    return "SceneIO::Description()";
}

int SceneIO::Caps()
{
    return 0;
}

SceneIOManager::SceneIOManager()
{
    m_lstSerializers.push_back(new SceneIOLm2);
}

void SceneIOManager::SerializeScene(const char *fileName, Scene *pScene, SceneIOClasses format)
{
    auto it = std::find_if(begin(m_lstSerializers), end(m_lstSerializers),
                           [&](SceneIO *pTest) -> bool { return pTest->FormatID() == format; });

    // Should not be triggered
    if (it == m_lstSerializers.end())
    {
        Application::EPICFAIL("SceneIOManager::SerializeScene(): unknown format ID!");
    }

    (*it)->Serialize(fileName, pScene);
}

void SceneIOManager::PerformSavingDialog()
{
    

    auto lfd = FileDialog::Instance();

    lfd->SetTitle("Save document");
    lfd->SetFilters(".lm2");
    lfd->SetOnSelectCallback([&](std::string & fileName) 
        
        {
            Scene *pScene = Scene::ActiveInstance();

            SerializeScene(fileName.c_str(), pScene, SceneIOClasses::LM2);
        });

    PopupsManager::Instance()->ShowPopup(PopupWindows::FileDialog);
}
