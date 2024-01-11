/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#include "application.h"
#include "properties_editor.h"

#include "imgui_helpers.h"
#include "lights.h"
#include "object_props.h"
#include "r_camera.h"
#include "ui_common.h"
#include "viewports_orchestrator.h"

ObjectPropertiesEditor::ObjectPropertiesEditor()
    : ToolUIPanel(ToolUIPanelID::ObjectProperties, (char *)"Object properties")
{
    // LightPropertiesBinding b;
    // b.FillProperties(m_vPropsData);

    m_pPropertiesBinding = new GoldSource::BSPEntitiesPropertiesBinder();
}

ObjectPropertiesEditor *ObjectPropertiesEditor::Instance()
{
    static ObjectPropertiesEditor *sInstance = new ObjectPropertiesEditor;
    return sInstance;
}

ObjectPropertiesEditor::~ObjectPropertiesEditor()
{
    if (m_pPropertiesBinding)
    {
        delete m_pPropertiesBinding;
        m_pPropertiesBinding = nullptr;
    }
}

GoldSource::BSPEntitiesPropertiesBinder *ObjectPropertiesEditor::GetBindings()
{
    return m_pPropertiesBinding;
}

void ObjectPropertiesEditor::RenderEditor()
{
    if (!m_FlagsEditorID)
        m_FlagsEditorID = ImGui::GetID("Flags");

    RenderPropetiesPane();
    RenderFlagsEditor();
}

void ObjectPropertiesEditor::LoadObject(SceneEntityWeakPtr &pObject, bool addToSelection)
{
    auto ref = pObject.lock();

    if (!ref)
        return;

    if (!addToSelection)
        m_pPropertiesBinding->ClearObjects();

    m_pPropertiesBinding->AddObject(pObject);

    auto &selected_objects = m_pPropertiesBinding->GetSelectedObjects();

    

    if (selected_objects.size() > 1)
        m_AllObjectsBounds.AddBoundingBox(ref->GetAbsoulteBoundingBox());
    else
        m_AllObjectsBounds = ref->GetAbsoulteBoundingBox();

    UpdateRelativePositions();

    SetupGuizmo();
}

void ObjectPropertiesEditor::AddObject(SceneEntityWeakPtr &pObject)
{
    auto ref = pObject.lock();

    if (!ref)
        return;

    m_pPropertiesBinding->AddObject(pObject);

    auto &selected_objects = m_pPropertiesBinding->GetSelectedObjects();

    if (selected_objects.size() > 1)
        m_AllObjectsBounds.AddBoundingBox(ref->GetAbsoulteBoundingBox());
    else
        m_AllObjectsBounds = ref->GetAbsoulteBoundingBox();

    UpdateRelativePositions();
    SetupGuizmo();
}

void ObjectPropertiesEditor::ResetGuizmoScaling()
{
    for (int i = 0; i < 4; i++)
        m_matGuizmo[i][i] = 1;
}

void ObjectPropertiesEditor::UpdateAllObjectsBoundingBox()
{
    auto &selected_objects2 = m_pPropertiesBinding->GetSelectedObjects();

    if (selected_objects2.size() == 0)
        return;

    bool bInitialized = false;

    for (auto it : selected_objects2)
    {
        auto ptr = it.lock();
        if (!ptr)
            continue;

        if (!bInitialized)
        {
            m_AllObjectsBounds = ptr->GetAbsoulteBoundingBox();
            bInitialized       = true;
        }
        else
            m_AllObjectsBounds.AddBoundingBox(ptr->GetAbsoulteBoundingBox());
    }
}

void ObjectPropertiesEditor::UpdateRelativePositions()
{
    auto &selected_objects2 = m_pPropertiesBinding->GetSelectedObjects();

    glm::vec3 centerPos  = m_AllObjectsBounds.Center();
    glm::vec3 boundsSize = m_AllObjectsBounds.Size();

    m_mapRelativeObjectsPos.clear();

    for (auto &it : selected_objects2)
    {
        auto ptr = it.lock();

        if (!ptr)
            continue;

        uint32_t  serial       = ptr->GetSerialNumber();
        glm::vec3 relative_pos = ptr->GetPosition() - centerPos;

        m_mapRelativeObjectsPos.insert(std::pair<uint32_t, glm::vec3>(serial, relative_pos));
    }
}

void ObjectPropertiesEditor::UpdateProperty(VariantValue *it)
{
    // m_pPropertiesBinding->UpdateObjectProperties(it, 1);
    m_pPropertiesBinding->UpdateProperty(it);
    ViewportsOrchestrator::Instance()->FlagRepaintAll();
}

void ObjectPropertiesEditor::RenderGuizmo(Viewport *pViewport)
{
    if (!CheckObjectValidity())
        return;

    if (!m_pGuizmoPropertyPosition)
        return;

    ImGuizmo::BeginFrame();

    EditTransform(pViewport, true);
}

void ObjectPropertiesEditor::UnloadObjects()
{
    if (m_pPropertiesBinding)
    {
        m_lstProperties.clear();
        m_pPropertiesBinding->ClearObjects();
    }

    SetupGuizmo();

    ViewportsOrchestrator::Instance()->FlagRepaintAll();
}

DockPanels ObjectPropertiesEditor::GetDockSide()
{
    return DockPanels::RightTop;
}

const BoundingBox ObjectPropertiesEditor::GetAllObjectsBoundingBox() const
{
    return m_AllObjectsBounds;
}

void ObjectPropertiesEditor::Render()
{
    RenderEditor();
}

void ObjectPropertiesEditor::ReloadPropertiesFromBinder()
{
    if (m_pPropertiesBinding)
    {
        m_lstProperties.clear();
        m_pPropertiesBinding->FillProperties(m_lstProperties);
    }

    m_pGuizmoPropertyPosition = FindFirstPropertyByType(PropertiesTypes::Position);
    m_pGuizmoPropertyRotation = FindFirstPropertyByType(PropertiesTypes::Angles);
}

void ObjectPropertiesEditor::UpdatePositionDelta(VariantValue *propertyPosition, glm::vec3 delta)
{
    m_pPropertiesBinding->UpdatePropertyPositionDelta(propertyPosition, delta);
}

void ObjectPropertiesEditor::RenderDebugOctree()
{
    auto entity = m_pPropertiesBinding->GetEntity(0);

    if (entity)
    {
        // entity->GetOctreeNode()->DrawDebug(2);
    }
}

void ObjectPropertiesEditor::ReloadPropertyValue(int id)
{
    m_lstProperties.clear();
    m_pPropertiesBinding->FillProperties(m_lstProperties);

    SetupGuizmo();
}

bool ObjectPropertiesEditor::CheckObjectValidity()
{
    // assert(false && "remove me");
    // Remove bad objects and return false if empty?
    return true;
}

void ObjectPropertiesEditor::SetupGuizmo()
{
    m_pGuizmoPropertyPosition = FindFirstPropertyByType(PropertiesTypes::Position);

    m_GizmoMode = 0;

    if (!m_pGuizmoPropertyPosition)
    {
        SelectionManager::EnableGizmo(false);
        // ImGuizmo::Enable(false);
        return;
    }
    else
        m_GizmoMode = ImGuizmo::TRANSLATE;

    m_pGuizmoPropertyRotation = FindFirstPropertyByType(PropertiesTypes::Angles);

    if (m_pGuizmoPropertyRotation)
        m_GizmoMode |= ImGuizmo::ROTATE;

    SelectionManager::EnableGizmo(true);

    glm::vec3 pos = m_pGuizmoPropertyPosition->GetPosition();

    glm::vec3 ang = {0, 0, 0};

    if (m_pGuizmoPropertyRotation)
        ang = m_pGuizmoPropertyRotation->GetAngles();

    glm::vec3 scale = glm::vec3(1.f);

    glm::mat4 transform;

    if (m_pPropertiesBinding->GetSelectedObjects().size() > 1)
        transform = R_RotateForEntity(m_AllObjectsBounds.Center(), {0, 0, 0});
    else
        transform = R_RotateForEntity(m_AllObjectsBounds.Center(), ang);

    memcpy(&m_matGuizmo, &transform, sizeof(float) * 16);
    // ImGuizmo::RecomposeMatrixFromComponents(&pos.x, &ang2.x, &scale.x, &m_matGuizmo[0][0]);
}

VariantValue *ObjectPropertiesEditor::FindFirstPropertyByType(PropertiesTypes type)
{
    for (auto &it : m_lstProperties)
    {
        if (it->GetType() == type)
            return it;
    }

    return nullptr;
}

void ObjectPropertiesEditor::RenderFlagsEditor()
{
    if (ImGui::BeginPopupModal("Flags", 0, ImGuiWindowFlags_AlwaysAutoResize))
    {
        assert(m_pCurrentFlagProp);

        for (auto &it : m_pCurrentFlagProp->GetEnumValues())
        {
            if (ImGui::CheckboxFlags(it.first.c_str(), (int *)m_pCurrentFlagProp->Data(), it.second))
                UpdateProperty(m_pCurrentFlagProp);
        }

        ImGui::SetNextItemWidth(100);
        if (ImGui::Button("OK"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void ObjectPropertiesEditor::RenderPropetiesPane()
{
    if (ImGui::Begin(m_strPanelTitle))
    {
        CheckObjectValidity();

        if (m_lstProperties.size() > 0)
        {
            if (ImGuiHelpers::ButtonWithCommonIcon(CommonIcons::SortAlpha, "###Hello there!", 16))
            {
                m_lstProperties.sort([](const VariantValue *a, const VariantValue *b) {
                    return strcasecmp(a->DisplayName(), b->DisplayName()) < 0;
                });
            }

            if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
            {
                int i = 0;

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::SeparatorText(m_pPropertiesBinding->ObjectClassname());

                for (auto &it : m_lstProperties)
                {
                    ImGui::PushID(i++); // Use field index as identifier.

                    // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGuiTreeNodeFlags flags =
                        ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

                    ImGui::TreeNodeEx("Field", flags, "%s", it->DisplayName());

                    auto &help = it->Help();

                    if (!help.empty())
                    {
                        ImGui::SetItemTooltip("Description:\n%s", help.c_str());
                    }

                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);

                    RenderPropertyControl(it);

                    ImGui::PopID();
                }

                ImGui::EndTable();
            }

            if (m_pPropertiesBinding)
                m_pPropertiesBinding->RenderFooter();
        }
        else
        {
            if (!m_pPropertiesBinding)
                ImGui::Text("No object selected");
            else
                ImGui::Text("Object has no properties");
        }

        
    }
    ImGui::End();
}

void ObjectPropertiesEditor::RenderPropertyControl(VariantValue *it)
{
    VariantValue oldValue = *it;

    ImGui::PushID(it);

    switch (it->GetType())
    {
    case PropertiesTypes::Enum:

        if (it->GetEnumValues().size() > 0)
        {
            int selectionIndex = it->GetInt();
            it->SetTempLabel((char *)it->GetEnumValues()[selectionIndex].first.c_str());

            if (ImGui::BeginCombo(it->DisplayName(), it->TempLabel()))
            {
                int i = 0;

                for (auto &enumValue : it->GetEnumValues())
                {
                    if (ImGui::Selectable(enumValue.first.c_str(), i == selectionIndex))
                    {
                        it->SetInt(i);
                        it->SetTempLabel((char *)enumValue.first.c_str());
                        UpdateProperty(it);
                    }

                    i++;
                }

                ImGui::EndCombo();
            }
        }
        else
        {
            ImGui::Text("No values defined!");
        }

        break;
    case PropertiesTypes::Flags: {
        if (ImGui::Button("..."))
        {
            ImGui::OpenPopup(m_FlagsEditorID);
            m_pCurrentFlagProp = it;
        }

        ImGui::SameLine();

        std::string flagDescription = "[";

        for (auto &flagValue : it->GetEnumValues())
        {
            if (it->GetFlags() & flagValue.second)
                flagDescription += flagValue.first + ",";
        }

        if (flagDescription.length() > 1)
            flagDescription[flagDescription.length() - 1] = ']';
        else
            flagDescription += "<None set>]";

        ImGui::Text("%s", flagDescription.c_str());
    }
    break;
    case PropertiesTypes::Position:
        if (ImGui::DragFloat3("Vector", (float *)it->Data()))
        {
            SetupGuizmo();
            UpdateProperty(it);
        }
        break;
    case PropertiesTypes::ColorRGB:
        if (ImGui::ColorEdit3("##value", (float *)it->Data()))
            UpdateProperty(it);
        break;
    case PropertiesTypes::ColorRGBA:
        if (ImGui::ColorEdit4("##value", (float *)it->Data()))
            UpdateProperty(it);
        break;
    case PropertiesTypes::Float:
        if (ImGui::DragFloat("##value", (float *)it->Data(), 0.01f))
            UpdateProperty(it);
        break;
    case PropertiesTypes::Angles:
        if (ImGui::DragFloat3("Vector", (float *)it->Data()))
            UpdateProperty(it);
        break;
    case PropertiesTypes::Int:
        if (ImGui::DragInt("##value", (int *)it->Data()))
            UpdateProperty(it);
        break;
    case PropertiesTypes::SizeX:
        if (ImGui::DragFloat("Vector", (float *)it->Data()))
            UpdateProperty(it);
        break;
    case PropertiesTypes::String:
        if (ImGui::InputText("##Value", it->GetStdStringPtr()))
            UpdateProperty(it);
        break;
    default:
        break;
    }

    ImGui::PopID();

    if (ImGui::IsItemActivated())
    {
        // Here we record old value when item is actived for the first time.
        m_OldPropertyValue = oldValue;
    }

    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        // Here we can save undo (to undo stack or something). Triggered once just after user stops editing.
        // RecordUndo(oldValue, value);

        // TODO: refactor for multiple objects edits

        // assert(false && "fixme");
        // auto history = Application::GetMainWindow()->GetSceneRenderer()->GetScene()->GetEditHistory();
        // history->PushAction(new CPropertyChangeAction(m_pPropertiesBinding->GetSerialNumber(), m_OldPropertyValue,
        // it));

        m_pPropertiesBinding->OnPropertyChangeSavedToHistory();
    }
}

void ObjectPropertiesEditor::EditTransform(Viewport *pViewport, bool editTransformDecomposition)
{
    glm::mat4 &copyOfMain = m_matGuizmo;

    float *matrix      = (float *)&copyOfMain;
    float *deltaMatrix = (float *)&m_matDeltaGuizmo;

    auto   cam              = pViewport->GetCamera();
    float *cameraProjection = cam->GetProjectionMatrixPtr();
    float *cameraView       = cam->GetViewMatrixPtr();

    static ImGuizmo::OPERATION mCurrentGizmoOperation = m_pPropertiesBinding->GetMeaningfulGizmoOperationMode();

    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
    static bool           useSnap         = false;
    static float          snap[3]         = {1.f, 1.f, 1.f};
    static float          bounds[]        = {-5.f, -5.f, -5.f, 5.f, 5.f, 5.f};
    static float          boundsSnap[]    = {0.1f, 0.1f, 0.1f};
    static bool           boundSizing     = false;
    static bool           boundSizingSnap = false;

    if (editTransformDecomposition)
    {
        ImGui::Begin("Edit transform");
        if (ImGui::IsKeyPressed(ImGuiKey_T))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyPressed(ImGuiKey_R))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsKeyPressed(ImGuiKey_Y)) // r Key
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
            mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
        ImGui::InputFloat3("Tr", matrixTranslation);
        ImGui::InputFloat3("Rt", matrixRotation);
        ImGui::InputFloat3("Sc", matrixScale);
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

        if (mCurrentGizmoOperation != ImGuizmo::SCALE)
        {
            if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                mCurrentGizmoMode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                mCurrentGizmoMode = ImGuizmo::WORLD;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_U))
            useSnap = !useSnap;
        ImGui::Checkbox("##UseSnap", &useSnap);
        ImGui::SameLine();

        switch (mCurrentGizmoOperation)
        {
        case ImGuizmo::TRANSLATE:
            ImGui::InputFloat3("Snap", &snap[0]);
            break;
        case ImGuizmo::ROTATE:
            ImGui::InputFloat("Angle Snap", &snap[0]);
            break;
        case ImGuizmo::SCALE:
            ImGui::InputFloat("Scale Snap", &snap[0]);
            break;
        }
        ImGui::Checkbox("Bound Sizing", &boundSizing);
        if (boundSizing)
        {
            ImGui::PushID(3);
            ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
            ImGui::SameLine();
            ImGui::InputFloat3("Snap", boundsSnap);
            ImGui::PopID();
        }

        if (ImGui::Button("Reset transform"))
        {
            m_matGuizmo = glm::mat4(1);
        }

        ImGui::End();
    }

    ImGuiIO &io                  = ImGui::GetIO();
    float    viewManipulateRight = io.DisplaySize.x;
    float    viewManipulateTop   = 0;

    glm::vec2 viewportPos        = pViewport->GetClientAreaPosAbs();
    glm::vec2 viewportClientArea = pViewport->GetClientArea();
    float     win[4]             = {viewportPos.x, viewportPos.y, viewportClientArea.x, viewportClientArea.y};

    // TODO: check this,
    auto h = 0;

    ImGuizmo::AllowAxisFlip(false);

    // ImGuizmo::SetRect(win[0], win[3] - (win[3] - win[1]), win[2], win[3]);
    ImGuizmo::SetDrawlist();

    ImGuizmo::SetRect(win[0], win[1], win[2], win[3]);

    auto   rel     = m_AllObjectsBounds.ConvertToRelative();
    float *relData = rel.Data();

    for (int i = 0; i < 6; i++)
        bounds[i] = relData[i];

    bool bChanged = ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode,
                                         matrix, deltaMatrix, useSnap ? &snap[0] : NULL, m_mapRelativeObjectsPos.size() > 1 ?  bounds : nullptr, nullptr);

    glm::vec3 matrixScale;
    glm::vec3 matrixScaleDelta;
    glm::vec3 pos;
    glm::vec3 posDelta;
    glm::vec3 unused2;

    ImGuizmo::DecomposeMatrixToComponents(&m_matGuizmo[0][0], &pos.x, &unused2.x, &matrixScale.x);

    if (bChanged)
    {
        HandleGizmoChange();
        pViewport->FlagUpdate();

        ResetGuizmoScaling();
        UpdateAllObjectsBoundingBox();

        UpdateRelativePositions();
    }
    else
    {

        ImGuizmo::DecomposeMatrixToComponents(&m_matDeltaGuizmo[0][0], &posDelta.x, &unused2.x, &matrixScaleDelta.x);

        // TODO: fix this
        if (glm::length2(matrixScaleDelta) > 0.01)
        {
            m_pPropertiesBinding->OnSelectionResized(m_mapRelativeObjectsPos, matrixScale, pos);
            pViewport->FlagUpdate();
        }
    }
}

void ObjectPropertiesEditor::HandleGizmoChange()
{
    glm::vec3 pos, matrixScale;
    glm::vec3 matrixRotation;

    ImGuizmo::DecomposeMatrixToComponents(&m_matDeltaGuizmo[0][0], &pos.x, &matrixRotation.x, &matrixScale.x);

    if (m_pGuizmoPropertyRotation)
    {
        m_pGuizmoPropertyRotation->SetDeltaAngles(matrixRotation.xzy);
        UpdateProperty(m_pGuizmoPropertyRotation);
    }

    glm::vec3 oldCenter = m_AllObjectsBounds.Center();
    glm::vec3 delta     = m_matGuizmo[3].xyz - oldCenter;

    m_pGuizmoPropertyPosition->SetPosition(m_matGuizmo[3].xyz);

    UpdatePositionDelta(m_pGuizmoPropertyPosition, delta);

    m_AllObjectsBounds.Translate(delta);
}

/*
void CalculateTransformsBlenderLike()
{
    glm::mat4 &test = m_matGuizmo;

    glm::vec4 forward = test[0];
    glm::vec4 right   = test[2];
    glm::vec4 up      = test[1];
    glm::vec4 pos4    = test[3];

    test[3] = {0, 0, 0, 1};

    glm::vec4 vecs[] = {forward, right, up};

    for (int i = 0; i < 3; i++)
    {
        if (abs(matrixRotation[i]) < 0.001f)
            continue;

        auto r = glm::rotate(glm::mat4(1.0f), glm::radians(matrixRotation[i]), (glm::vec3)vecs[i].xyz);
        test   = r * test;
    }

    test[3] = pos4;

    glm::vec3 eulerTest = glm::vec3();

    // glm::extractEulerAngleYZX(test, eulerTest.x, eulerTest.y, eulerTest.z);
    eulerTest = rotationMatrixToEulerAngles(test);
    eulerTest = glm::degrees(eulerTest);

    m_pGuizmoPropertyRotation->SetAngles(eulerTest.xyz);
    UpdateProperty(m_pGuizmoPropertyRotation);

    //         SceneEntity *pEntity = m_pPropertiesBinding->GetEntity(0);
    //         if (pEntity)
    //             pEntity->SetTransform(test);
}

glm::vec3 rotationMatrixToEulerAngles(glm::mat4 in2)
{
    glm::vec3 angles;

    glm::mat4 in = glm::transpose(in2);

    float xyDist = sqrt(in[0][0] * in[0][0] + in[0][1] * in[0][1]);

    if (xyDist > 0.001f)
    {
        // enough here to get angles?
        angles[0] = atan2(-in[0][2], xyDist);
        angles[1] = atan2(in[0][1], in[0][0]);
        angles[2] = atan2(in[1][2], in[2][2]);
    }
    else
    {
        // forward is mostly Z, gimbal lock
        angles[0] = atan2(-in[0][2], xyDist);
        angles[1] = atan2(-in[1][0], in[1][1]);
        angles[2] = 0.0f;
    }

    return angles;
}

*/
