#include "common.h"
#include "camera.h"
#include "scene_renderer.h"
#include "application.h"
#include "common_resources.h"
#include "selection_3d.h"
#include "draw_utils.h"
#include "..\include\ImGuizmo\ImGuizmo.h"

SceneRenderer::SceneRenderer(MainWindow * pTargetWindow)
{
	m_pCamera = new Camera(this);
	m_pTargetWindow = pTargetWindow;

	m_pUnitBoundingBox = DrawUtils::MakeWireframeBox(glm::vec3(1,1,1));
	m_pIntensitySphere = DrawUtils::MakeWireframeSphere();
}

SceneRenderer::~SceneRenderer()
{
	FreeVector(m_vecSceneLightDefs);

	delete m_pCamera;
	if (m_pSceneModel) delete m_pSceneModel;

	if (m_pUnitBoundingBox) delete m_pUnitBoundingBox;
}

void SceneRenderer::RenderScene()
{
	auto selectionManager = SelectionManager::Instance();
	m_pCamera->Apply();

	selectionManager->NewFrame(this);
	Debug_DrawGround();
	
	if (m_pSceneModel) m_pSceneModel->DrawDebug();

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
		DrawBoundingBoxAroundLight(selection);

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

void SceneRenderer::LoadModel(char* dropped_filedir)
{
	Application::GetPersistentStorage()->PushMRUFile(dropped_filedir);

	if (m_pSceneModel)
		delete m_pSceneModel;

	m_pSceneModel = new ModelOBJ(Application::GetFileSystem()->LoadFile(dropped_filedir));

	m_vecSceneLightDefs.clear();

	auto modelLightDefs = m_pSceneModel->ParsedLightDefs();

	for (auto& it : modelLightDefs)
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

	m_pSceneModel->Export(path);
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

void SceneRenderer::DrawBoundingBoxAroundLight(lightDefWPtr_t pObject)
{
	auto ptr = pObject.lock();
	
	if (!ptr)
		return;

	glTranslatef(ptr->pos[0], ptr->pos[1], ptr->pos[2]);
	

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor4f(1-ptr->color[0], 1 - ptr->color[1], 1 - ptr->color[2],1);

	glPushMatrix();
	glScalef(4, 4, 4);
	m_pUnitBoundingBox->BindAndDraw();
	glPopMatrix();
	

	glPushMatrix();

	float f = sqrt(ptr->intensity);

	glScalef(f, f, f);
	m_pIntensitySphere->BindAndDraw();
	glPopMatrix();
	

	glTranslatef(-ptr->pos[0], -ptr->pos[1], -ptr->pos[2]);
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

	m_vecSceneLightDefs.push_back(lightDefPtr_t(newLight));
	newLight->InvokeSelect();
}
