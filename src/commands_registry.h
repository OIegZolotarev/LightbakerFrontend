/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "gl_texture.h"

enum class GlobalCommands
{
	AddOmniLight = 0,
	AddSpotLight,
	AddDirectLight,
	LoadFile,
	Bake,
	ToggleGround,
	DeleteSelection,
	ResetLayout,
	Undo,
	Redo,
	SceneScale
};

typedef std::function<void()> pfnCommandCallback;

#define CMD_ON_MAINTOOLBAR (1<<0)
#define CMD_ONLY_ICON (1<<1)
#define CMD_ONLY_TEXT (1<<2)

class CCommand
{
	char				m_szDescription[32];
	char				m_szKeyStroke[32];

	int					m_rKeys[6 + 3] = { -1 };


	int					m_iFlags;
	gltexture_t*		m_pIcon;
	GlobalCommands		m_eCommandId;
	pfnCommandCallback	m_pfnCallback;

public:
	CCommand(GlobalCommands id, const char* description,const char* keyStroke, gltexture_t* icon, int flags, pfnCommandCallback callback);
	
	void				Execute();
	int					Flags() { return m_iFlags; }	
	void				RenderImGUI(int size = 32);
	GlobalCommands		GetId();
	const char*			GetDescription();

	bool IsKeystrokeActive(const uint8_t* kbState);
	const char* GetShortcutDescription();
};

class CCommandsRegistry
{
	std::vector<int>				 m_vecMainToolbarCommands;
	std::vector<CCommand*>			 m_vecCommandDescriptor;
public:
	CCommandsRegistry();
	~CCommandsRegistry();

	void				RegisterCommand(CCommand * pCommand);

	std::vector<int>&	GetMainToolbarCommands();
	CCommand*			GetCommandByInternalIndex(int index);
	CCommand*			FindCommandByGlobalId(GlobalCommands id);

	void				RenderCommandAsMenuItem(GlobalCommands cmdId);


	bool				OnKeyDown();
};

