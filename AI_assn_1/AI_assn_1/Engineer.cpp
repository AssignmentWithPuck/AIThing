#include "Engineer.h"

#ifndef __VECTOR_H__
#include <vector>
#endif;

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


CEngineer::CEngineer(void)
{
	pos=Vector3D(300,400,0);
	scale=Vector3D(50,50,50);
	m_state = IDLE;
	m_moveState = MOVETORECHARGE;
	m_stats.charge = 3;
	m_stats.hp = 8;
	m_objType=ENGINEER_TYPE;
	repairTarget=NULL;
	m_stats.timeRef=MVCTime::GetInstance()->PushNewTime(1300);
	LoadTGA(&engiTex,"engineer.tga");
}


CEngineer::~CEngineer(void)
{
}

void CEngineer::SwitchState()
{
	switch (m_state){
	case IDLE:
		for(vector<baseObj*>::iterator it=ObjHandle::GetInstance()->m_AIList.begin();it!=ObjHandle::GetInstance()->m_AIList.end();++it)
		{
			if((*it)->m_objType==SOLDIER_TYPE)
			{
				Soldier2* temp=(Soldier2*)(*it);
				if(temp->reqRepair)
				{
					if(repairTarget!=NULL)
					{
						Vector3D tPos=repairTarget->GetPos()-pos;
						Vector3D nPos=temp->GetPos()-pos;
						if(tPos.GetMagnitudeSquare()>nPos.GetMagnitudeSquare())
						{
							repairTarget=temp;
						}
					}
					else
					{
						repairTarget=temp;
					}
				}
			}
		}
		if(repairTarget!=NULL && HEAL)
			m_state = HEAL;
		break;
	case HEAL:
		if (m_stats.charge <= 0){
			m_state = RECHARGE;
		}
		break;
	case DAMAGED:
		--m_stats.hp;
		if (m_stats.hp > 0){
			if (m_stats.charge > 0){
				if (repairTarget != NULL)
					m_state = HEAL;
				else
					m_state = IDLE;
			}
			else{
				m_state = RECHARGE;
			}
		}
		else
		{
			m_state = DEAD;
			active=false;
		}
		break;
	case RECHARGE:
		if(m_stats.charge>=3)
		{
			m_state=IDLE;
		}
	}
}

bool CEngineer::Init()
{
	return true;
}

void CEngineer::Update(float delta)
{
	if(squadBoard!=NULL)
			{
				if(m_currentOrders==NULL)
				{
					m_currentOrders=squadBoard->GetMessage1(m_objType,ORDER,this);
					if(m_currentOrders!=NULL)
						if(m_currentOrders->general==false)
						{
							m_currentOrders->taken=true;
						}
				}
				else
				{
					MessageStruc* temp=squadBoard->GetMessage1(m_objType,ORDER,this);
					if(temp!=NULL)
						if(temp->priority>m_currentOrders->priority)
						{
							if(temp->general==false)
								temp->taken=true;
							m_currentOrders->taken=false;
							m_currentOrders=temp;
						}
				}

				//switch state
			
				bool changedState=false;//to check if state has been changed

				if(!changedState)
				{
					if(m_currentOrders!=NULL)
					{
						switch(m_currentOrders->m_Content)
						{
						case HEAL:
							{
								//!!!!!!??????
							}
							break;
						}
					}
				}
	}

	SwitchState();
	switch(m_state)
	{
	case HEAL:
		if(MVCTime::GetInstance()->TestTime(m_stats.timeRef))
		{
			m_stats.charge--;
			repairState();

		}
		break;
	case RECHARGE:
		if(MVCTime::GetInstance()->TestTime(m_stats.timeRef))
		{
			m_stats.charge++;
		}
	}

}

void CEngineer::repairState(){
	if(repairTarget->m_stats.hp>=5)
	{
		repairTarget->reqRepair=false;
	}
	else
	{
		repairTarget->m_stats.hp++;
		cout<<"repairTarget HP: "<<repairTarget->m_stats.hp;
	}
}


void CEngineer::Draw(){
	

	glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D,engiTex.texID);

		glPushMatrix();
				glColor3f(1,1,1);
				glTranslatef(pos.m_x,pos.m_y,pos.m_z);
				glScalef(50,50,0);
				basicShape::drawSquare();
		glPopMatrix();

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor3f(1,1,1);

	
}

void CEngineer::bulletHit(bullet* bul)
{
	bul->SetActive(false);
	m_state=DAMAGED;
}