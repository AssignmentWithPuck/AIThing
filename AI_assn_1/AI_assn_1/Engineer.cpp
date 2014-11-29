#include "Engineer.h"

#include "Turret.h""

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

CTurret tur;

CEngineer::CEngineer(void)
{
	pos=Vector3D(300,400,0);
	scale=Vector3D(50,50,50);
	m_state = IDLE;
	m_moveState = MOVETORECHARGE;
	m_stats.charge = 3;
	m_stats.hp = 8;
}


CEngineer::~CEngineer(void)
{
}

void CEngineer::SwitchState()
{
	switch (m_state){
	case IDLE:
		if(tur.reqRepair){
			m_state = REPAIR;
		}
		break;
	case REPAIR:
		if (m_stats.charge <= 0){
			m_state = RECHARGE;
		}
		break;
	case DAMAGED:
		--m_stats.hp;
		if (m_stats.hp > 0){
			if (m_stats.charge > 0 && !tur.reqRepair){
				m_state = IDLE;
			}
			if(m_stats.charge > 0 && tur.reqRepair){
				m_state = REPAIR;
			}
			if(m_stats.charge <= 0){
				m_state = RECHARGE;
			}
		}
		else
		{
			m_state = DEAD;
		}
	}
}

void CEngineer::Update(float delta)
{
	switch(m_state)
	{
	case REPAIR:
		if(MVCTime::GetInstance()->TestTime(m_stats.timeRef))
		{
			m_stats.charge--;
			repairState();
		}
		break;
	}
}

void CEngineer::repairState(){
	while (tur.reqRepair && m_stats.charge > 0){
			tur.m_stats.hp++;
		}
}


void CEngineer::Draw(){
	

		glPushMatrix(); 
			glColor3f(0.0,1.0,0.0);
			glTranslatef(pos.m_x,pos.m_y,pos.m_z);
			glScalef(50,50,0);
			basicShape::drawSquare();
		glPopMatrix();

	
}