#pragma once
#include "vector3D.h"
#include "AI_header.h"
class CEngineer : public baseObj
{
public:
	enum moveSubState
{
	MOVETOLEFT = 0,
	MOVETOMIDDLE,
	MOVETORIGHT,
	MOVETORECHARGE,
};

	enum engineerState
{
	IDLE = 0,
	REPAIR,
	RECHARGE,
	DAMAGED,
	DEAD,
};

typedef struct
{
	int hp;
	int charge;
	int timeRef;
}stats;

private:
	moveSubState m_moveState;
	engineerState m_state;
	stats m_stats;
	void repairState();
	void rechargeState();
	void idleState();
public:
	CEngineer(void);
	~CEngineer(void);
	void SwitchState();
	void Update(float delta);
	void Draw();
	bool isAlive();

};

