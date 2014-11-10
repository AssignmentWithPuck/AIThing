#ifndef __SOLDIER_H__
#define __SOLDIER_H__

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

typedef struct
{
	float x,y;
	int hp;
	int ammo;
	int timeRef;
}stats;

class SoldierSMControl
{
private:
	soldierState m_state;
	moveSubState m_moveState;
	stats m_stats;
	bool m_underFire;
	void attackState();
	void moveState();
	void coverState();
public:
	SoldierSMControl();
	~SoldierSMControl();
	void switchState();//choosing which state to use
	void update();
	void draw();//just draw
	bool underFire();
	bool isAlive();
};

#endif