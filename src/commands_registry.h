#pragma once

enum GlobalCommands
{
	AddOmniLight = 0,
	AddSpotLight,
	AddDirectLight
};

typedef std::function<void()> pfnCommandCallback;

typedef struct commandDescriptor_s
{
	size_t commandId;
	pfnCommandCallback callback;
}commandDescriptor_t;

class CCommandsRegistry
{
	std::vector<commandDescriptor_t> m_vecCommandDescriptor;
public:
	CCommandsRegistry();
	~CCommandsRegistry();
};

