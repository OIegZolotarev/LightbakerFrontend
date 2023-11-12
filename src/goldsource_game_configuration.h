/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once
#include "game_configuration.h"


namespace GoldSource
{
class HammerGameConfiguration: public GameConfiguration
{

	std::string m_GameName = "Half-life";
	std::string m_ModDirectory = "";	

	std::list<std::string> m_WadFiles;
	
	struct  
	{
		std::string csg = "";
		std::string bsp = "";
		std::string vis = "";
		std::string rad = ""; // Не особо то и нужно :)
	}m_CompilationPrograms;

	std::list<std::string> m_FGDFiles;


	GameEngines m_EngineHint = GameEngines::GoldSource;

  public:
    HammerGameConfiguration(std::string gameRootDir, GameEngines engineHint);
	~HammerGameConfiguration();

  private:
    void ParseLiblistGam();
    void ParseGameInfo();
};

}