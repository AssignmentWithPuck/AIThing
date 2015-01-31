#pragma once
#include "vector3D.h"
#include "AI_header.h"
#include  "texture.h"
#include "MessageBoard.h"

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
	Vector3D atkDir;
	void attackState();
	void reloadState();
	void Shoot();
	baseObj* atkTarget;
	MessageStruc* m_currentOrders;


public:
	bool Init();
	turretState m_state;
	TextureImage turretTex;
	stats m_stats;
	CTurret(void);
	~CTurret(void);
	void SwitchState();
	void Update(float delta);
	void bulletHit(bullet* bul);
	void Draw();
	bool isDamaged();
	bool isAlive();

};

