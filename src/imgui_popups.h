/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include <vector>

enum class PopupWindows
{
	LightBaker3KConfig = 0,
	LoadfileDialog,
	SceneScaleDialog,
	ProgramOptions,
	EditGameConfiguration
};

class IImGUIPopup
{
protected:
	bool m_bVisible;
private:
	PopupWindows m_Id;
public:
	IImGUIPopup(PopupWindows id);
	virtual void Render() = 0;

	void SetVisible(bool bFlag);

	bool IsVisible()
	{
		return m_bVisible;
	}

	PopupWindows Id()
	{
		return m_Id;
	}

	virtual void OnOpen() {};
};

class PopupsManager
{
	std::vector<IImGUIPopup*> m_vPopups;
	PopupsManager();
public:
	static PopupsManager* Instance();
	~PopupsManager();
	
	void ApplyFileDialogSkins();
	
	void ShowPopup(PopupWindows id);
	void RenderPopups();

	IImGUIPopup *FindPopupByID(PopupWindows id);
};

