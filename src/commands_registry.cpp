#include "common.h"
#include "commands_registry.h"

CCommandsRegistry::CCommandsRegistry()
{
	m_vecCommandDescriptor.clear();
}

CCommandsRegistry::~CCommandsRegistry()
{
	m_vecCommandDescriptor.clear();
	m_vecCommandDescriptor.shrink_to_fit();
}
