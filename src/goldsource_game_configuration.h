/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "game_configuration.h"
#include "hammer_fgd.h"
#include "imgui_listview_ex.h"
#include "inputfield_ex.h"

namespace GoldSource
{
class HammerGameConfiguration : public GameConfiguration
{
    std::string m_GameName     = "Half-life";
    std::string m_ModDirectory = "valve";

    std::list<std::string> m_WadFiles;

    struct
    {
        std::string csg = "";
        std::string bsp = "";
        std::string vis = "";
        std::string rad = ""; // Не особо то и нужно :)
    } m_CompilationPrograms;

    
    std::list<std::string> m_FGDFiles;
    std::list<HammerFGDFile *> m_lstFGDData;

    void ParseLiblistGam();
    void ParseGameInfo();

    ListViewEx *m_pFGDListView;
    ListViewEx *m_pWadsListView;

    InputFieldEx *m_pCSGInputField;
    InputFieldEx *m_pBSPInputField;
    InputFieldEx *m_pVISInputField;
    InputFieldEx *m_pRADInputField;

    void InitListViewBindings();

  public:
    HammerGameConfiguration();
    HammerGameConfiguration(HammerGameConfiguration &other);
    HammerGameConfiguration(std::string & savedFileName);
    HammerGameConfiguration(std::string gameRootDir, GameEngines engineHint);
    
    ~HammerGameConfiguration();

    FGDEntityClass *LookupFGDClass(std::string &classname);

    void Deserialize(std::string & fileName);
    void Serialize(std::string fileName) const override;

    void EditDialog() override;

    void RenderGeneralUI();
    void RenderCompilerUI();
    void RenderFGDUI();

    GameConfiguration *Clone() override;
};

} // namespace GoldSource