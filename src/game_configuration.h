/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include <optional>

class GameConfiguration
{
protected:
	std::string m_GameDirectory;
    std::string m_Description;
public:

	GameConfiguration(std::string description,std::string gameDirectory);
	~GameConfiguration();

	const char *Name() const;

    const char *Description() const;

    bool MatchesGameDirectoryMask(std::string & levelFilePath) const;
  
    void SetDescription(std::string & descr);
    void SetGameDirectory(std::string & gameDir);

    
};

enum class GameEngines
{
    GoldSource = 0,
    Xash3d
};

typedef std::tuple<std::string, GameEngines> gamelookupresult_t;

class GameConfigurationsManager
{
    GameConfigurationsManager();
    std::list<GameConfiguration *> m_Configurations;

    // For GoldSource/Xash3d games returns game directory + detected engine type (by detecting liblist.gam/gameinfo.txt)
    std::optional<gamelookupresult_t> ScanForGameDefinitionFile(std::string &levelFileName, size_t maxRecursion = 3);

 public:
  
	static GameConfigurationsManager *Instance();
    ~GameConfigurationsManager();

	GameConfiguration *FindGameByName(const char *gameName) const;
    GameConfiguration *FindConfigurationForLevel(std::string & levelFilePath);
};