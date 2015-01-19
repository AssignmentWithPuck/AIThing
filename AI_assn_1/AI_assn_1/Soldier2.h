#ifndef __SOLDIER2_H__
#define __SOLDIER2_H__

#include "AI_header.h"
#include "MessageBoard.h"


struct stats2
{
	int hp;
};

enum soldierState2
{
	SOMETHING
};

class Soldier2:public baseObj
{
public:
	Soldier2(void);
	~Soldier2(void);
	bool Init();
	void Update(float delta);
	void Draw();
	void SwitchState();
	void UnderFire(float priority);
	void bulletHit(bullet* bul);
	bool IsAlive();
	TextureImage soldierTex;;
	stats2 m_stats;

private:
	SquadBoard* squadBoard;
	objType type;
	MessageStruc* m_currentOrders;
	bool m_isLeader;
	void Shoot();
	bool m_underFire;
	float m_spdMilt;
	int m_underFireLimit;
	Vector3D m_atkTarget;
};

#endif