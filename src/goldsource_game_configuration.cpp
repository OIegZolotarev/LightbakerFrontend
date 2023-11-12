/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "goldsource_game_configuration.h"
#include "text_utils.h"

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


void HammerGameConfiguration::ParseGameInfo()
{
    auto filePath = m_GameDirectory + "/gameinfo.gam";
    auto fd       = FileSystem::Instance()->LoadFile(filePath);

    auto lines = TextUtils::SplitTextSimple((char *)fd->Data(), fd->Length(), '\n');

    for (auto &line : lines)
    {
        auto items = TextUtils::SplitTextWhitespaces(line.c_str(), line.length());

        if (items.size() != 2)
            continue;

        auto it = items.begin();

        if (!strcasecmp((*it).c_str(), "title"))
        {
            it++;
            SetDescription(*it);
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
        auto items = TextUtils::SplitTextWhitespaces(line.c_str(), line.length());

        if (items.size() != 2)
            continue;

        auto it = items.begin();

        if (!strcasecmp((*it).c_str(), "game"))
        {
            it++;
            SetDescription(*it);
        }

    }

    fd->UnRef();
}

HammerGameConfiguration::~HammerGameConfiguration()
{

}
