/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "goldsource_game_configuration.h"
#include "common.h"
#include "text_utils.h"
#include <boost/algorithm/string.hpp>

using namespace GoldSource;

GoldSource::HammerGameConfiguration::HammerGameConfiguration(std::string gameRootDir, GameEngines engineHint)
    : GameConfiguration("", gameRootDir)
{
    m_Engine      = engineHint;
    m_Description = "<Missing description>";

    switch (engineHint)
    {
    case GameEngines::GoldSource:
        ParseLiblistGam();
        break;
    case GameEngines::Xash3d:
        ParseGameInfo();
        break;
    }

    // TODO: fixme
    FileData *fd = FileSystem::Instance()->LoadFile("D:/temp/halflife_2020/fgd/halflife_2020.fgd");
    m_lstFGDData.push_back(new GoldSource::HammerFGDFile(fd));
    fd->UnRef();
}

GameConfiguration *HammerGameConfiguration::Clone()
{
    return new HammerGameConfiguration(*this);
}

HammerGameConfiguration::HammerGameConfiguration(std::string &savedFileName)
{    
    m_SavedFileName = savedFileName;    
    Deserialize(savedFileName);
}

void HammerGameConfiguration::EditDialog()
{
    static size_t selectedConf = 0;

    ImGui::InputText("Game name:", &m_Description);
    ImGui::InputText("Game directory:", &m_GameDirectory);

    ImGui::SeparatorText("Compilers:");
    ImGui::InputText("CSG program", &m_CompilationPrograms.csg);
    ImGui::InputText("BSP program", &m_CompilationPrograms.bsp);
    ImGui::InputText("VIS program", &m_CompilationPrograms.vis);
    ImGui::InputText("RAD program", &m_CompilationPrograms.rad);

    ImGui::SeparatorText("FGD Files");

    if (ImGui::BeginListBox("###FGDFilesListBox"))
    {
        size_t index = 0;
        for (auto &it : m_FGDFiles)
        {
            if (ImGui::Selectable(it.c_str(), index == selectedConf))
            {
                selectedConf = index;
            }

            index++;
        }

        ImGui::EndListBox();
    }

    if (ImGui::Button("Add"))
    {
        m_FGDFiles.push_back("<DUMMY>");
    }

    ImGui::SameLine();

    if (ImGui::Button("Remove"))
    {
        if (selectedConf > m_FGDFiles.size() - 1)
        {
            selectedConf = 0;
            return;
        }

        auto it = m_FGDFiles.begin();
        std::advance(it, selectedConf);

        m_FGDFiles.erase(it);
        selectedConf = 0;
    }
}

void HammerGameConfiguration::Serialize(std::string fileName) const
{
    if (fileName.empty())
    {
        fileName = GameConfigurationsManager::SuggestSaveFileName(m_Engine, m_Description) + ".json";
    }

    try
    {
        nlohmann::json j;
        nlohmann::json compilers;
        nlohmann::json fgdFiles;

        FILE *fp = FileSystem::OpenFileForWriting(fileName);

        if (!fp)
        {
            // TODO: throw some kind of error (presumably gracefull one)
            return;
        }

        switch (m_Engine)
        {
        case GameEngines::GoldSource:
            j["Engine"] = "GoldSource";
            break;
        case GameEngines::Xash3d:
            j["Engine"] = "Xash3d";
            break;
        default:
            break;
        
        }
        
        j["Description"]   = m_Description;
        j["GameDirectory"] = m_GameDirectory;

        compilers["CSG"] = m_CompilationPrograms.csg;
        compilers["BSP"] = m_CompilationPrograms.bsp;
        compilers["VIS"] = m_CompilationPrograms.vis;
        compilers["RAD"] = m_CompilationPrograms.rad;

        j["Compilers"] = compilers;

        for (auto &it : m_FGDFiles)
        {
            fgdFiles.push_back(it);
        }

        j["FGDFiles"] = fgdFiles;

        std::string data = j.dump(4);
        fprintf(fp, "%s", data.c_str());
        fclose(fp);
    }
    catch (std::exception &e)
    {
        e;
    }
}

void HammerGameConfiguration::Deserialize(std::string &fileName)
{
    FileData *fd = FileSystem::Instance()->LoadFile(fileName);

    std::string json_data = std::string(std::string_view((char *)fd->Data(), fd->Length()));
    nlohmann::json j      = nlohmann::json::parse(json_data);

    if (j.contains("Compilers"))
    {
        auto & compilers = j["Compilers"];

        m_CompilationPrograms.csg = compilers["CSG"];
        m_CompilationPrograms.bsp = compilers["BSP"];
        m_CompilationPrograms.vis = compilers["VIS"];
        m_CompilationPrograms.rad = compilers["RAD"];
    }

    if (j.contains("Description"))
        m_Description = j["Description"];

    if (j.contains("GameDirectory"))
        m_GameDirectory = j["GameDirectory"];

    if (j.contains("Engine"))
    {
        std::string engine = j["Engine"];

        if (engine == "GoldSource")
            m_Engine = GameEngines::GoldSource;
        else
            m_Engine = GameEngines::Xash3d;
    }

    fd->UnRef();
}

FGDEntityClass *HammerGameConfiguration::LookupFGDClass(std::string &classname)
{
    for (auto &it : m_lstFGDData)
    {
        auto fgdClass = it->FindEntityClass(classname);

        if (fgdClass)
            return fgdClass;
    }

    return nullptr;
}

void HammerGameConfiguration::ParseGameInfo()
{
    auto filePath = m_GameDirectory + "/gameinfo.txt";
    auto fd       = FileSystem::Instance()->LoadFile(filePath);

    auto lines = TextUtils::SplitTextSimple((char *)fd->Data(), fd->Length(), '\n');

    for (auto &line : lines)
    {
        if (!strncasecmp(line.c_str(), "title", 5))
        {
            auto descr = line.substr(5);
            boost::trim(descr);
            auto title = descr.substr(1, descr.size() - 2);
            SetDescription(title);
        }
    }

    fd->UnRef();
}

void HammerGameConfiguration::ParseLiblistGam()
{
    auto filePath = m_GameDirectory + "/liblist.gam";
    auto fd       = FileSystem::Instance()->LoadFile(filePath);

    auto lines = TextUtils::SplitTextSimple((char *)fd->Data(), fd->Length(), '\n');

    for (auto &line : lines)
    {
        if (!strncasecmp(line.c_str(), "game ", 5))
        {
            auto descr = line.substr(4);
            boost::trim(descr);
            auto title = descr.substr(1, descr.size() - 2);
            SetDescription(title);
        }
    }

    fd->UnRef();
}

HammerGameConfiguration::~HammerGameConfiguration()
{
    for (auto it : m_lstFGDData)
        delete it;

    m_lstFGDData.clear();
    m_FGDFiles.clear();
}
