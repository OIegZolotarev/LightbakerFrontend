/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "scene.h"
#include "gl_backend.h"
#include "goldsource_bsp_world.h"
#include "mod_mdlv10.h"
#include "mod_obj_asynch_exporter.h"
#include "mod_obj_atlas_gen.h"
#include "model_obj_world.h"
#include "properties_editor.h"
#include "r_camera_controller.h"
#include "viewports_orchestrator.h"

LevelFormat Scene::DetermineLevelFormatFromFileName(std::string levelName)
{
    auto fs  = Application::GetFileSystem();
    auto ext = fs->ExtensionFromPath(levelName);

    if (ext == ".obj")
        return LevelFormat::WavefrontOBJ;
    else if (ext == ".bsp")
        return LevelFormat::BSP;

    return LevelFormat::Unknown;
}

void Scene::LoadLevel(const char *levelName, int loadFlags)
{
    //	m_pEditHistory = new CEditHistory;

    if (levelName)
        m_LevelFormat = DetermineLevelFormatFromFileName(levelName);

    if (!(loadFlags & LRF_KEEP_ENTITIES))
        ClearEntities();

    LoadLevel(levelName);
}

const std::list<SceneEntityPtr> &Scene::GetEntities() const
{
    return m_SceneEntities;
}

Scene::Scene()
{
    m_pEditHistory = new CEditHistory;

    // TODO: review extents
    m_pBVHTree = new BVHTree(0.05, {8192, 8192, 8192});
}

Scene::~Scene()
{
    if (auto ptr = m_pGameConfiguration.lock())
    {
        ptr->UnmountGameFS();
    }

    delete m_pBVHTree;

    m_SceneEntities.clear();
    delete m_pEditHistory;
}

void Scene::DeleteEntity(SceneEntityWeakPtr l)
{
    auto ptr = l.lock();

    if (!ptr)
        return;

    auto predicate = [&](SceneEntityPtr &it) -> bool { return it == ptr; };

    auto pos = std::remove_if(m_SceneEntities.begin(), m_SceneEntities.end(), predicate);
    // m_SceneEntities.remove(*pos);
}

void Scene::DoDeleteSelection()
{
    auto &objects = ObjectPropertiesEditor::Instance()->GetBindings()->GetSelectedObjects();

    if (objects.size() == 0)
        return;

    EditTransaction *pTransaction = new EditTransaction;

    for (auto &it : objects)
    {
        auto ptr = it.lock();

        if (!ptr)
            continue;

        pTransaction->AddAction(new CDeleteLightAction(ptr));

        auto predicate = [&](SceneEntityPtr &it) -> bool {
            if (it == ptr)
            {
                m_pBVHTree->RemoveEntity(it);
            }

            return it == ptr;
        };

        m_SceneEntities.remove_if(predicate);
    }

    m_pEditHistory->PushAction(pTransaction);

    ObjectPropertiesEditor::Instance()->UnloadObjects();
    ViewportsOrchestrator::Instance()->FlagRepaintAll();
    Application::ScheduleCompilationIfNecceseary();
}

const CameraDescriptorsList &Scene::GetSceneCamerasDescriptors() const
{
    return m_lstEditorCameras;
}

void Scene::AddSceneCameraDescriptor(sceneCameraDescriptor_t &newDescritor)
{
    m_lstEditorCameras.push_back(newDescritor);
}

void Scene::DeleteSceneCameraDescriptor(size_t idx)
{
    if (idx >= m_lstEditorCameras.size())
        return;

    auto it = m_lstEditorCameras.begin();
    std::advance(it, idx);

    m_lstEditorCameras.erase(it);
}

void Scene::UpdateSceneCameraDescriptor(size_t idx, sceneCameraDescriptor_t &newDescriptor)
{
    if (idx >= m_lstEditorCameras.size())
        return;

    auto it = m_lstEditorCameras.begin();
    std::advance(it, idx);

    (*it).angles      = newDescriptor.angles;
    (*it).angles      = newDescriptor.position;
    (*it).description = newDescriptor.description;
}

sceneCameraDescriptor_t &Scene::GetSceneCameraDescriptor(size_t selected)
{
    auto it = m_lstEditorCameras.begin();
    std::advance(it, selected);

    return *it;
}

Scene *Scene::ActiveInstance()
{
    return Application::Instance()->GetMainWindow()->GetSceneRenderer()->GetScene();
}

SceneEntityPtr Scene::AddNewLight(glm::vec3 pos, LightTypes type, bool interactive)
{
    auto newLight = new Lb3kLightEntity(this);

    // newLight->pos = m_pCamera->GetOrigin() + m_pCamera->GetForwardVector() * 10.f;
    newLight->SetPosition(pos);
    newLight->SetType(type);
    newLight->intensity = 10;

    float hue = (float)rand() / 32768.f;

    glm::vec4 rgb = {1, 1, 1, 1};

    ImGui::ColorConvertHSVtoRGB(hue, 1, 1, rgb[0], rgb[1], rgb[2]);

    newLight->SetRenderColor(rgb);
    newLight->SetSerialNumber(m_ObjectsCounter.Allocate());

    newLight->SetIntensity(500);
    newLight->SetAngles(0, 0, 0);
    newLight->SetFlags(LF_EULER);

    if (type == LightTypes::Spot)
        newLight->SetCones(45, 60);
    else
        newLight->SetCones(45, 60);

    newLight->SetSize(0, 0);

    // m_SceneEntities.push_back(newLight);

    if (interactive)
        newLight->InvokeSelect();

    return AddNewSceneEntity(newLight);
}

SceneEntityPtr Scene::AddNewSceneEntity(SceneEntity *entity)
{
    auto result = SceneEntityPtr(entity);
    m_SceneEntities.push_back(result);
    OnEntityRegistered(result);
    return result;
}

std::list<SceneEntityPtr> &Scene::GetSceneObjects()
{
    return m_SceneEntities;
}

CEditHistory *Scene::GetEditHistory() const
{
    return m_pEditHistory;
}

bool Scene::IsModelLoaded()
{
    if (m_SceneEntities.size() < 1)
        return false;

    auto it = m_SceneEntities.begin();
    return it->get()->IsDataLoaded();
}

void Scene::AddEntityWithSerialNumber(SceneEntityPtr it, uint32_t sn)
{
    it->SetSerialNumber(sn);
    m_SceneEntities.push_back(it);

    OnEntityRegistered(it);
}

SceneEntityWeakPtr Scene::GetEntityBySerialNumber(size_t serialNumber)
{
    for (auto &it : m_SceneEntities)
    {
        if (it->GetSerialNumber() == serialNumber)
            return SceneEntityWeakPtr(it);
    }

    return SceneEntityWeakPtr();
}

void Scene::DeleteEntityWithSerialNumber(size_t serialNumber)
{
    auto lightPtr = GetEntityBySerialNumber(serialNumber);
    DeleteEntity(lightPtr);
}

float Scene::GetSceneScale()
{
    return m_flSceneScale;
}

void Scene::SetScale(float f)
{
    m_flSceneScale = f;
}

void Scene::RescaleLightPositions(float m_flScaleOriginal, float m_flScale)
{
    for (auto &it : m_SceneEntities)
    {
        glm::vec3 pos = it->GetPosition();

        pos /= m_flScaleOriginal;
        pos *= m_flScale;

        it->SetPosition(pos);
    }

    ObjectPropertiesEditor::Instance()->UnloadObjects();

    // Not really necessary
    // Application::ScheduleCompilationIfNecceseary();
}

SceneEntityWeakPtr Scene::GetEntityWeakRef(SceneEntity *pEntity)
{
    for (auto &ptr : m_SceneEntities)
    {
        if (ptr.get() == pEntity)
            return SceneEntityWeakPtr(ptr);
    }

    return SceneEntityWeakPtr();
}

void Scene::Reload(int loadFlags)
{
    if (loadFlags & LRF_RELOAD_LIGHTMAPS)
    {
        auto          it     = m_SceneEntities.begin();
        IWorldEntity *entity = (IWorldEntity *)(*it).get();

        entity->ReloadLightmaps();

        // 		auto obj = (ModelOBJ*)entity;
        // 		obj->ReloadTextures();
    }
}

void Scene::ClearEntities()
{
    if (m_SceneEntities.empty())
        return;

    auto begin = m_SceneEntities.begin();
    begin++;
    auto end = m_SceneEntities.end();

    m_SceneEntities.erase(begin, end);
}

void Scene::LoadLevel(const char *levelName)
{
    auto s = std::string(levelName);

    auto cfg             = GameConfigurationsManager::Instance()->FindConfigurationForLevel(s);
    m_pGameConfiguration = *cfg;

    if (auto ptr = m_pGameConfiguration.lock())
    {
        ptr->MountGameFS();
    }

    if (m_pBVHTree)
    {
        m_pBVHTree->RemoveAll();
    }

    auto format = DetermineLevelFormatFromFileName(levelName);

    std::shared_ptr<SceneEntity> pLevelEntity;

    switch (format)
    {
    case LevelFormat::Unknown:
        break;
    case LevelFormat::WavefrontOBJ:
        pLevelEntity = std::make_shared<ModelObjWorld>(levelName, this);
        break;
    case LevelFormat::BSP:
        pLevelEntity = std::make_shared<GoldSource::BSPWorld>(levelName, this);
        break;
    default:
        break;
    }

    if (!m_SceneEntities.empty())
    {
        auto it = m_SceneEntities.begin();
        *it     = pLevelEntity; // replace existing world
    }
    else
        m_SceneEntities.push_back(pLevelEntity);

    // pLevelEntity->OnAdditionToScene(this);
    // OnEntityRegistered(pLevelEntity);

    Application::GetMainWindow()->UpdateStatusbar(FL_UPDATE_ALL_STATUS_FIELDS);
}

std::string Scene::ExportForCompiling(const char *newPath, lightBakerSettings_t *lb3kOptions)
{
    // TODO: fixme
    // 	auto it = m_SceneEntities.begin();
    // 	SceneEntity* entity = (*it).get();
    // 	auto obj = (ModelOBJ*)entity;
    // 	return obj->Export(newPath, lb3kOptions);

    auto it = m_SceneEntities.begin();

    auto entity = (*it).get();

    IWorldEntity *pWorld = dynamic_cast<IWorldEntity *>(entity);
    if (NULL != pWorld)
    {
        return pWorld->ExportForCompiling(newPath, lb3kOptions);
    }
    else
        Application::EPICFAIL("Bad entity 0");

    return "";
}

void Scene::DumpLightmapMesh()
{
    // 	// TODO: fixme
    // 	auto it = m_SceneEntities.begin();
    // 	SceneEntity* entity = (*it).get();
    // 	auto obj = (ModelOBJ*)entity;
    //
    // 	GenerateAtlasTask* tsk = new GenerateAtlasTask(obj);
    // 	tsk->TransferNewUV();
    //
    // 	ModObjAsynchExporter* exporter = new ModObjAsynchExporter(obj, "lm_debug.obj", true);
    // 	exporter->ExportSynch();
    // 	delete exporter;
}

void Scene::DumpLightmapUV()
{
    // 	// TODO: fixme
    // 	auto it = m_SceneEntities.begin();
    // 	SceneEntity* entity = (*it).get();
    // 	auto obj = (ModelOBJ*)entity;
    //
    // 	GenerateAtlasTask* tsk = new GenerateAtlasTask(obj);
    // 	tsk->DumpUVImage("lm_debug%02u.tga");
    // 	delete tsk;
}

GameConfigurationWeakPtr Scene::UsedGameConfiguration()
{
    return m_pGameConfiguration;
}

uint32_t Scene::AllocSerialNumber()
{
    return m_ObjectsCounter.Allocate();
}

IWorldEntity *Scene::GetWorldEntity()
{
    assert(m_SceneEntities.size() > 0);
    auto it = m_SceneEntities.begin();

    SceneEntity *ptr = (*it).get();

    assert(instanceof <IWorldEntity>(ptr));
    return (IWorldEntity *)ptr;
}

size_t Scene::TotalEntities()
{
    return m_SceneEntities.size();
}

void Scene::DebugRenderBVH()
{
    extern int nodeToRender;
    m_pBVHTree->DebugRender(nodeToRender);
}

void Scene::DebugRenderBVHUI()
{
    m_pBVHTree->DebugRenderTreeUI();
}

BVHTree *Scene::GetBVHTree()
{
    return m_pBVHTree;
}

void Scene::OnEntityRegistered(SceneEntityPtr &it)
{
    it->FlagRegisteredInScene(true);
    auto wptr = SceneEntityWeakPtr(it);

    m_pBVHTree->InsertEntity(it);
}

void Scene::UpdateEntityBVH(const uint32_t serialNumber, const BVHBoundingBox &bboxAbsolute)
{
    m_pBVHTree->UpdateEntity(serialNumber, bboxAbsolute);
}

// void Scene::RenderEntities(RenderMode mode, SceneRenderer *sr)
// {
//     BT_PROFILE("Scene::RenderEntities()");
//
//     DebugSorting(sr);
//
//     renderStats_s *stats = GLBackend::Instance()->RenderStats();
//
//     stats->nEntitiesTotal    = m_SceneEntities.size();
//     stats->nEntitiesRendered = 0;
//
//     ModelType currentType = ModelType::Unset;
//
//     for (auto & info: m_vSortedEntities)
//     {
//         if (info.modType != currentType)
//         {
//
//         }
//
//
//         stats->nEntitiesRendered++;
//         info.pEntity->Render(mode, nullptr);
//     }
// }
