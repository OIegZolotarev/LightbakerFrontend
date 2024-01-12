/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#define SCENE_IO_CAPS_CAMERAS (1 << 0)

class SceneIO
{
public:
    
    virtual const char *Description();
    virtual int         Caps();

    virtual void Serialize(const char* fileName, Scene *pScene){};
    virtual void Deserialize(Scene *pNewScene){};
};
