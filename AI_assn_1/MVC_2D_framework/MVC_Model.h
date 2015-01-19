#ifndef __MVC_MODEL_H__
#define __MVC_MODEL_H__

#ifndef __TEXTURE_H__
#include "texture.h"
#endif

#ifndef __VECTOR_H__
#include <vector>
#define __VECTOR_H__
#endif

#ifndef __SOLDIER_H__
#include "soldier.h"
#endif

#include "Engineer.h"
#include "Turret.h"
#include "MessageBoard.h"
#include "Soldier2.h"

class MVCTime;

class MVC_Model
{
public:
	MVC_Model(void);
	~MVC_Model(void);
	// Update the model
	void Update(void);
	bool Init(float fpsLimit);
	bool InitPhase2(void);//init after view's init e.g Textures,stuff requiring screen size
	
	float m_worldSizeX;
	float m_worldSizeY;

	int timeHolder;

	SquadBoard theSquad;
	
	void ProcMessages();

	TextureImage background;
	MVCTime* m_timer;
	bool pause;
private: 
};

#endif