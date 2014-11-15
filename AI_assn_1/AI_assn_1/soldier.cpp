#ifndef __SOLDIER_H__
#include "soldier.h"
#endif

#ifndef __BASIC_H__
#include "basicShape.h"
#endif

#ifndef __MVCTIME_H__
#include "MVCtime.h"
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
	pos=Vector3D(100,100,0);
	m_state=MOVE;
	m_moveState=MOVEFORWARD;
	m_stats.ammo=4;
	m_stats.hp=8;
	m_stats.reloading=false;
	m_stats.timeRef=MVCTime::GetInstance()->PushNewTime(2000);
	m_stats.bulletRef=MVCTime::GetInstance()->PushNewTime(100);
	m_stats.proning=false;
	m_underFire=false;
	m_spdMult=1.0;
	spd=Vector3D(0,1.0,0);
}

SoldierSMControl::~SoldierSMControl()
{

}

void SoldierSMControl::SwitchState()
{
	if(IsAlive())
	{
		//test if timer activates if so then set bool underfire to true
		if(MVCTime::GetInstance()->TestTime(m_stats.timeRef))
		{
			m_underFire=false;
			m_stats.proning=false;
			m_spdMult=1.0;
		}
		if(m_underFire&&(m_state!=COVER))//under fire and not cover or damaged
		{
			cout<<"under fire\n";
			cout<<"swapped to state move\n";
			cout<<"	swapped to substate move to cover\n\n";
			m_moveState=MOVETOCOVER;
			m_state=MOVE;
			m_stats.proning=true;
			m_spdMult=0.5;
		}
		if(m_stats.reloading&&m_stats.ammo<4)
		{
			if(MVCTime::GetInstance()->TestTime(m_stats.bulletRef))
			{
				cout<<"adding ammo\n";
				m_stats.ammo++;
			}
		}
		else
		{
			m_stats.reloading=false;
			MVCTime::GetInstance()->SetLimit(m_stats.bulletRef,300);
		}
		switch(m_state)
		{
		case ATTACK:
				if(m_stats.ammo<=0)
				{
					MVCTime::GetInstance()->SetLimit(m_stats.bulletRef,1000);
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
					//cout<<"moving forward\n";
					pos+=spd*m_spdMult;
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
	}
}

void SoldierSMControl::Shoot()
{
	if(IsAlive())
	{
		Vector3D aim;
		aim=spd;
		aim.NormalizeVector3D();
		float recoil =rand()%50-25;
		recoil=recoil/180*3.142;
		Vector3D side;
		if(aim.m_x<0.0001&&aim.m_x>-0.0001)
		{
			side.Set(aim.m_y,0,0);
		}
		else if(aim.m_y<0.0001&&aim.m_y>-0.0001)
		{
			side.Set(0,aim.m_x,0);
		}
		else
		{
			side.Set(-aim.m_x,aim.m_y,aim.m_z);
		}
		side=side*(tan(recoil));
		aim=(side+aim);
		aim.NormalizeVector3D();
		aim=aim*300;
		ObjHandle::GetInstance()->GetBullet(rand()%1000+500,pos,aim,SOLDIER);
	}
}

void SoldierSMControl::Update(float delta)
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
		
		if(MVCTime::GetInstance()->TestTime(m_stats.bulletRef))
		{
			cout<<"shoot\n";
			m_stats.ammo--;
			Shoot();
		}
		 //find possible turrets that can be shot at\
		 //if there are multiple turrets select 1 at random
		 //do test for bullet
		 //spawn bullet and shoot in general direction
		 //set timer to next next bullet
		break;
	}
}

void SoldierSMControl::UnderFire()
{
	m_underFire=true;
	MVCTime::GetInstance()->ResetTime(m_stats.timeRef);
}

bool SoldierSMControl::IsAlive()
{
	if(m_state==DEAD)
	{
		return false;
	}
	else
		return true;
}

void SoldierSMControl::Draw()
{
	if(IsAlive())
	{
		if(m_stats.proning==true)
		{
			glColor3f(0.0,0.0,1.0);
		}
		else
		{
			glColor3f(1.0,1.0,1.0);
		}
		glPushMatrix();
			glTranslatef(pos.m_x,pos.m_y,pos.m_z);
			glScalef(50,50,0);
			basicShape::drawSquare();
		glPopMatrix();
	}

}