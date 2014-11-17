#include "Turret.h"


#ifndef __MVCTIME_H__
#include "MVCtime.h"
#endif

#ifndef __BASIC_H__
#include "basicShape.h"
#endif

CTurret::CTurret(void)
{
	pos = Vector3D(200,400,0);
	m_state = ATTACK;
	m_stats.ammo = 4;
	m_stats.hp = 5;
	reqRepair = false;
	m_stats.timeRef=MVCTime::GetInstance()->PushNewTime(5000);
	m_stats.bulletRef=MVCTime::GetInstance()->PushNewTime(100);
}


CTurret::~CTurret(void)
{
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
		MVCTime::GetInstance()->SetLimit(m_stats.bulletRef,100);
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

void CTurret::Shoot()
{
	if (isAlive()){
		Vector3D fire;
		fire.Set(0, -1 ,0);
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
	glPushMatrix();
			glColor3f(1,0,0);
			glTranslatef(pos.m_x,pos.m_y,pos.m_z);
			glScalef(50,50,0);
			basicShape::drawSquare();
	glPopMatrix();
}

void CTurret::Update(float delta)
{
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
