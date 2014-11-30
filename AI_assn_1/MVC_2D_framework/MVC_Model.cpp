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
	ObjHandle::GetInstance()->PushObj(SOLDIER_TYPE,Vector3D(500,100,0));
	ObjHandle::GetInstance()->PushObj(ENGINEER_TYPE,Vector3D(600,700,0));
	ObjHandle::GetInstance()->PushObj(TURRET_TYPE,Vector3D(600,600,0));
	LoadTGA(&background,"grassland.tga");
	pause=false;
	timeHolder=m_timer->PushNewTime(3000);
	solVictory=0;
	engiVictory=0;
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

			for(vector<baseObj*>::iterator it=ObjHandle::GetInstance()->m_AIList.begin();it!=ObjHandle::GetInstance()->m_AIList.end();++it)
			{
				if(!(*it)->GetActive())
				{
					if((*it)->m_objType==SOLDIER_TYPE||(*it)->m_objType==TURRET_TYPE)
					{
						pause=true;
						m_timer->ResetTime(timeHolder);
						if((*it)->m_objType==SOLDIER_TYPE)
						{
							engiVictory++;
						}
						if((*it)->m_objType==TURRET_TYPE)
						{
							solVictory++;
						}
					}
				}
			}
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
			ObjHandle::GetInstance()->PushObj(SOLDIER_TYPE,Vector3D(500,100,0));
			ObjHandle::GetInstance()->PushObj(ENGINEER_TYPE,Vector3D(600,700,0));
			ObjHandle::GetInstance()->PushObj(TURRET_TYPE,Vector3D(600,600,0));
		}
	}
}

