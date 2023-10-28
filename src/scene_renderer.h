/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "main_window.h"
#include "lights.h"
#include "model_obj.h"
#include "edit_history.h"
#include "Scene.h"

class SceneRenderer: public IEventHandler
{	
public:
	SceneRenderer(class MainWindow* pTargetWindow);
	~SceneRenderer();

	class Camera* GetCamera();

	void	RenderScene();
	void	RenderHelperGeometry(SelectionManager* selectionManager);

	int		HandleEvent(bool bWasHandled, SDL_Event& e) override;
	float	FrameDelta();

	void	LoadModel(const char* dropped_filedir, bool keepLight = false);
	void	ReloadModel();

	void DrawBillboard(const glm::vec3 pos, const glm::vec2 size, const  gltexture_t* texture, const glm::vec3 tint);
	void DrawBillboardSelection(const glm::vec3 pos, const glm::vec2 size, const gltexture_t* texture, const int index);

	
	
	void ExportModelForCompiling(const char* path);

	void FocusCameraOnObject(SceneEntityPtr it);

	Scene* GetScene()
	{
		return m_pScene;
	}

	glm::vec3 GetNewLightPos();
private:
	void Debug_DrawGround();

	class Camera*		m_pCamera;
	class MainWindow*	m_pTargetWindow;
	Scene*		m_pScene;
	
	DrawMesh*	m_pUnitBoundingBox;
	DrawMesh*	m_pIntensitySphere;
	DrawMesh*	m_pSpotlightCone;
	
	ModelOBJ*	m_pDirectionModel;
	DrawMesh*	m_pDirectionArrow;

	void DrawLightHelperGeometry(SceneEntityWeakPtr pObject);
	lightDefWPtr_t m_pCurrentSelection;

};

