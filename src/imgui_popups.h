/*	
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include <vector>
#include <string>
#include <stack>
#include <list>

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
	std::string m_Key;
private:
    bool m_bSchedulePopupOpen = false;
	PopupWindows m_Id;

	void SchedulePopupOpen();
    void OpenPopup();

	friend class PopupsManager;

public:
	IImGUIPopup(PopupWindows id);

	bool BeginRendering();
    void EndRendering();

	virtual int RenderingFlags();
	virtual void Render() = 0;

	void SetVisible(bool bFlag);
	bool IsVisible();

	PopupWindows Id();

	virtual void OnOpen();
    const char *GetKey();
    
	
};

class PopupsManager
{
	std::vector<IImGUIPopup*> m_vPopups;
    std::list<IImGUIPopup *> m_lstOpenedPopups;

	PopupsManager();
public:
	static PopupsManager* Instance();
	~PopupsManager();
	
	void ApplyFileDialogSkins();
	
	void ShowPopup(PopupWindows id);
	void RenderPopups();

	IImGUIPopup *FindPopupByID(PopupWindows id);
};

