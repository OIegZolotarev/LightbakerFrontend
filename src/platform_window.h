/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/

#pragma once

class IPlatformWindow
{
private:
    std::list<IEventHandler *> m_vEventHandlers;
public:
    IPlatformWindow() = default;
    virtual ~IPlatformWindow();

    size_t GetId();

    void AddEventHandler(IEventHandler *pHandler);
    const std::list<IEventHandler *> &EventHandlers();
    virtual void                      InitImGUISDL2Platform(){};
};