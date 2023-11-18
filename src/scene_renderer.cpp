/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "common.h"
#include "camera.h"
#include "scene_renderer.h"
#include "scene.h"
#include "application.h"
#include "common_resources.h"
#include "selection_3d.h"
#include "draw_utils.h"
#include "..\include\ImGuizmo\ImGuizmo.h"
#include "properties_editor.h"
#include "grid_renderer.h"

SceneRenderer::SceneRenderer(MainWindow* pTargetWindow)
{
	m_pCamera = new Camera(this);
	m_pTargetWindow = pTargetWindow;
	m_pScene = nullptr;

	m_pUnitBoundingBox = DrawUtils::MakeWireframeBox(glm::vec3(1, 1, 1));
    m_pIntensitySphere = DrawUtils::MakeIcosphere(2);
	m_pSpotlightCone = DrawUtils::MakeWireframeCone();

	m_pDirectionModel = new ModelOBJ("res/mesh/arrow.obj");
	m_pDirectionArrow = m_pDirectionModel->GetDrawMesh();
	RegisterRendermodesCommands();

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::DumpLightmapMesh, "Dump lightmap mesh", 0, 0, 0,
		[&]()
		{
			DumpLightmapMesh();
		}));

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::DumpLightmapUV, "Dump lightmap uv", 0, 0, 0,
		[&]()
		{
			DumpLightmapUV();
		}));

	
	GridRenderer::Instance()->Init();
}


void SceneRenderer::RegisterRendermodesCommands()
{
	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::LightshadedRenderMode, "Lightshaded", 0, 0, 0,
		[&]()
		{
			m_RenderMode = RenderMode::Lightshaded;
		}));

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::UnshadedRenderMode, "Unshaded (diffuse only)", 0, 0, 0,
		[&]()
		{
			m_RenderMode = RenderMode::Unshaded;
		}));

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::WireframeLightshadedRenderMode, "Lightshaded wireframe", 0, 0, 0,
		[&]()
		{
			m_RenderMode = RenderMode::WireframeShaded;
		}));

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::WireframeUnshadedRenderMode, "Unshaded wireframe", 0, 0, 0,
		[&]()
		{
			m_RenderMode = RenderMode::WireframeUnshaded;
		}));

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::GroupShadedRenderMode, "Groups shaded", 0, 0, 0,
		[&]()
		{
			m_RenderMode = RenderMode::Groups;
		}));

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::ReloadAllShaders, "Reload all shaders", 0, 0, 0,
		[&]()
		{
			GLBackend::Instance()->ReloadAllShaders();
		}));
}

SceneRenderer::~SceneRenderer()
{
	delete m_pCamera;
	delete m_pScene;
	delete m_pUnitBoundingBox;

	delete SelectionManager::Instance();
    delete GridRenderer::Instance();;
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
		m_pScene->RenderObjectsFor3DSelection();
		return;
	}

	// Render visible stuff

	auto pers = Application::Instance()->GetPersistentStorage();
	auto showGround = pers->GetSetting(ApplicationSettings::ShowGround);



	if (m_pScene)
	{
		switch (m_RenderMode)
		{
		case RenderMode::Unshaded:
			m_pScene->RenderUnshaded();
			break;
		case RenderMode::Lightshaded:
			m_pScene->RenderLightShaded();
			break;
		case RenderMode::WireframeUnshaded:
			break;
		case RenderMode::WireframeShaded:
			break;
		case RenderMode::Groups:
			m_pScene->RenderGroupsShaded();
		default:
			break;
		}

		RenderHelperGeometry(selectionManager);
	}

	if (showGround->GetAsBool())
    {
        GridRenderer::Instance()->Render();
    }


}

void SceneRenderer::RenderHelperGeometry(SelectionManager* selectionManager)
{
	SceneRenderer* sceneRenderer = Application::GetMainWindow()->GetSceneRenderer();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	SceneEntityWeakPtr selection;

	for (auto& it : m_pScene->GetLightDefs())
	{
		if (!it)
			return;

		if (!it->IsLightEntity())
			continue;

		sceneRenderer->DrawBillboard(it->GetPosition(), glm::vec2(4, 4), it->GetEditorIcon(), it->GetColor());
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

void SceneRenderer::LoadModel(const char* dropped_filedir, int loadFlags)
{
	if (!m_pScene)
		m_pScene = new Scene;

	m_pScene->LoadLevel(dropped_filedir, loadFlags);

	if (dropped_filedir)
		Application::GetPersistentStorage()->PushMRUFile(dropped_filedir);


	auto fileName = FileSystem::Instance()->ExtractFileName(dropped_filedir);

	Application::GetMainWindow()->SetTitle(fileName);


	// 	if (dropped_filedir)
	// 		Application::GetPersistentStorage()->PushMRUFile(dropped_filedir);
	// 
	// 	
	// 	FileData* fd = nullptr;
	// 
	// 	if (dropped_filedir)	
	// 		fd = Application::GetFileSystem()->LoadFile(dropped_filedir);	
	// 	else
	// 	{
	// 		// TODO: fixme
	// 		m_pSceneModel->ReloadTextures();
	// 	}
	// 
	// 	if (!fd)
	// 	{
	// 		// PopupError()
	// 		// RemoveFromMRU()
	// 		return;
	// 	}

		//m_pSceneModel = std::make_shared<ModelOBJ>(dropped_filedir);
		//SetSceneScale(m_pSceneModel->GetSceneScale());
}

void SceneRenderer::DrawBillboard(const glm::vec3 pos, const glm::vec2 size, const gltexture_t* texture, const glm::vec3 tint)
{
    return;

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
		pt = pos + (right * (size.x / 2) * (float)pointDef[i][0]) + (up * (size.y / 2) * (float)pointDef[i][1]);
		glTexCoord2f(pointDef[i][0] == -1 ? 0 : 1, pointDef[i][1] == -1 ? 0 : 1);
		glVertex3f(pt.x, pt.y, pt.z);
	}

	glEnd();
}

void SceneRenderer::DrawBillboardSelection(const glm::vec3 pos, const glm::vec2 size, const gltexture_t* texture, const int index)
{
    return;

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

Camera* SceneRenderer::GetCamera()
{
	return m_pCamera;
}

void SceneRenderer::ReloadScene(int loadFlags)
{
	assert(m_pScene);
	m_pScene->Reload(loadFlags);
}

void SceneRenderer::Debug_DrawGround()
{

}

void SceneRenderer::DrawLightHelperGeometry(SceneEntityWeakPtr pObject)
{
	auto _ptr = pObject.lock();

	if (!_ptr)
		return;

	lightDefPtr_t ptr = std::dynamic_pointer_cast<Lb3kLightEntity>(_ptr);

	switch (ptr->type)
	{
	case LightTypes::Omni:
	{
		auto shader = GLBackend::Instance()->HelperGeometryShader();

		shader->Bind();
		shader->SetDefaultCamera();
        shader->SetTransformIdentity();

		shader->SetColor(glm::vec4(glm::vec3(1, 1, 1) - ptr->GetColor(), 1));
		shader->SetScale(ptr->intensity);

		glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), ptr->GetPosition());
		shader->SetTransform(mat);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
		m_pIntensitySphere->BindAndDraw();

		glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		shader->Unbind();
	}
	break;
	case LightTypes::Spot:
	{

		auto shader = GLBackend::Instance()->SpotlightConeShader();

		shader->Bind();
        shader->SetDefaultCamera();
		shader->SetColor(glm::vec4(glm::vec3(1, 1, 1) - ptr->GetColor(), 1));

		glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), ptr->GetPosition());

		shader->SetScale(1);
		shader->SetConeHeight(ptr->intensity);
		shader->SetConeAngleDegrees(ptr->cones[1]);
		shader->SetTransform(mat);

		auto mat1 = glm::rotate(glm::mat4x4(1), glm::radians(ptr->anglesDirection[0]), glm::vec3(1, 0, 0));
		auto mat2 = glm::rotate(glm::mat4x4(1), glm::radians(ptr->anglesDirection[1]), glm::vec3(0, 1, 0));
		auto mat3 = glm::rotate(glm::mat4x4(1), glm::radians(ptr->anglesDirection[2]), glm::vec3(0, 0, 1));

		// 		mat *= mat1;
		// 		mat *= mat2;
		// 		mat *= mat3;

				// Quake-ish order
		mat *= mat3;
		mat *= mat2;
		mat *= mat1;

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

}

RenderMode SceneRenderer::GetRenderMode()
{
	return m_RenderMode;
}

void SceneRenderer::DumpLightmapMesh()
{
	m_pScene->DumpLightmapMesh();
}

void SceneRenderer::DumpLightmapUV()
{
	m_pScene->DumpLightmapUV();
}

void SceneRenderer::SetRenderMode(RenderMode newMode)
{
	m_RenderMode = newMode;
}

void SceneRenderer::RenderGenericEntity(SceneEntity* pEntity)
{
	auto shader = GLBackend::Instance()->HelperGeometryShader();

	shader->Bind();
	shader->SetDefaultCamera();
    shader->SetTransformIdentity();

	glm::vec4 col = glm::vec4(pEntity->GetColor(),1);
	shader->SetColor(col);
	shader->SetScale(16);

	glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), pEntity->GetPosition());
	shader->SetTransform(mat);

	m_pUnitBoundingBox->BindAndDraw();
	shader->Unbind();
}

void SceneRenderer::FocusCameraOnObject(SceneEntityPtr it)
{
	m_pCamera->LookAtPoint(it->GetPosition());
}

Scene* SceneRenderer::GetScene()
{
	if (!m_pScene)
		m_pScene = new Scene();

	return m_pScene;
}

glm::vec3 SceneRenderer::GetNewLightPos()
{
	return m_pCamera->GetOrigin() + m_pCamera->GetForwardVector() * 10.f;
}
