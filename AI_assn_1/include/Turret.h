#pragma once
#include "vector3D.h"
#include "AI_header.h"
class CTurret : public baseObj
{
public:
	enum turretState
{
	ATTACK = 0,
	RELOAD,
	DAMAGED,
	DEAD,
};

typedef struct
{
	int hp;
	int ammo;
	int timeRef;
	int bulletRef;
}stats;

private:
	turretState m_state;
	
	void attackState();
	void reloadState();
	void Shoot();
public:
	stats m_stats;
	CTurret(void);
	~CTurret(void);
	void SwitchState();
	void Update(float delta);
	void Draw();
	bool isDamaged();
	bool isAlive();
	bool reqRepair;
};

