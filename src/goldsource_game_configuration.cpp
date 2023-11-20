/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/


#include "application.h"
#include "common.h"
#include "text_utils.h"
#include <boost/algorithm/string.hpp>
#include "goldsource_game_configuration.h"

using namespace GoldSource;

GoldSource::HammerGameConfiguration::HammerGameConfiguration(std::string gameRootDir, GameEngines engineHint)
    : GameConfiguration("", gameRootDir), m_EngineHint(engineHint)
{
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

void HammerGameConfiguration::EditDialog()
{
    ImGui::InputText("Game name:", &m_Description);
    ImGui::InputText("Game directory:", &m_GameDirectory);

    //     if (ImGui::BeginChildFrame(532, ImVec2(0, 0)))
    //     {
    ImGui::SeparatorText("Compilers:");
    ImGui::InputText("CSG program", &m_CompilationPrograms.csg);
    ImGui::InputText("BSP program", &m_CompilationPrograms.bsp);
    ImGui::InputText("VIS program", &m_CompilationPrograms.vis);
    ImGui::InputText("RAD program", &m_CompilationPrograms.rad);
    //
    //         ImGui::EndChildFrame();
    //     }
}

void HammerGameConfiguration::Serialize(std::string fileName) const
{
    if (fileName.empty())
    {
        char *p  = SDL_GetPrefPath(SDL_ORGANIZATION, SDL_APP_NAME);
        fileName = std::string(p) + "game_configs/" + m_Description + ".json";

        if (!FileSystem::FileExists(std::string(p) + "game_configs/"))
        {
            FileSystem::MakeDir(std::string(p) + "game_configs/");
        }
    }

    try
    {
        nlohmann::json j;
        nlohmann::json compilers;
        nlohmann::json fgdFiles;
        // j["MRU"] = m_lstMRUFiles;

        FILE *fp = FileSystem::OpenFileForWriting(fileName);

        if (!fp)
        {
            // TODO: throw some kind of error (presumably gracefull one)
            return;
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

void HammerGameConfiguration::Deserialize(std::string fileName)
{
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
        if (!strcasecmp(line.c_str(), "title"))
        {
            auto descr = line.substr(5);
            boost::trim(descr);
            auto title = descr.substr(1, descr.size() - 1);
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
