#pragma once
#include "vector3D.h"
#include "AI_header.h"
#include "Turret.h"
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
	CTurret* repairTarget;
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

