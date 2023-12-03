/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "camera.h"
#include "scene_renderer.h"
#include "scene.h"
#include "common_resources.h"
#include "selection_3d.h"
#include "draw_utils.h"
#include "..\include\ImGuizmo\ImGuizmo.h"
#include "properties_editor.h"
#include "grid_renderer.h"

#define FAST_BB

SceneRenderer::SceneRenderer(MainWindow* pTargetWindow)
{
	m_pCamera = new Camera(this);
	m_pTargetWindow = pTargetWindow;
	m_pScene = nullptr;

	m_pUnitBoundingBox = DrawUtils::MakeWireframeBox(glm::vec3(1, 1, 1));
    m_pIntensitySphere = DrawUtils::MakeIcosphere(2);
	m_pSpotlightCone = DrawUtils::MakeWireframeCone();
    m_pSolidCube       = DrawUtils::MakeCube(1);
		
	SetupBuildboardsRenderer();


	RegisterRendermodesCommands();
		
	GridRenderer::Instance()->Init();
}


void SceneRenderer::SetupBuildboardsRenderer()
{
    float s = 1;

    m_pBillBoard = new DrawMesh();

	// TODO: try triangle strip

    m_pBillBoard->Begin(GL_TRIANGLES);

    m_pBillBoard->TexCoord2f(0, 0);
    m_pBillBoard->Normal3f(-1, -1, 0);
    m_pBillBoard->Vertex2f(0, 0); // 0 Left Up

    m_pBillBoard->TexCoord2f(1, 0);
    m_pBillBoard->Normal3f(1, -1, 0);
    m_pBillBoard->Vertex2f(0, 0); // 1 Right Up

    m_pBillBoard->TexCoord2f(0, 1);
    m_pBillBoard->Normal3f(-1, 1, 0);
    m_pBillBoard->Vertex2f(0, 0); // 2 Left Down

    m_pBillBoard->TexCoord2f(1, 1);
    m_pBillBoard->Normal3f(1, 1, 0);
    m_pBillBoard->Vertex2f(0, 0); // 3 Right Down

	int indicies[] = {0, 1, 2, 1, 3, 2};
    m_pBillBoard->Element1iv(indicies, ARRAYSIZE(indicies));

	// 012 - 132

    m_pBillBoard->End();

	
	
    m_pBillBoardsShader = GLBackend::Instance()->QueryShader("res/glprogs/billboard.glsl", {});
    
	std::list<const char *> defs;
	defs.push_back("SELECTION");
    m_pBillBoardsShaderSel = GLBackend::Instance()->QueryShader("res/glprogs/billboard.glsl", defs);
}

void SceneRenderer::RegisterRendermodesCommands()
{
    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::DumpLightmapMesh, "Dump lightmap mesh", 0, 0, 0, [&]() { DumpLightmapMesh(); }));

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::DumpLightmapUV, "Dump lightmap uv", 0, 0, 0, [&]() { DumpLightmapUV(); }));

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
    delete m_pBillBoard;
    delete m_pSolidCube;
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

#ifdef FAST_BB
	m_pBillBoardsShader->Bind();
    m_pBillBoard->Bind();

		for (auto &it : m_pBillBoardsShader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Color:
        case UniformKind::TransformMatrix:          
        case UniformKind::Scale:        
        case UniformKind::Diffuse:        
            break;
		default:
            GLBackend::SetUniformValue(it);
            break;
        }
    }

#else
    m_pBillBoard->Unbind();

    glDisable(GL_CULL_FACE);
    glUseProgram(0);
#endif
	
	

	for (auto& it : m_pScene->GetLightDefs())
	{
		if (!it)
			return;

		if (!it->IsLightEntity())
			continue;
		
		
        sceneRenderer->DrawBillboard(it->GetPosition(), glm::vec2(8, 8), it->GetEditorIcon(), it->GetColor());
        
		selectionManager->PushObject(it);

		if (it->IsSelected())
		{
			selection = it;
		}
	}


 	if (selection.lock() && false)
     {
         DrawLightHelperGeometry(selection);
     }

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

}

void SceneRenderer::DrawBillboard(const glm::vec3 pos, const glm::vec2 size, const GLTexture* texture, const glm::vec3 tint)
{
#ifdef FAST_BB

	// TODO: make render-chains for transparent, billboards, etc
	m_pBillBoardsShader->Bind();
    m_pBillBoard->Bind();

    for (auto &it : m_pBillBoardsShader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Color:
        case UniformKind::TransformMatrix:
        case UniformKind::Scale:
        case UniformKind::Diffuse:
            break;
        default:
            GLBackend::SetUniformValue(it);
            break;
        }
    }


	glm::mat4 matTransform = glm::translate(glm::mat4(1),pos);


	for (auto & it: m_pBillBoardsShader->Uniforms())
    {
        switch (it->Kind())
        {        
        case UniformKind::Color:
            it->SetFloat4({tint.xyz, 1});
            break;
        case UniformKind::TransformMatrix:
            it->SetMat4(matTransform);
            break;
        case UniformKind::Scale:
	        it->SetFloat3({size.xyy});                   
            break;
		}
	}

	GLBackend::BindTexture(0, texture);
    GLBackend::BindTexture(1, nullptr);
	m_pBillBoard->Draw();
#else 

	auto right    = GetCamera()->GetRightVector();    
    auto up       = GetCamera()->GetUpVector();

 	glm::vec3 pt;
 
 	int pointDef[4][2] =
 	{
 		{-1,-1},
 		{1,-1},
 		{1,1},
 		{-1,1},
 	};

  	glBindTexture(GL_TEXTURE_2D, texture->GLTextureNum());
 	glColor3f(tint.x, tint.y, tint.z);
 	glBegin(GL_QUADS);
 
 	for (int i = 0; i < 4; i++)
	{
 		pt = pos + (right * (size.x / 2) * (float)pointDef[i][0]) + (up * (size.y / 2) * (float)pointDef[i][1]);
 		glTexCoord2f(pointDef[i][0] == -1 ? 0 : 1, pointDef[i][1] == -1 ? 0 : 1);
 		glVertex3f(pt.x, pt.y, pt.z);
 	}
 
 	glEnd();

#endif
}

void SceneRenderer::DrawBillboardSelection(const glm::vec3 pos, const glm::vec2 size, const int index)
{
    m_pBillBoardsShaderSel->Bind();
    m_pBillBoard->Bind();

	glm::mat4 matTransform = glm::translate(glm::mat4(1), pos);

    for (auto &it : m_pBillBoardsShaderSel->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Color: 
		{
            byte *pixel = (byte*)&index;
            it->SetFloat4({pixel[0] / 255.f, pixel[1] / 255.f, pixel[2] / 255.f, pixel[3] / 255.f});
            //it->SetFloat4({1.f,1.f,1.f,1.f});
        }
            break;
        case UniformKind::TransformMatrix:
            it->SetMat4(matTransform);
            break;
        case UniformKind::Scale:
            it->SetFloat3({size.xyy});
            break;
        default:
            GLBackend::SetUniformValue(it);
            break;
        }
    }

	m_pBillBoard->Draw();

#if 0
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

	glBindTexture(GL_TEXTURE_2D, texture->GLTextureNum());
	glColor4ubv((GLubyte*)&index);
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++)
	{
		pt = pos + (right * (size.x / 2) * (float)pointDef[i][0]) + (up * (size.y / 2) * (float)pointDef[i][1]);
		glTexCoord2f(pointDef[i][0] == -1 ? 0 : 1, pointDef[i][1] == -1 ? 0 : 1);
		glVertex3f(pt.x, pt.y, pt.z);
	}

	glEnd();

#endif
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
        shader->SetScale({ptr->intensity, ptr->intensity, ptr->intensity});

		glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), ptr->GetPosition());
		shader->SetTransform(mat);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
		//m_pIntensitySphere->BindAndDraw();

		m_pSolidCube->BindAndDraw();

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

void SceneRenderer::RenderPointEntityDefault(const glm::vec3 & m_Position, const glm::vec3 & m_Mins, const glm::vec3 & m_Maxs,
                                             const glm::vec3 & m_Color)
{
    auto shader = GLBackend::Instance()->HelperGeometryShader();

    shader->Bind();
    shader->SetDefaultCamera();
    shader->SetTransformIdentity();

    shader->SetColor({m_Color.xyz, 1});

	glm::vec3 scale = m_Maxs - m_Mins;

	glm::vec3 offset = m_Mins + scale * 0.5f;
    
	shader->SetScale(scale.xzy);

    glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), m_Position - offset);
    shader->SetTransform(mat);
	
    m_pSolidCube->BindAndDraw();
	    
    shader->Unbind();
}

glm::vec3 SceneRenderer::GetRenderPos()
{
    return m_pCamera->GetOrigin();
}



void SceneRenderer::DrawPointEntitySelection(glm::vec3 m_Position, glm::vec3 m_Mins, glm::vec3 m_Maxs, int objectId)
{

    auto shader = GLBackend::Instance()->HelperGeometryShader();

    shader->Bind();
    shader->SetDefaultCamera();
    shader->SetTransformIdentity();

	byte *    pixel = (byte *)&objectId;
    glm::vec4 col = {pixel[0] / 255.f, pixel[1] / 255.f, pixel[2] / 255.f, pixel[3] / 255.f};

    shader->SetColor({col});

    glm::vec3 scale = m_Maxs - m_Mins;

    glm::vec3 offset = m_Mins + scale * 0.5f;

    shader->SetScale(scale.xzy);

    glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), m_Position - offset);
    shader->SetTransform(mat);

    m_pSolidCube->BindAndDraw();

    shader->Unbind();

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
    shader->SetScale({16.f, 16.f, 16.f});

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
