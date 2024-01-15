/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "ui_common.h"

#define EVENT_HANDLED (1<<0) // Event was handled but should continue propagation
#define EVENT_CONSUMED (1<<1) // Event was handled and propagation must be stopped

#define EVENT_FINISHED (EVENT_CONSUMED | EVENT_HANDLED)

class IEventHandler
{
public:
	virtual int HandleEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta) = 0;
};