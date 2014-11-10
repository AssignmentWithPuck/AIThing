#ifndef __SOLDIER_H__
#include "soldier.h"
#endif

#ifndef __BASIC_H__
#include "basicShape.h"
#endif

#ifndef __AI_HEADER_H__
#include "AI_header.h"
#endif

#ifndef __IOSTREAM_H__
#include <iostream>
#define __IOSTREAM_H__
#endif

using namespace std;

SoldierSMControl::SoldierSMControl()
{
	m_stats.pos=Vector3D(100,100,0);
	m_state=MOVE;
	m_moveState=MOVEFORWARD;
	m_stats.ammo=4;
	m_stats.hp=8;
	m_stats.reloading=false;
	m_underFire=false;
}

SoldierSMControl::~SoldierSMControl()
{

}

void SoldierSMControl::switchState()
{
	//test if timer activates if so then set bool underfire to true
	//if(underFire()&&m_state!=COVER&&m_state!=DAMAGE&&m_state!=DEAD)//under fire and not cover or damaged and not dead
	if(m_underFire&&(m_state!=COVER))
	{
		cout<<"under fire\n";
		cout<<"swapped to state move\n";
		cout<<"	swapped to substate move to cover\n\n";
		m_moveState=MOVETOCOVER;
		m_state=MOVE;
	}
	if(m_stats.reloading&&m_stats.ammo<4)
	{
		cout<<"adding ammo\n";
		m_stats.ammo++;
	}
	else
	{
		m_stats.reloading=false;
	}
	switch(m_state)
	{
	case ATTACK:
		cout<<"shoot\n";
		m_stats.ammo--;
		if(m_stats.ammo<=0)
		{
			m_state=RELOAD;
			cout<<"out of ammo\n";
			cout<<"swapped to state reload\n\n";
		}
		break;
	case RELOAD:
		m_state=MOVE;
		m_moveState=MOVEFORWARD;
		m_stats.reloading=true;
		cout<<"reloading\n";
		cout<<"swapped to state move\n";
		cout<<"	swapped to substate move forward\n\n";
		break;
	case MOVE:
		switch(m_moveState)
		{
		case MOVEFORWARD:
			if(!m_stats.reloading)
			{
				m_moveState=MOVETOSHOOT;
				cout<<"have ammo\n";
				cout<<"	swapped to substate move to shoot\n\n";
			}
			else
			{
				cout<<"moving forward\n";
				m_stats.pos.m_y++;
				//pathing forward
			}
			break;
		case MOVETOSHOOT:
			//if statement to check is there is a clear shot
			cout<<"clear shot of enemy\n";
			cout<<"swapped to state attack\n\n";
			m_state=ATTACK;
			break;
		case MOVETOCOVER:
			//if statement to check if in cover;
			cout<<"in cover\n";
			cout<<"swapped to state cover\n\n";
			m_state=COVER;
			break;
		}
		break;
	case COVER:
		if(!m_underFire)
		{
			cout<<"no longer under fire\n";
			if(m_stats.ammo>0)
			{
				cout<<"have ammo\n";
				cout<<"swapped to state move\n";
				cout<<"	swapped to substate move to shoot\n\n";
				m_state=MOVE;
				m_moveState=MOVETOSHOOT;
			}
			else
			{
				cout<<"no ammo\n";
				cout<<"swapped to state reload\n\n";
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
	Sleep(1000);
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
	//testing for alive
	{
		glPushMatrix();
			glTranslatef(m_stats.pos.m_x,m_stats.pos.m_y,m_stats.pos.m_z);
			glScalef(100,100,0);
			basicShape::drawSquare();
		glPopMatrix();
	}

}