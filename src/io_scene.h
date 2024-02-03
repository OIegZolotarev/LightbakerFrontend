/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

enum class SceneIOClasses
{
    LM2
};

#define SCENE_IO_CAPS_CAMERAS (1 << 0)

class SceneIO: public IUserPresentableObject
{
protected:
    
    friend class SceneIOManager;


    virtual bool           IsFileFormatSupported(const char *fileName) = 0;
    virtual SceneIOClasses FormatID() = 0;
public:
    
    virtual const char *Description() const;
    virtual int         Caps();

    virtual void Serialize(const char* fileName, Scene *pScene){};
    virtual void Deserialize(Scene *pNewScene){};
};


class SceneIOManager: public Singleton<SceneIOManager>
{
    friend class Singleton;

    SceneIOManager();

private:
    std::list<SceneIO *> m_lstSerializers;
public:
    void SerializeScene(const char *fileName, Scene * pScene, SceneIOClasses format);
    void PerformSavingDialog();
};