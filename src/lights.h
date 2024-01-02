/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#pragma once
#include "gl_texture.h"
#include "object_props.h"
#include "scene_entity.h"
#include "selection_3d.h"

// Light flags for LB3k app

#define LF_EULER  (1 << 0)
#define LF_DYN    (1 << 1)
#define LF_XYZ    (1 << 2)
#define LF_LINEAR (1 << 3)
#define LF_DISK   (1 << 4)
#define LF_RECT   (1 << 5)

enum class LightTypes
{
    Omni = 0,
    Spot,
    Direct
};

// TODO: нужно ли оставить привязки свойств или же сделать этот объект провайдером свойств?
class Lb3kLightEntity : public SceneEntity
{
  public:
    Lb3kLightEntity(Scene * pScene);
    Lb3kLightEntity(Lb3kLightEntity &other);

    void SetType(LightTypes type);

    LightTypes type = LightTypes::Omni;
    int flags       = 0;

    float intensity = 1;

    glm::vec3 anglesDirection;
    float cones[2];
    glm::vec2 size;

    int style = 0;

    void OnHovered() override;
    void OnMouseMove(glm::vec2 delta) override;
    void OnUnhovered() override;
    void OnSelect(ISelectableObjectWeakRef myWeakRef) override;

    void OnUnSelect() override;

    const char *Description() override;

    bool IsLightEntity() override;

    void SetIntensity(float f);
    void SetFlags(int _flags);
    void SetAngles(float pitch, float yaw, float roll);
    void SetCones(float a, float b);
    void SetSize(float sizeX, float sizeY);

    EntityClasses EntityClass() override;

    float GetIntensity();

    void Render(RenderMode mode, ShaderProgram* shader) override;
};

enum LightProperties
{
    Type = 0,
    Flags,
    Pos,
    Color,
    Intensity,
    Angles,
    ConeA,
    ConeB,
    Size,
    Style
};

typedef std::shared_ptr<Lb3kLightEntity> lightDefPtr_t;
typedef std::weak_ptr<Lb3kLightEntity> lightDefWPtr_t;

