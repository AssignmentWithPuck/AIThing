#include "Turret.h"


CTurret::CTurret(void)
{
	m_state = ATTACK;
	m_stats.ammo = 4;
	m_stats.hp = 5;
	reqRepair = false;
}


CTurret::~CTurret(void)
{
}

void CTurret::switchState()
{
	if(m_stats.ammo <= 0 && m_state != DEAD){
		m_state = RELOAD;
	}

	if(m_stats.hp >= 5){
		reqRepair = false;
	}

	switch (m_state){

	case RELOAD:
		m_stats.ammo = 4;
		m_state = ATTACK;
		break;

	case DAMAGED:
		--m_stats.hp;
		reqRepair = true;
		if(m_stats.hp <= 0){
			m_state = ATTACK;
		}
		else {
		m_state = DEAD;	
		};
	}
	 
}

bool CTurret::isAlive()
{
	if (m_state == DEAD)
	{
		return false;
	}
	else
		return true;
}