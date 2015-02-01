#ifndef __SOLDIER2_H__
#define __SOLDIER2_H__

#include "AI_header.h"
#include "MessageBoard.h"

#ifndef __QUEUE_H__
#include <queue>
#define __QUEUE_H__
#endif

#ifndef __DEQUE_H__
#include <deque>
#define __DEQUE_H__
#endif


enum soldierState2
{
	ATTACK=0,//
	DEAD,
	OTHER
};

class Soldier2:public baseObj
{
public:
	Soldier2(void);
	~Soldier2(void);
	bool Init();
	void Update(float delta);
	void Draw();
	bool SwitchState();
	bool UnderFire(float priority);
	void bulletHit(bullet* bul);
	bool IsAlive();
	TextureImage soldierTex;;
	bool m_isLeader;
	bool reqRepair;

private:
	bool recruiting;
	Vector3D* atkTarget;
	std::deque<bullet*> m_dodgeList;
	bool reqSquad;
	void Shoot();
	bool m_underFire;
	float m_spdMilt;
	int m_underFireLimit;
	soldierState2 m_state;
};

#endif