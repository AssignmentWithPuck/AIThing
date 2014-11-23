#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#ifndef __AI_HEADER_H__
#include "AI_header.h"
#endif

#ifndef __VECTOR_3D_H__
#include "vector3D.h"
#endif

#ifndef __QUEUE_H__
#include <queue>
#endif


typedef struct
{
	int hp;
	int ammo;
	int timeRef;//for misc operations
	int bulletRef;//for reloading and shooting
	bool reloading;
	bool proning;
}stats;

class SoldierSMControl:public baseObj
{
public:
	enum moveSubState
	{
		MOVEFORWARD=0,
		MOVETOCOVER,
		MOVETOSHOOT,
	};

	enum soldierState
	{
		COVER=0,//
		MOVE,//
		ATTACK,//
		RELOAD,//
		DAMAGE,
		DEAD,
	};
private:
	soldierState m_state;
	moveSubState m_moveState;
	stats m_stats;
	bool m_underFire;
	float m_spdMult;

	int underFireLimit;

	void Shoot();

	std::queue<Vector3D> m_nearestCover;
	std::queue<Vector3D> m_nearestEnemy;

	void AttackState();
	void MoveState();
	void CoverState();
public:
	SoldierSMControl();
	~SoldierSMControl();
	void SwitchState();//choosing which state to use
	void Update(float delta);
	void Draw();//just draw
	void UnderFire(float priority);
	bool IsAlive();
};

#endif