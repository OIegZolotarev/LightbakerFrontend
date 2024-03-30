/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "brush_object.h"
#include "gl_draw_mesh.h"
#include "hammer_map.h"
#include "winding.h"

BrushObject::BrushObject(Scene *pScene) : SceneEntity(pScene)
{
}

BrushObject::BrushObject(BrushObject *pOther) : SceneEntity(pOther->m_pScene)
{
    assert(false && "Implement me!");
}

BrushObject::~BrushObject()
{
    m_pModel.reset();
}

void BrushModel::RemoveInvalidFaces()
{
    for (int32_t i = (int32_t)m_lstFaces.size() - 1; i >= 0; i--)
    {
        BrushFace *    pFace = GetFace(i);
        const plane_t *f     = pFace->GetPlane();

        if (f->normal == glm::vec3(0, 0, 0))
        {
            RemoveFace(i);
            continue;
        }

        for (int32_t j = 0; j < i; j++)
        {
            BrushFace *    pFaceCheck = GetFace(j);
            const plane_t *checkPlane = pFaceCheck->GetPlane();

            const glm::vec3 &f1 = f->normal;
            const glm::vec3 &f2 = checkPlane->normal;

            if ((glm::dot(f1, f2) > 0.999) && (fabs(f->dist - checkPlane->dist) < 0.01))
            {
                RemoveFace(j);
                break;
            }
        }
    }
}

size_t BrushModel::UniqueMaterialsCount()
{
    std::set<MaterialAsset *> m_uniqueMaterials;

    for (auto &it : m_lstFaces)
    {
        m_uniqueMaterials.insert(it->GetMaterialAsset().get());
    }

    return m_uniqueMaterials.size();
}

bool BrushModel::CreateFacesFromPlanes(int flags)
{
    int i, j, k;
    m_bValid      = true;
    size_t nFaces = m_lstFaces.size();

    m_lstFaces.sort([](const BrushFace * a, const BrushFace * b) -> bool {
        
            return strcmp(a->GetTextureName(), b->GetTextureName());
        });

    assert(nFaces < MAPSOLID_MAX_FACES);
    RemoveInvalidFaces();

    bool bGotFaces = false;

    if (m_pDrawBuffer)
        delete m_pDrawBuffer;

    m_Bounds = BoundingBox(1);

    m_pDrawBuffer = new DrawMesh();
    m_pDrawBuffer->Begin(GL_TRIANGLES);

    // 

    m_nMeshes = UniqueMaterialsCount();
    m_pMeshes = new brushModelMesh_t[m_nMeshes];

    size_t first_mesh_element = 0;
    size_t num_mesh_elements  = 0;
    size_t mesh_index         = 0;

    MaterialAssetPtr currentMaterial = MaterialAssetPtr();

    for (i = 0; i < nFaces; i++)
    {
        BrushFace *    pFace = GetFace(i);
        const plane_t *f     = pFace->GetPlane();

        if (pFace->GetMaterialAsset() != currentMaterial)
        {

            if (num_mesh_elements > 0)
            {
                m_pMeshes[mesh_index].first_element = first_mesh_element;
                m_pMeshes[mesh_index].num_elements = num_mesh_elements;
                m_pMeshes[mesh_index].asset = currentMaterial;
                mesh_index++;
            }


            first_mesh_element = m_pDrawBuffer->CurrentElement();
            num_mesh_elements  = 0;
            currentMaterial    = pFace->GetMaterialAsset();
        }

        Winding *w = new Winding(f);

        for (j = 0; j < nFaces && w; j++)
        {
            BrushFace *pFaceClip = GetFace(j);

            if (j != i)
            {
                w->Clip(pFaceClip);
                ValidateWinding(w);
            }
        }

        if (w != nullptr)
        {
            std::vector<glm::vec3> &points  = w->GetPoints();
            const size_t            nPoints = points.size();

            for (j = 0; j < nPoints; j++)
            {
                for (k = 0; k < 3; k++)
                {
                    float v  = points[j][k];
                    float v1 = int(v);
                    if ((v != v1) && (fabs(v - v1) < ROUND_VERTEX_EPSILON))
                    {
                        points[j][k] = v1;
                    }
                }
            }

            w->RemoveDuplicatePoints(ON_EPSILON);

            bGotFaces = true;

            if (flags & CREATE_FROM_PLANES_CLIPPING)
            {
                pFace->CreateFaceFromWinding(w, CREATE_FACE_PRESERVE_PLANE | CREATE_FACE_CLIPPING);
            }
            else
            {
                pFace->CreateFaceFromWinding(w, CREATE_FACE_PRESERVE_PLANE);
            }

            delete w;
        }
    }

    if (num_mesh_elements > 0)
    {
        m_pMeshes[mesh_index].first_element = first_mesh_element;
        m_pMeshes[mesh_index].num_elements  = num_mesh_elements;
        m_pMeshes[mesh_index].asset         = currentMaterial;
        mesh_index++;
    }

    m_pDrawBuffer->End();

    m_BoundsAbsolute = m_Bounds;
    m_Bounds         = m_Bounds.ConvertToRelative();

    return m_bValid;
}

void BrushModel::AddPointsToBounds(const glm::vec3 &pt)
{
    m_Bounds.AddPoint(pt);
}

SceneEntity *BrushObject::Clone()
{
    return new BrushObject(this);
}

void BrushObject::GenerateModel(const HammerMap *pMap, const size_t firstFace, const size_t lastFace, size_t index)
{
    char tmp[32];
    sprintf_s(tmp, 32, "%d", (int)index);

    m_pModel = std::make_shared<BrushModel>(tmp);

    for (size_t f = firstFace; f < lastFace; f++)
    {
        const map220face_t *pFace = pMap->GetFace(f);
        
        auto pBrushFace = m_pModel->AddFace(pFace->points);

        GLTexture *pTexture = TextureManager::LoadWADTextureAsynch(pFace->textureName.c_str());
        pBrushFace->SetTexture(pTexture);
        pBrushFace->SetUAxis(pFace->uAxis);
        pBrushFace->SetVAxis(pFace->vAxis);

    }

    m_pModel->CreateFacesFromPlanes(CREATE_FROM_PLANES_CLIPPING);

    auto m = IModelWeakPtr(m_pModel);
    SetModel(m);

    auto bbox = m_pModel->GetBoundingBox();
    SetBoundingBox(*bbox);
    SetPosition(glm::vec3{0, 0, 0});
}

void BrushObject::Render(RenderMode mode, const SceneRenderer *sr, ShaderProgram *shader)
{
    

    m_pModel->Render(this, sr, mode, shader);
}

DrawMesh *BrushModel::GetDrawMesh()
{
    return m_pDrawBuffer;
}

BrushFace *BrushModel::AddFace(const glm::vec3 pts[3])
{
    BrushFace *pNewFace = new BrushFace(this, pts);

    pNewFace->m_FaceId = m_lstFaces.size() + 1;

    m_lstFaces.push_back(pNewFace);

    return pNewFace;
}

void BrushModel::RemoveFace(size_t idx)
{
    auto it = m_lstFaces.begin();
    std::advance(it, idx);

    m_lstFaces.erase(it);
}

BrushFace *BrushModel::GetFace(size_t idx)
{
    auto it = m_lstFaces.begin();
    std::advance(it, idx);
    return *it;
}

BrushModel::BrushModel(const char *modelName) : IModel(modelName)
{
    SetType(ModelType::Brush);
}

 BrushModel::~BrushModel()
{
     for (auto &it : m_lstFaces)
         delete it;

     m_lstFaces.clear();

     ReleaseMeshes();

 }

void BrushModel::ReleaseMeshes()
 {
     if (m_pMeshes)
     {
         delete[] m_pMeshes;
         m_nMeshes = 0;
     }
 }

 const std::optional<BoundingBox> BrushModel::GetBoundingBox() const
{
    return m_Bounds;
}

void BrushModel::Render(SceneEntity *pEntity, const SceneRenderer *sr, RenderMode mode, ShaderProgram *currentShader)
{
    ColorRGBA color = PointerHash(pEntity);

    for (auto &it : currentShader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Color:
            it->SetFloat4(color);
            break;
        case UniformKind::TransformMatrix:
            it->SetMat4(glm::mat4x4(1.f));
            break;
        case UniformKind::Scale:
            it->SetFloat3({1.f, 1.f, 1.f});
            break;
        case UniformKind::ObjectSerialNumber:
            it->SetInt(pEntity->GetSerialNumber() + 1);
            break;
        case UniformKind::Color2:
            sr->ApplySelectedObjectColor(pEntity, it);
            break;
        }
    }

    for (size_t i = 0; i < m_nMeshes; i++)
    {
        auto mesh = &m_pMeshes[i];
        GLBackend::BindTexture(0, mesh->asset->DiffuseImage());

        m_pDrawBuffer->Draw(mesh->first_element, mesh->num_elements);
    }
}