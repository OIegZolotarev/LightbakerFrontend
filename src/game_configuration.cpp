/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "game_configuration.h"
#include "common.h"
#include "goldsource_bsp_entity.h"
#include "goldsource_game_configuration.h"
#include <regex>

GameConfiguration::GameConfiguration(std::string descr, std::string gameDirectory)
    : m_Description(descr)
{
    SetGameDirectory(gameDirectory);
}


GameConfiguration::~GameConfiguration()
{
    
}

const char *GameConfiguration::Name() const
{
    return m_GameDirectory.c_str();
}

const char *GameConfiguration::Description() const
{
    return m_Description.c_str();
}

bool GameConfiguration::MatchesGameDirectoryMask(std::string &levelFilePath) const
{
    auto levelPath          = std::filesystem::path(levelFilePath);
    auto levelPathCanonical = std::filesystem::canonical(levelPath).string();

    std::regex pathRegex(m_GameDirectory + "/*");
    
    auto words_begin = std::sregex_iterator(levelPathCanonical.begin(), levelPathCanonical.end(), pathRegex);
    auto words_end   = std::sregex_iterator();

    auto dist = std::distance(words_begin, words_end);

    if (dist != 1)
        return false;

    return true;
}

void GameConfiguration::SetDescription(std::string &descr)
{
    m_Description = descr;
}

void GameConfiguration::SetGameDirectory(std::string &gameDir)
{
    auto gdPath          = std::filesystem::path(gameDir);
    auto gdPathCanonical = std::filesystem::canonical(gdPath);

    m_GameDirectory = gdPathCanonical.string();
}

GameConfigurationsManager *GameConfigurationsManager::Instance()
{
    static GameConfigurationsManager *sInstance = new GameConfigurationsManager;
    return sInstance;
}

GameConfigurationsManager::~GameConfigurationsManager()
{
    for (auto &it : m_Configurations)
    {
        it->Serialize("");
        //delete it;
    }

    m_Configurations.clear();
}

GameConfigurationWeakPtrOpt GameConfigurationsManager::FindGameByName(const char *gameName) const
{
    for (auto &game : m_Configurations)
    {
        if (!strcasecmp(gameName, game->Name()))
            return game;
    }

    return std::nullopt;
}

GameConfigurationWeakPtrOpt GameConfigurationsManager::FindConfigurationForLevel(std::string &levelFilePath)
{
    for (auto &it : m_Configurations)
    {
        if (it->MatchesGameDirectoryMask(levelFilePath))
            return it;
    }

    // If none of load configuration matches - attempt to parse new one on the fly

    auto lookupResult = ScanForGameDefinitionFile(levelFilePath, 3);

    // If none configuration data found - fail
    if (!lookupResult)
        return GameConfigurationWeakPtr();

    GameConfigurationPtr pNewConfiguration = nullptr;

    std::string gameDir = std::get<0>(*lookupResult);
    GameEngines engine  = std::get<1>(*lookupResult);
    switch (engine)
    {
    case GameEngines::GoldSource:
        pNewConfiguration = std::make_shared<GoldSource::HammerGameConfiguration>(gameDir, engine);
        break;
    case GameEngines::Xash3d:        
        pNewConfiguration = std::make_shared<GoldSource::HammerGameConfiguration>(gameDir, engine);
        break;
    default:
        break;
    }

    if (pNewConfiguration)
        m_Configurations.push_back(pNewConfiguration);

    return pNewConfiguration;
}

const std::list<GameConfigurationPtr> & GameConfigurationsManager::AllConfigurations() const
{
    return m_Configurations;
}

const std::list<GameConfigurationWeakPtr> GameConfigurationsManager::AllConfigurationsWeakPtr() const
{
    std::list<GameConfigurationWeakPtr> result;

    for (auto & it : m_Configurations)
        result.push_back(std::weak_ptr(it));

    return result;
}

GameConfigurationsManager::GameConfigurationsManager()
{
    (void)0;
}

std::optional<gamelookupresult_t> GameConfigurationsManager::ScanForGameDefinitionFile(std::string &levelFileName,
                                                                                       size_t maxRecursion)
{
    auto directory = FileSystem::Instance()->BaseDirectoryFromPath(levelFileName);

    for (size_t i = 0; i < maxRecursion; i++)
    {
        // Step-up to parent directory;
        directory += "../";

        // Check for gameinfo.txt - to detect migrated from GoldSource to xash mods
        if (FileSystem::Instance()->FileExists(directory + "gameinfo.txt"))
            return gamelookupresult_t(directory, GameEngines::Xash3d);
        else if (FileSystem::Instance()->FileExists(directory + "liblist.gam"))
            return gamelookupresult_t(directory, GameEngines::GoldSource);
    }

    return std::nullopt;
}
