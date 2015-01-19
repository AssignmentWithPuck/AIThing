#include "Turret.h"


#ifndef __MVCTIME_H__
#include "MVCtime.h"
#endif

#ifndef __BASIC_H__
#include "basicShape.h"
#endif

#include <iostream>

using namespace std;

CTurret::CTurret(void)
{
	pos = Vector3D(200,400,0);
	m_state = ATTACK;
	m_stats.ammo = 4;
	m_stats.hp = 5;
	reqRepair = false;
	m_stats.timeRef=MVCTime::GetInstance()->PushNewTime(5000);
	m_stats.bulletRef=MVCTime::GetInstance()->PushNewTime(150);
	m_objType=TURRET_TYPE;
	atkTarget=NULL;
	atkDir=(0,-1,0);
	if(!LoadTGA(&turretTex,"turret.tga"))
	{
		cout<<"error loading turret textures\n";
	}
	for(vector<baseObj*>::iterator it=ObjHandle::GetInstance()->m_AIList.begin();it!=ObjHandle::GetInstance()->m_AIList.end();++it)
	{
		if((*it)->m_objType==SOLDIER_TYPE)
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
}


CTurret::~CTurret(void)
{
}

bool CTurret::Init()
{
	return true;
}

void CTurret::SwitchState()
{
	if(m_stats.hp >= 5){
		reqRepair = false;
	}

	switch (m_state){

	case RELOAD:
		if(MVCTime::GetInstance()->TestTime(m_stats.bulletRef))
		{
			m_stats.ammo++;
		}
		if (m_stats.ammo >= 4)
		{
			for(vector<baseObj*>::iterator it=ObjHandle::GetInstance()->m_AIList.begin();it!=ObjHandle::GetInstance()->m_AIList.end();++it)
			{
				if((*it)->m_objType==SOLDIER_TYPE)
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
			if(atkTarget!=NULL)
			{
				atkDir=atkTarget->GetPos()-pos;
				atkDir.NormalizeVector3D();
			}
			MVCTime::GetInstance()->SetLimit(m_stats.bulletRef,150);
			m_state = ATTACK;
		}
		break;

	case ATTACK:
		if(m_stats.ammo<=0)
		{
			MVCTime::GetInstance()->SetLimit(m_stats.bulletRef,625);
			m_state=RELOAD;
		}
		break;

	case DAMAGED:
		--m_stats.hp;
		reqRepair = true;
		if(m_stats.hp > 0){
			m_state = ATTACK;
		}
		else {
		m_state = DEAD;	
		active=false;
		std::cout<<"turret died\n";
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

void CTurret::Shoot()
{
	if (isAlive()){
		Vector3D fire;
		if(atkTarget!=NULL)
		{
			fire=atkTarget->GetPos()-pos;
		}
		else
		{
			fire.Set(0,-1,0);
		}
		fire.NormalizeVector3D();
		float spread = rand()%50-25;
		spread = spread/180*3.142;
		Vector3D side;

		if (fire.m_x<0.0001 && fire.m_x>-0.0001){
			side.Set(fire.m_y,0 , 0);
		} else
			if (fire.m_y<0.0001 && fire.m_y>-0.0001){
				side.Set(0, fire.m_x, 0);
			}
			else side.Set(-fire.m_x, fire.m_y, fire.m_z);
		side = side*(tan(spread));
		fire += side;
		fire.NormalizeVector3D();
		fire *= 300;
		ObjHandle::GetInstance()->GetBullet(1500,pos,fire,TURRET);
				
	}
}

void CTurret::Draw()
{
	if(isAlive())
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D,turretTex.texID);

		glPushMatrix();
				glColor3f(1,1,1);
				glTranslatef(pos.m_x,pos.m_y,pos.m_z);
				if(atkDir.m_x==0)
					atkDir.m_x=0.001;
				if(atkDir.m_y>0)
				{
					if(atkDir.m_x>0)
						glRotatef(atan(atkDir.m_y/atkDir.m_x)/3.142*180,0,0,1);
					else
						glRotatef(atan(atkDir.m_y/atkDir.m_x)/3.142*180+90,0,0,1);
				}
				else 
				{
					if(atkDir.m_x<0)
						glRotatef(atan(atkDir.m_y/atkDir.m_x)/3.142*180+180,0,0,1);
					else
						glRotatef(270-atan(atkDir.m_y/atkDir.m_x)/3.142*180,0,0,1);
				}
				//glRotatef(atan(spd.m_y/spd.m_x)/3.142*180+90,0,0,1);
				glScalef(50,50,0);
				basicShape::drawSquare();
		glPopMatrix();

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor3f(1,1,1);
	}
}

void CTurret::Update(float delta)
{
	if(atkTarget!=NULL)
	{
		atkDir=atkTarget->GetPos()-pos;
		atkDir.NormalizeVector3D();
	}
	SwitchState();
	switch(m_state)
	{
	case ATTACK:
		if(MVCTime::GetInstance()->TestTime(m_stats.bulletRef))
		{
			m_stats.ammo--;
			Shoot();
		}
		break;
	}
}

void CTurret::bulletHit(bullet* bul)
{
	bul->SetActive(false);
	m_state=DAMAGED;
}