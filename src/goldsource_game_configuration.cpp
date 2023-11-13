/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "goldsource_game_configuration.h"
#include "text_utils.h"
#include <boost/algorithm/string.hpp>

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
}


FGDEntityClass *HammerGameConfiguration::LookupFGDClass(std::string &classname)
{
    for (auto & it: m_lstFGDData)
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
    auto fd = FileSystem::Instance()->LoadFile(filePath);

    auto lines = TextUtils::SplitTextSimple((char *)fd->Data(), fd->Length(), '\n');

    for (auto & line: lines)
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
