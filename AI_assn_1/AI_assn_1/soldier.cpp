#ifndef __SOLDIER_H__
#include "soldier.h"
#endif

SoldierSMControl::SoldierSMControl()
{
	m_state=MOVE;
	m_moveState=MOVEFORWARD;
	m_stats.ammo=4;
	m_stats.hp=8;
}

void SoldierSMControl::switchState()
{
	//test if timer activates if so then set bool underfire to true
	if(m_underFire&&m_state!=COVER&&m_state!=DAMAGE&&m_state!=DEAD)//under fire and not cover or damaged and not dead
	{
		m_moveState=MOVETOCOVER;
		m_state=MOVE;
	}
	switch(m_state)
	{
	case ATTACK:
		if(m_stats.ammo<=0)
		{
			m_state=RELOAD;
		}
		break;
	case RELOAD:
		m_state=MOVE;
		m_moveState=MOVEFORWARD;
		break;
	case MOVE:
		switch(m_moveState)
		{
		case MOVEFORWARD:
			if(m_stats.ammo>0)
			{
				m_moveState=MOVETOSHOOT;
			}
			break;
		case MOVETOSHOOT:
			//if statement to check is there is a clear shot
			m_state=ATTACK;
			break;
		case MOVETOCOVER:
			//if statement to check if in cover;
			m_state=COVER;
			break;
		}
		break;
	case COVER:
		if(!m_underFire)
		{
			if(m_stats.ammo>0)
			{
				m_state=MOVE;
				m_moveState=MOVETOSHOOT;
			}
			else
			{
				m_state=RELOAD;
			}
		}
		break;
	case DAMAGE:
		m_stats.hp-=1;
		if(m_stats.hp>0)
		{
			m_state=MOVE;
			m_moveState=MOVETOCOVER;
		}
		else
		{
			m_state=DEAD;
		}
		break;
	}
}

void SoldierSMControl::update()
{
	switch(m_state)
	{
	case COVER:
		//stay in cover;
		break;
	case MOVE:
		switch(m_moveState)
		{
		case MOVETOSHOOT:
			//path finding to area that can shoot;
			break;
		case MOVETOCOVER:
			//path finding to nearest cover;
			break;
		case MOVEFORWARD:
			//pathfinding to enemy base;
			break;
		}
		break;
	case ATTACK:
		 //find possible turrets that can be shot at\
		 //if there are multiple turrets select 1 at random
		 //do test for bullet
		 //spawn bullet and shoot in general direction
		 //set timer to next next bullet
		break;
	}
}

bool SoldierSMControl::underFire()
{
	//if under fire reset the timer
	//else if timer expires 
	return false;
}

bool SoldierSMControl::isAlive()
{
	if(m_state==DEAD)
	{
		return true;
	}
	else
		return false;
}

void SoldierSMControl::draw()
{
	//opengl stuff here
}