#pragma once

// Light flags from LB3k app

#define LF_EULER	(1<<0)
#define LF_DYN		(1<<1)
#define LF_XYZ		(1<<2)
#define LF_LINEAR	(1<<3)
#define LF_DISK		(1<<4)
#define LF_RECT		(1<<5)

class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();

	void RenderScene();
};

