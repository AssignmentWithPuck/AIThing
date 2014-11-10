#pragma once
#include "vector3D.h"
class CTurret
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
	Vector3D pos;
	int hp;
	int ammo;
	int timeRef;
}stats;

private:
	turretState m_state;
	stats m_stats;
	void attackState();
	void reloadState();
public:
	CTurret(void);
	~CTurret(void);
	void switchState();
	void update();
	void draw();
	bool isDamaged();
	bool isAlive();
	bool reqRepair;
};

