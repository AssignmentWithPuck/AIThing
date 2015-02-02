#ifndef __MVC_MODEL_H__
#include "MVC_Model.h"
#endif
#ifndef __MVC_TIME_H__
#include "MVCtime.h"
#endif

#ifndef __VECTOR_H__
#include <vector>
#define __VECTOR_H__
#endif

using namespace std;

MVC_Model::MVC_Model(void)
{
	m_timer=MVCTime::GetInstance();
}

MVC_Model::~MVC_Model(void)
{
	delete m_commander;
}

bool MVC_Model::Init(float fpsLimit)
{
	m_timer->Init(true,int(fpsLimit));
	m_commander=new Commander;
	m_commander->side=0;
	m_commander->pos=Vector3D(150,400,0);
	m_commander2=new Commander;
	m_commander2->side=1;
	m_commander2->pos=Vector3D(800,400,0);
	m_commander->otherCommander=m_commander2;
	m_commander2->otherCommander=m_commander;
	m_commander->spawn=m_commander2->spawn=ObjHandle::GetInstance();
	m_commander->fixedAttackPos[0]=m_commander2->fixedAttackPos[0]=Vector3D(500,50,0);
	m_commander->fixedAttackPos[1]=m_commander2->fixedAttackPos[1]=Vector3D(500,300,0);
	m_commander->fixedAttackPos[2]=m_commander2->fixedAttackPos[2]=Vector3D(500,550,0);
	m_commander->fixedAttackPos[3]=m_commander2->fixedAttackPos[3]=Vector3D(500,800,0);
	m_commander->fixedAttackPos[4]=m_commander2->pos;
	m_commander2->fixedAttackPos[4]=m_commander->pos;
	m_commander->Init();
	m_commander2->Init();
	return true;
}

bool MVC_Model::InitPhase2(void)
{
	LoadTGA(&background,"grassland.tga");
	pause=false;
	timeHolder=m_timer->PushNewTime(3000);
	return true;
}

// Update the model
void MVC_Model::Update(void)
{
	m_timer->UpdateTime();
	if(!pause)
	{
		if(m_timer->TestFramerate())//update control
		{
			ObjHandle::GetInstance()->Update(m_timer->GetDelta());

			m_commander->Update();
			m_commander2->Update();
		}
	}
	else
	{
		if(m_timer->TestTime(timeHolder))
		{
			pause=false;
			ObjHandle::Drop();
		}
	}
}

void MVC_Model::ProcMessages()
{
	//for(vector<MessageStruc*>::iterator it=MessageBoardGlobal::GetInstance(side)->messageList.begin();it!=MessageBoardGlobal::GetInstance()->messageList.end();++it)
	//{//goes through all messages
	//	if((*it)->m_Type==REPORT)
	//	{
	//		if((*it)->m_Content==FINDINGSQUAD&&(*it)->active)
	//		{
	//			for(vector<MessageStruc*>::iterator it2=MessageBoardGlobal::GetInstance()->messageList.begin();it2!=MessageBoardGlobal::GetInstance()->messageList.end();++it2)
	//			{
	//				if((*it2)->active&&(*it2)->m_Content==RECRUITING&&!(*it2)->taken)
	//				{
	//					baseObj* temp = (baseObj*)(*it)->info;
	//					theSquad.SMember.push_back(temp);
	//					temp->squadBoard=&theSquad;
	//					temp->m_objType=SOLDIER_TYPE;
	//					(*it)->active=false;
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}
}