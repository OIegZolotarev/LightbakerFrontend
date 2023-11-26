/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#pragma once
#include <optional>
#include "persistent.h"


enum class GameEngines
{
    GoldSource = 0,
    Xash3d
};

class GameConfiguration
{
    friend class GameConfigurationsManager;
    bool m_bDefault = false;

  protected:
         

    std::string m_GameDirectory;
    std::string m_Description;

    // File name from which one this config was loaded
    std::string m_SavedFileName = "";

    GameEngines m_Engine;

  public:
    GameConfiguration() = default;
    GameConfiguration(std::string description, std::string gameDirectory);
    ~GameConfiguration();

    const char *Name() const;

    const char *Description() const;

    bool MatchesGameDirectoryMask(std::string &levelFilePath) const;

    void SetDescription(std::string &descr);
    void SetGameDirectory(std::string &gameDir);

    virtual void Serialize(std::string fileName) const {};

    virtual void EditDialog()
    {
    }

    virtual GameConfiguration *Clone() = 0;

    void SetDefault(bool bFlag)
    {
        m_bDefault = bFlag;
    }

    bool IsDefault()
    {
        return m_bDefault;
    }
};

typedef std::tuple<std::string, GameEngines> gamelookupresult_t;

typedef std::shared_ptr<GameConfiguration> GameConfigurationPtr;
typedef std::weak_ptr<GameConfiguration> GameConfigurationWeakPtr;

typedef std::optional<GameConfigurationWeakPtr> GameConfigurationWeakPtrOpt;

class GameConfigurationsManager : public Singleton<GameConfigurationsManager>
{
    std::list<GameConfigurationPtr> m_Configurations;

    // For GoldSource/Xash3d games returns game directory + detected engine type (by detecting liblist.gam/gameinfo.txt)
    std::optional<gamelookupresult_t> ScanForGameDefinitionFile(std::string &levelFileName, size_t maxRecursion = 3);

    PersistentStorage *m_pPersistentStorage;

  public:
    ~GameConfigurationsManager();
    
    // Initialization
    void Init(PersistentStorage * storage);
    void LoadGameConfigsFromDisk();
       
    // Searching
    GameConfigurationWeakPtrOpt FindGameByName(const char *gameName) const;
    GameConfigurationWeakPtrOpt FindConfigurationForLevel(std::string &levelFilePath);
    
    // Accessing
    std::list<GameConfigurationPtr> &AllConfigurations();
    const std::list<GameConfigurationWeakPtr> AllConfigurationsWeakPtr() const;
    
    // Saving and updating
    static std::string SuggestSaveFileName(GameEngines engine, const std::string &m_Description);    
    void UpdateGameConfiguration(GameConfigurationWeakPtr ptr, GameConfiguration *edited);

    // Default configurations
    void SetDefaultGameConfiguration(GameConfigurationWeakPtr ptr);
    GameConfigurationWeakPtr GetDefaultGameConfiguration();
};