#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#ifndef __VECTOR_3D__
#include "vector3D.h"
#endif


typedef struct
{
	Vector3D pos;
	int hp;
	int ammo;
	int timeRef;
	bool reloading;
}stats;

class SoldierSMControl
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
	void attackState();
	void moveState();
	void coverState();
public:
	bool m_underFire;
	SoldierSMControl();
	~SoldierSMControl();
	void switchState();//choosing which state to use
	void update();
	void draw();//just draw
	bool underFire();
	bool isAlive();
};

#endif