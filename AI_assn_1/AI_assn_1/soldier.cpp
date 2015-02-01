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
	m_objType=SOLDIER_TYPE;
	pos=Vector3D(100,100,0);
	scale=Vector3D(50,50,50);
	m_state=MOVE;
	m_moveState=MOVEFORWARD;
	m_stats.ammo=7;
	m_stats.hp=10;
	m_stats.reloading=false;
	m_stats.timeRef=MVCTime::GetInstance()->PushNewTime(2000);
	m_stats.bulletRef=MVCTime::GetInstance()->PushNewTime(100);
	underFireLimit=MVCTime::GetInstance()->PushNewTime(200);
	m_stats.proning=false;
	m_underFire=false;
	atkTarget=NULL;
	m_spdMult=1.0;
	spd=Vector3D(0,1.0,0);
	if(!LoadTGA(&soldierTex,"Soldier.tga"))
	{
		cout<<"error loading textures\n";
	}
}

SoldierSMControl::~SoldierSMControl()
{
}

void SoldierSMControl::SwitchState()
{
	if(IsAlive())
	{
		if(MVCTime::GetInstance()->TestTime(underFireLimit))
		{
			int i=ObjHandle::GetInstance()->BulletsInProx(TURRET,pos,100.0f);
			UnderFire(i);
		}
		if(m_stats.reloading&&m_stats.ammo<7)
		{
			if(MVCTime::GetInstance()->TestTime(m_stats.bulletRef))
			{
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
				}
			break;
		case RELOAD:
			m_state=MOVE;
			m_moveState=MOVEFORWARD;
			m_stats.reloading=true;
			break;
		case MOVE:
			switch(m_moveState)
			{
			case MOVEFORWARD:
				if(!m_stats.reloading)
				{
					m_moveState=MOVETOSHOOT;
				}
				break;
			case MOVETOSHOOT:
				m_state=ATTACK;
				for(vector<baseObj*>::iterator it=ObjHandle::GetInstance()->m_AIList.begin();it!=ObjHandle::GetInstance()->m_AIList.end();++it)
				{
					if((*it)->m_objType!=SOLDIER_TYPE)
					{
						if((*it)->GetActive())
						{
							baseObj* temp=(*it);
							if(atkTarget!=NULL&&atkTarget->GetActive())
							{
								Vector3D tPos=atkTarget->GetPos()-pos;
								Vector3D nPos=temp->GetPos()-pos;
								if(tPos.GetMagnitudeSquare()>nPos.GetMagnitudeSquare())
								{
									atkTarget=temp;
								}
							}
							else
							{
								atkTarget=temp;
							}
						}
					}
				}
				break;
			case MOVETOCOVER:
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
				//m_state=MOVE;
				//m_moveState=MOVETOCOVER;
				m_state=MOVE;
				UnderFire(rand()%8+7);
			}
			else
			{
				m_state=DEAD;
				active=false;
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
		if(atkTarget!=NULL)
		{
			aim=atkTarget->GetPos()-pos;
		}
		else
		{
			aim=spd;
		}
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
		ObjHandle::GetInstance()->GetBullet(rand()%1000+1000,pos,aim,SOLDIER);
	}
}

void SoldierSMControl::Update(float delta)
{
	//there are stuff inside and the front is not active
	//working with assumtion that the stuff infront is older and so will be more likely to expire first
	if(atkTarget!=NULL)
	{
		spd=atkTarget->GetPos()-pos;
		spd.NormalizeVector3D();
	}
	while(m_dodgeList.size()>0&&!m_dodgeList.front()->GetActive())
	{
		m_dodgeList.pop_front();
	}
	SwitchState();
	
	switch(m_state)
	{
	case COVER:
		//stay in cover;
		break;
	case MOVE:
		switch(m_moveState)
		{
		case MOVEFORWARD:
			spd=(atkTarget->GetPos()-pos);
			spd.NormalizeVector3D();
			pos+=spd*m_spdMult*0.5;
			//pathfinding to enemy base;
			break;
		}
		break;
	case ATTACK:
		if(MVCTime::GetInstance()->TestTime(m_stats.bulletRef))
		{
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

//priority ~ number of bullets nearby
void SoldierSMControl::UnderFire(float priority)//state changer
{
	if(m_state==COVER)
	{
		//test if timer activates if so then set bool underfire to false
		if(MVCTime::GetInstance()->TestTime(m_stats.timeRef))
		{
			m_underFire=false;
			m_stats.proning=false;
			m_spdMult=1.0;
		}
		
	}
	else if(m_moveState==MOVETOCOVER&&m_state==MOVE)
	{
		if(MVCTime::GetInstance()->TestTime(m_stats.timeRef))
		{
			m_underFire=true;
			m_stats.proning=true;
			//rand number between 1s-2s + priority*0.15s( time taken to get back up )
			//reasoning if u are under heavy fire u will be scared and will take longer to be willing to
			//go back into battle
			int i=rand()%1000+1000+priority*150;
			m_state=COVER;
			MVCTime::GetInstance()->SetLimit(m_stats.timeRef,i);
			MVCTime::GetInstance()->ResetTime(m_stats.timeRef);
			//set limit and reset timer here
		}
		//test for timer and then if true change state to cover
	}
	else
	{
		if(priority>0)
		{
			cout<<"under fire\n";
		}
		int i=rand()%10;
		if(i<priority)
		{
			//test for going under cover condition~number of bullets in proximity
			//if success then move to cover else carry on
			i=rand()%1000+750-priority*100;//rand number between 0.75-1.75s - priority*0.1s (time taken to get down)
			if(i<0)
			{
				i=1;
			}
			m_spdMult=0.5;
			m_moveState=MOVETOCOVER;
			m_state=MOVE;
			MVCTime::GetInstance()->SetLimit(m_stats.timeRef,i);
			MVCTime::GetInstance()->ResetTime(m_stats.timeRef);
		}
		else
		{
			if(priority>0)
				cout<<"ignore enemy fire\n";
		}
	}
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
		else if(m_state==MOVE&&m_moveState==MOVETOCOVER)
		{
			glColor3f(1.0,0.0,1.0);
		}
		else
		{
			glColor3f(1.0f,1.0f,1.f);
		}
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D,soldierTex.texID);

		glPushMatrix(); 
			
			glTranslatef(pos.m_x,pos.m_y,pos.m_z);
			if(spd.m_y>0)
			{
				if(spd.m_x>0)
					glRotatef(atan(spd.m_y/spd.m_x)/3.142*180+90,0,0,1);
				else
					glRotatef(atan(spd.m_y/spd.m_x)/3.142*180+180,0,0,1);
			}
			else 
			{
				if(spd.m_x<0)
					glRotatef(270-atan(spd.m_y/spd.m_x)/3.142*180,0,0,1);
				else
					glRotatef(atan(spd.m_y/spd.m_x)/3.142*180,0,0,1);
			}
			glScalef(50,50,0);
			basicShape::drawSquare();
		glPopMatrix();

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor3f(1,1,1);
	}
}

void SoldierSMControl::bulletHit(bullet* bul)
{
	if(bul->GetActive())
	{
		for(deque<bullet*>::iterator it=m_dodgeList.begin();it!=m_dodgeList.end();++it)
		{
			if((*it)==bul)
			{
				return;
			}
		}
		if(m_state==MOVE&&m_moveState==MOVETOCOVER)
		{
			int i=rand()%2;
			switch(i)
			{
			case 0:
				m_dodgeList.push_back(bul);
				break;
			case 1:
				bul->SetActive(false);
				m_state=DAMAGE;
				break;
			}
		}
		else if(m_state!=COVER)//once in cover he will have 100% dodge chance
		{
			bul->SetActive(false);
			m_state=DAMAGE;
		}
	}
}