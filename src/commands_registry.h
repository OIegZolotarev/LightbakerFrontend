#pragma once
#include "gl_texture.h"

enum class GlobalCommands
{
	AddOmniLight = 0,
	AddSpotLight,
	AddDirectLight,
	LoadFile,
	Bake
};

typedef std::function<void()> pfnCommandCallback;

#define CMD_ON_MAINTOOLBAR (1<<0)
#define CMD_ONLY_ICON (1<<1)
#define CMD_ONLY_TEXT (1<<2)

class CCommand
{
	char				m_szDescription[32];
	int					m_iFlags;
	gltexture_t*		m_pIcon;
	GlobalCommands		m_eCommandId;
	pfnCommandCallback	m_pfnCallback;

public:
	CCommand(GlobalCommands id, const char* description, gltexture_t* icon, int flags, pfnCommandCallback callback);
	void Execute();

	int Flags() { return m_iFlags; }
	
	void RenderImGUI(int size = 32);
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
};

