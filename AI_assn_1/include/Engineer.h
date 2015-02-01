#pragma once
#include "vector3D.h"
#include "AI_header.h"
#include "Turret.h"
#include "MessageBoard.h"
#include "Soldier2.h"

class CEngineer : public baseObj
{
public:
	enum moveSubState
	{
		MOVETOTARGET = 0,
		RETREAT,
		MOVETORECHARGE,
	};

		enum engineerState
	{
		IDLE = 0,
		RECHARGE,
		HEAL,
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
	Soldier2* repairTarget;
	MessageStruc* m_currentOrders;
	void repairState();
	void rechargeState();
	void idleState();
	TextureImage engiTex;


public:
	bool Init();
	stats m_stats;
	moveSubState m_moveState;
	engineerState m_state;
	CEngineer(void);
	~CEngineer(void);
	void SwitchState();
	void Update(float delta);
	void Draw();
	bool isAlive();
	void bulletHit(bullet* bul);
		

};

