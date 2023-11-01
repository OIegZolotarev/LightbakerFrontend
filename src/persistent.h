/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include <nlohmann/json.hpp>
#include <stack>
#include <list>
#include "object_props.h"
#include "igui_panel.h"

enum class ApplicationSettings
{
	ShowGround = 0,
	BackgroundColor1,
	BackgroundColor2,
	UseGradientBackground,

	RebakeSceneAfterChanges,	
	CameraControlScheme,
	CameraMovementSpeed,
	CameraZNear,
	CameraZFar,
};


class PersistentStorage
{
	nlohmann::json SerializeApplicationProperty(nlohmann::json& j, VariantValue* it);
	void ParseApplicationSettings(nlohmann::json j);

	std::list<std::string>	m_lstMRUFiles;
	void						SaveToFile();
	void						LoadFromFile(class Application * appInstance);
	
	std::vector<VariantValue*> m_ApplicationProps;

	bool					m_bPanelsValid[PanelsId::MaxPanels] = {false};

	bool m_bFreshFile;
public:
	
	PersistentStorage(class Application * appInstance);
	~PersistentStorage();

	void						PushMRUFile(const char* fileName);
	std::list<std::string>&		GetMRUFiles();
		
	VariantValue * GetSetting(ApplicationSettings id);

	bool	GetSettingBool(ApplicationSettings id);
	
	bool IsPanelAtValidPosition(PanelsId id);
	void FlagPanelIsAtValidPosition(PanelsId id);
	bool IsFreshFile();

};