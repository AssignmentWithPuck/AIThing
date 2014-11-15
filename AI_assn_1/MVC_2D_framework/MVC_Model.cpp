#ifndef __MVC_MODEL_H__
#include "MVC_Model.h"
#endif
#ifndef __MVC_TIME_H__
#include "MVCtime.h"
#endif


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
	return true;
}

// Update the model
void MVC_Model::Update(void)
{
	m_timer->UpdateTime();
	if(m_timer->TestFramerate())//update control
	{
		
		ObjHandle::GetInstance()->Update(m_timer->GetDelta());
		thing.SwitchState();


		thing.Update(m_timer->GetDelta());
	}
}

