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
}

bool MVC_Model::Init(float fpsLimit)
{
	m_timer->Init(true,int(fpsLimit));
	return true;
}

bool MVC_Model::InitPhase2(void)
{
	LoadTGA(&background,"grassland.tga");
	pause=false;
	timeHolder=m_timer->PushNewTime(3000);
	theSquad.Update();
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
			ProcMessages();
			ObjHandle::GetInstance()->Update(m_timer->GetDelta());

			//thing.SwitchState();
			//thing2.SwitchState();
			//eng.SwitchState();

			//thing.Update(m_timer->GetDelta());
			//thing2.Update(m_timer->GetDelta());
			//eng.Update(m_timer->GetDelta());
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
	for(vector<MessageStruc*>::iterator it=MessageBoardGlobal::GetInstance()->messageList.begin();it!=MessageBoardGlobal::GetInstance()->messageList.end();++it)
	{//goes through all messages
		if((*it)->m_Type==REPORT)
		{
			if((*it)->m_Content==FINDINGSQUAD&&(*it)->active)
			{
				for(vector<MessageStruc*>::iterator it2=MessageBoardGlobal::GetInstance()->messageList.begin();it2!=MessageBoardGlobal::GetInstance()->messageList.end();++it2)
				{
					if((*it2)->active&&(*it2)->m_Content==RECRUITING&&!(*it2)->taken)
					{
						baseObj* temp = (baseObj*)(*it)->info;
						theSquad.SMember.push_back(temp);
						temp->squadBoard=&theSquad;
						temp->m_objType=SOLDIER_TYPE;
						(*it)->active=false;
						break;
					}
				}
			}
		}
	}
}