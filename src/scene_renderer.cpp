#include "common.h"
#include "camera.h"
#include "scene_renderer.h"
#include "application.h"

SceneRenderer::SceneRenderer(MainWindow * pTargetWindow)
{
	m_pCamera = new Camera(this);
	m_pTargetWindow = pTargetWindow;
}

SceneRenderer::~SceneRenderer()
{
	delete m_pCamera;

	if (m_pSceneModel) delete m_pSceneModel;
}

void SceneRenderer::RenderScene()
{
	m_pCamera->Apply();

	Debug_DrawGround();

	if (m_pSceneModel) m_pSceneModel->DrawDebug();
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
	if (m_pSceneModel)
		delete m_pSceneModel;

	m_pSceneModel = new ModelOBJ(Application::GetFileSystem()->LoadFile(dropped_filedir));

}

void SceneRenderer::Debug_DrawGround()
{
	int dimensions = 1000;
	int step = 10;

	glBegin(GL_LINES);

	glColor3f(1, 0, 0);

	for (int x = -dimensions; x < dimensions; x += step)
	{
		glVertex3f(x, -dimensions, 0);
		glVertex3f(x, dimensions, 0);
	}

	glColor3f(0, 1, 0);

	for (int y = -dimensions; y < dimensions; y += step)
	{
		glVertex3f(-dimensions, y, 0);
		glVertex3f(dimensions, y, 0);
	}

	glEnd();

// 	glEnable(GL_DEPTH_TEST);
// 	glDisable(GL_TEXTURE_2D);
// 
// 
// 	glDisable(GL_CULL_FACE);
// 
// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_ONE, GL_ONE);
// 
// 	glColor3f(1, 0, 1);
// 	//glColor3ubv(&m_GroundColor[0]);
// 
// 	glBegin(GL_TRIANGLE_STRIP);
// 
// 	float z = -1;
// 
// 	glTexCoord2f(0.0f, 0.0f);
// 	glVertex3f(-100.0f, 100.0f, z);
// 
// 	glTexCoord2f(0.0f, 1.0f);
// 	glVertex3f(-100.0f, -100.0f, z);
// 
// 	glTexCoord2f(1.0f, 0.0f);
// 	glVertex3f(100.0f, 100.0f, z);
// 
// 	glTexCoord2f(1.0f, 1.0f);
// 	glVertex3f(100.0f, -100.0f, z);
// 
// 	glEnd();
// 
// 	glDisable(GL_BLEND);
// 	glEnable(GL_CULL_FACE);


	glEnable(GL_TEXTURE_2D);
}
