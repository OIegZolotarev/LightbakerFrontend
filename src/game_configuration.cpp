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

void GameConfigurationsManager::Init()
{
    LoadGameConfigsFromDisk();
    return;
}

void GameConfigurationsManager::LoadGameConfigsFromDisk()
{
    char *p         = SDL_GetPrefPath(SDL_ORGANIZATION, SDL_APP_NAME);
    std::string dir = std::string(p) + "game_configs/gs_and_xash/";

    if (!FileSystem::FileExists(std::string(p) + "game_configs/gs_and_xash/"))
    {
        return;
    }

    for (auto const &dir_entry : std::filesystem::directory_iterator{dir})
    {
        auto path = std::filesystem::canonical(dir_entry.path()).string();

        auto pNewConfiguration = std::make_shared<GoldSource::HammerGameConfiguration>(path);       
        m_Configurations.push_back(pNewConfiguration);

    }
}

GameConfigurationsManager::~GameConfigurationsManager()
{
    for (auto &it : m_Configurations)
    {
        it->Serialize(it->m_SavedFileName);
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

std::string GameConfigurationsManager::SuggestSaveFileName(GameEngines engine, const std::string &m_Description)
{
    std::string engineDir = "others";
    char *p  = SDL_GetPrefPath(SDL_ORGANIZATION, SDL_APP_NAME);

    switch (engine)
    {
    case GameEngines::GoldSource:                
    case GameEngines::Xash3d:
        engineDir = "gs_and_xash";
        break;
    default:
        break;        
    }

    std::string prefPath = p;
    std::string engineConfigDir = prefPath + "/game_configs/" + engineDir;

    std::string result = std::format("{0}/{1}", engineConfigDir, FileSystem::SanitizeFileName(m_Description));
        
    if (!FileSystem::FileExists(engineConfigDir))
    {
        FileSystem::MakeDir(engineConfigDir);
    }

    return result;
}

void GameConfigurationsManager::UpdateGameConfiguration(GameConfigurationWeakPtr ptr, GameConfiguration *edited)
{
    auto lockedPtr = ptr.lock();

    // shared_ptr comparsion compares pointers, not managed object so this shoud be fine
    auto it = std::find(m_Configurations.begin(), m_Configurations.end(), lockedPtr);

    if (it == m_Configurations.end())
        return;

    *it = std::shared_ptr<GameConfiguration>(edited);
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
