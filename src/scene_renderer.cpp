/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "common.h"
#include "camera.h"
#include "scene_renderer.h"
#include "application.h"
#include "common_resources.h"
#include "selection_3d.h"
#include "draw_utils.h"
#include "..\include\ImGuizmo\ImGuizmo.h"
#include "properties_editor.h"

SceneRenderer::SceneRenderer(MainWindow * pTargetWindow)
{
	m_pCamera = new Camera(this);
	m_pTargetWindow = pTargetWindow;

	m_pUnitBoundingBox = DrawUtils::MakeWireframeBox(glm::vec3(1,1,1));
	m_pIntensitySphere = DrawUtils::MakeWireframeSphere();
	m_pSpotlightCone = DrawUtils::MakeWireframeCone();
	
	
	auto fd = Application::GetFileSystem()->LoadFile("res/mesh/arrow.obj");
	
	m_pDirectionModel = new ModelOBJ(fd);
	m_pDirectionArrow = m_pDirectionModel->GetDrawMesh();
	
	m_pEditHistory = new CEditHistory;
}

SceneRenderer::~SceneRenderer()
{
	FreeVector(m_vecSceneLightDefs);

	delete m_pCamera;
	
	

	if (m_pUnitBoundingBox) delete m_pUnitBoundingBox;

	delete m_pEditHistory;

	delete m_pDirectionModel; m_pDirectionModel = 0;
	m_pDirectionArrow = 0;
}

void SceneRenderer::RenderScene()
{
	auto selectionManager = SelectionManager::Instance();
	m_pCamera->Apply();


	selectionManager->NewFrame(this);

	if (DEBUG_3D_SELECTION)
	{
		if (m_pSceneModel)
		{
			selectionManager->PushObject(m_pSceneModel);
		}

		for (auto& it : m_vecSceneLightDefs)
		{

			selectionManager->PushObject(it);
		}

		return;
	}
	

	auto pers = Application::Instance()->GetPersistentStorage();
	auto showGround = pers->GetSetting(ApplicationSettings::ShowGround);

	if (showGround->GetAsBool())
		Debug_DrawGround();
	
	if (m_pSceneModel)
	{
		m_pSceneModel->DrawShaded();
		selectionManager->PushObject(m_pSceneModel);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	lightDefWPtr_t selection;

	for (auto& it : m_vecSceneLightDefs)
	{
		DrawBillboard(it->pos, glm::vec2(4, 4), it->editor_icon, it->color);		
		selectionManager->PushObject(it);

		if (it->IsSelected())
		{
			selection = it;
			
		}
	}
	if (selection.lock())
		DrawLightHelperGeometry(selection);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	
}

int SceneRenderer::HandleEvent(bool bWasHandled, SDL_Event& e)
{
	return m_pCamera->HandleEvent(bWasHandled, e);
}

float SceneRenderer::FrameDelta()
{
	return m_pTargetWindow->FrameDelta();
}

void SceneRenderer::LoadModel(const char* dropped_filedir,bool keepLights)
{
	

	if (dropped_filedir)
		Application::GetPersistentStorage()->PushMRUFile(dropped_filedir);

	
	FileData* fd = nullptr;

	if (dropped_filedir)	
		fd = Application::GetFileSystem()->LoadFile(dropped_filedir);	
	else
	{
		auto s = GetModelFileName();
		const char* n = s.c_str();
		fd = Application::GetFileSystem()->LoadFile(n);
	}

	if (!fd)
	{
		// PopupError()
		// RemoveFromMRU()
		return;
	}

	m_pSceneModel = std::make_shared<ModelOBJ>(fd);
	
	SetSceneScale(m_pSceneModel->GetSceneScale());

	m_serialNubmerCounter = 1;

	if (!keepLights)
	{
		m_vecSceneLightDefs.clear();

		auto modelLightDefs = m_pSceneModel->ParsedLightDefs();

		for (auto& it : modelLightDefs)
		{
			int sn = AllocSerialNumber();
			AddLightToSceneWithSerialNumber(it, sn);
		}
	}

}

void SceneRenderer::AddLightToSceneWithSerialNumber(lightDef_t& it, int sn)
{
	it.serial_number = sn;
	m_vecSceneLightDefs.push_back(std::make_shared<lightDef_t>(it));
}

bool SceneRenderer::IsModelLoaded()
{
	return m_pSceneModel != nullptr;
}

std::string SceneRenderer::GetModelFileName()
{
	if (!m_pSceneModel)
		return "";

	return m_pSceneModel->GetModelFileName();
}

std::string SceneRenderer::GetModelTextureName()
{
	if (!m_pSceneModel)
		return "";

	return m_pSceneModel->GetModelTextureName();
}

void SceneRenderer::DrawBillboard(glm::vec3 pos, glm::vec2 size, gltexture_t* texture, glm::vec3 tint)
{
	auto right = m_pCamera->GetRightVector();
	auto up = m_pCamera->GetUpVector();

	glm::vec3 pt;

	int pointDef[4][2] =
	{
		{-1,-1},
		{1,-1},
		{1,1},
		{-1,1},
	};

	glBindTexture(GL_TEXTURE_2D, texture->gl_texnum);
	glColor3f(tint.x, tint.y, tint.z);	
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++)
	{
		pt = pos + (right * (size.x / 2) * (float)pointDef[i][0]) + (up * (size.y / 2) * (float) pointDef[i][1]);
		glTexCoord2f(pointDef[i][0] == -1 ? 0 : 1, pointDef[i][1] == -1 ? 0 : 1);
		glVertex3f(pt.x, pt.y, pt.z);
	}

	glEnd();
}

void SceneRenderer::DrawBillboardSelection(glm::vec3 pos, glm::vec2 size, gltexture_t* texture, int index)
{
	auto right = m_pCamera->GetRightVector();
	auto up = m_pCamera->GetUpVector();

	glm::vec3 pt;

	int pointDef[4][2] =
	{
		{-1,-1},
		{1,-1},
		{1,1},
		{-1,1},
	};

	glBindTexture(GL_TEXTURE_2D, texture->gl_texnum);
	glColor4ubv((GLubyte*)&index);
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++)
	{
		pt = pos + (right * (size.x / 2) * (float)pointDef[i][0]) + (up * (size.y / 2) * (float)pointDef[i][1]);
		glTexCoord2f(pointDef[i][0] == -1 ? 0 : 1, pointDef[i][1] == -1 ? 0 : 1);
		glVertex3f(pt.x, pt.y, pt.z);
	}

	glEnd();
}

lightDefWPtr_t SceneRenderer::GetLightWeakRef(lightDef_s* param1)
{
	for (auto& it : m_vecSceneLightDefs)
	{
		if (it.get() == param1)
			return lightDefWPtr_t(it);
	}

	return lightDefWPtr_t();
}

void SceneRenderer::ExportModelForCompiling(const char* path)
{
	if (!m_pSceneModel)
		return;

	m_pSceneModel->ClearLightDefinitions();
	
	for (auto& it : m_vecSceneLightDefs)
	{
		m_pSceneModel->AddLight(it);
	}

	auto settings = Application::Instance()->GetLightBakerApplication()->Settings();

	m_pSceneModel->SetLightmapDimensions(settings->m_lmSettings.size[0], settings->m_lmSettings.size[1]);

	if (path)
		m_pSceneModel->Export(path);
	else
		m_pSceneModel->Export(m_pSceneModel->GetModelFileName().c_str());


}

Camera* SceneRenderer::GetCamera()
{
	return m_pCamera;
}

void SceneRenderer::ReloadModel()
{
	if (!m_pSceneModel)
		return;

	m_pSceneModel->ReloadTextures();
}

void SceneRenderer::Debug_DrawGround()
{
	int dimensions = 1000;
	int step = 10;

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);

	glColor3f(1, 0, 0);

	for (int x = -dimensions; x < dimensions; x += step)
	{
		glVertex3f(x,0, -dimensions);
		glVertex3f(x,0, dimensions);
	}

	glColor3f(0, 1, 0);

	for (int y = -dimensions; y < dimensions; y += step)
	{
		glVertex3f(-dimensions, 0, y);
		glVertex3f(dimensions,	0, y);
	}

	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void SceneRenderer::DrawLightHelperGeometry(lightDefWPtr_t pObject)
{
	auto ptr = pObject.lock();
	
	if (!ptr)
		return;

	switch (ptr->type)
	{
	case LightTypes::Omni:
		break;
	case LightTypes::Spot:
	{

		auto shader = GLBackend::Instance()->SpotlightConeShader();

		shader->Bind();
		shader->SetProjection(m_pCamera->GetProjectionMatrix());
		shader->SetView(m_pCamera->GetViewMatrix());
		shader->SetColor(glm::vec4(glm::vec3(1, 1, 1) - ptr->color, 1));

		glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), ptr->pos);
		
		shader->SetScale(1);
		shader->SetConeHeight(ptr->intensity);
		shader->SetConeAngleDegrees(ptr->cones[1]);
		shader->SetTransform(mat);

		auto mat1 = glm::rotate(glm::mat4x4(1), glm::radians(ptr->anglesDirection[0]), glm::vec3(1, 0, 0));
		auto mat2 = glm::rotate(glm::mat4x4(1), glm::radians(ptr->anglesDirection[1]), glm::vec3(0, 1, 0));
		auto mat3 = glm::rotate(glm::mat4x4(1), glm::radians(ptr->anglesDirection[2]), glm::vec3(0, 0, 1));

		mat *= mat1;
		mat *= mat2;
		mat *= mat3;
		
		shader->SetTransform(mat);		
		m_pSpotlightCone->BindAndDraw();

		shader->Unbind();
	}
		break;
	case LightTypes::Direct:
		break;
	default:
		break;

	}

	

// 
// 	//m_pUnitBoundingBox->BindAndDraw();
// 

// 	
// 	float f = sqrt(ptr->intensity);
// 	shader->SetScale(f);
// 
// 	//m_pIntensitySphere->BindAndDraw();
// 

}

int SceneRenderer::AllocSerialNumber()
{
	return m_serialNubmerCounter++;
}

void SceneRenderer::FocusCameraOnObject(lightDefPtr_t& it)
{
	m_pCamera->LookAtPoint(it->pos);
}

lightDefWPtr_t SceneRenderer::GetLightBySerialNumber(int serialNumber)
{
	for (auto& it : m_vecSceneLightDefs)
	{
		if (it->serial_number == serialNumber)
			return lightDefWPtr_t(it);
	}

	return lightDefWPtr_t();
}

void SceneRenderer::DeleteLightWithSerialNumber(int serialNumber)
{
	auto lightPtr = GetLightBySerialNumber(serialNumber);
	DeleteLight(lightPtr);
}

float SceneRenderer::GetSceneScale()
{
	return m_flSceneScale;
}

void SceneRenderer::SetSceneScale(float f)
{
	m_flSceneScale = f;
}

void SceneRenderer::RescaleLightPositions(float m_flScaleOriginal, float m_flScale)
{
	for (auto& it : m_vecSceneLightDefs)
	{
		it->pos /= m_flScaleOriginal;
		it->pos *= m_flScale;
	}

	ObjectPropertiesEditor::Instance()->UnloadObject();

	// Not really necessary
	//Application::ScheduleCompilationIfNecceseary();
}

void SceneRenderer::AddNewLight(LightTypes type)
{
	lightDef_s* newLight = new lightDef_s;
	

	newLight->pos = m_pCamera->GetOrigin() + m_pCamera->GetForwardVector() * 10.f;
	newLight->type = type;
	newLight->UpdateEditorIcon();
	newLight->intensity = 10;

	float hue = (float)rand() / 32768.f;

	ImGui::ColorConvertHSVtoRGB(hue, 1, 1,
		newLight->color.x,
		newLight->color.y,
		newLight->color.z);

	newLight->serial_number = AllocSerialNumber();

	m_vecSceneLightDefs.push_back(lightDefPtr_t(newLight));
	newLight->InvokeSelect();
}

std::vector<lightDefPtr_t>& SceneRenderer::GetSceneObjects()
{
	return m_vecSceneLightDefs;
}

void SceneRenderer::HintSelected(lightDefWPtr_t weakRef)
{
	m_pCurrentSelection = weakRef;
}

lightDefWPtr_t SceneRenderer::GetSelection()
{
	return m_pCurrentSelection;	
}

void SceneRenderer::DeleteLight(lightDefWPtr_t l)
{
	auto ptr = l.lock();

	if (!ptr)
		return;

	auto pos = std::remove_if(m_vecSceneLightDefs.begin(), m_vecSceneLightDefs.end(), [&](lightDefPtr_t& it)
		{
			return it == ptr;
		});
		

	m_vecSceneLightDefs.erase(pos);
}

void SceneRenderer::DoDeleteSelection()
{	
	auto sel = GetSelection();
	auto ptr = sel.lock();

	if (!ptr)
		return;

	m_pEditHistory->PushAction(new CDeleteLightAction(ptr.get()));

	DeleteLight(sel);
	Application::ScheduleCompilationIfNecceseary();
}

CEditHistory* SceneRenderer::GetEditHistory() const
{
	return m_pEditHistory;
}
