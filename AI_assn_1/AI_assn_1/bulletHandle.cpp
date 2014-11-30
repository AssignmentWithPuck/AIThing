#ifndef __AI_HEADER_H__
#include "AI_header.h"
#endif

#ifndef __VECTOR_3D_H__
#include "vector3D.h"
#endif

#ifndef __VECTOR_H__
#include <vector>
#endif

#ifndef __BASIC_H__
#include "basicShape.h"
#endif

#ifndef __SOLDIER_H__
#include "soldier.h"
#endif

#ifndef __ENGINEER_H__
#include "Engineer.h"
#endif

#ifndef __TURRET_H__
#include "Turret.h"
#endif

#ifndef __IOSTREAM_H__
#include <iostream>
#define __IOSTREAM_H__
#endif


using namespace std;

void baseObj::SetPos(Vector3D pos)
{
	this->pos=pos;
}

void baseObj::SetSpd(Vector3D spd)
{
	this->spd=spd;
}

Vector3D baseObj::GetPos()
{
	return pos;
}

Vector3D baseObj::GetSpd()
{
	return spd;
}

bool baseObj::GetActive()
{
	return active;
}

void baseObj::SetActive(bool nActive)
{
	active=nActive;
}

baseObj::~baseObj()
{

}

baseObj::baseObj()
{
}

void bullet::Draw()
{
	if(active)
	{
		glPushMatrix();
			glTranslatef(pos.m_x,pos.m_y,pos.m_z);
			glScalef(15,15,0);
			basicShape::drawSquare();
		glPopMatrix();
	}
}

void bullet::Update(float delta)
{
	if(active)
	{
		pos=pos+spd*delta;
		lifeLeft-=1/delta;
		if(lifeLeft<=0)
		{
			active=false;
		}
	}
}

bullet::bullet()
{
	active=true;
	lifeLeft=0;
	type=NONE;
}

bullet::~bullet()
{

}

bullet::bullet(int life,Vector3D pos,Vector3D spd,source type)
{
	lifeLeft=life;
	this->pos=pos;
	this->spd=spd;
	this->type=type;
	this->active=true;

}

void bullet::Set(int life,Vector3D pos,Vector3D spd,Vector3D scale,source type)
{
	lifeLeft=life;
	this->pos=pos;
	this->spd=spd;
	this->type=type;
	this->scale=scale;
	this->active=true;
}

bullet* ObjHandle::GetBullet(int life,Vector3D pos,Vector3D spd,source type)
{
	m_bulletList.size();
	for(vector<bullet*>::iterator it=m_bulletList.begin();it!=m_bulletList.end();++it)
	{
		if(!(*it)->GetActive())
		{
			//swap to use set
			(*it)->Set(life,pos,spd,Vector3D(1,1,1),type);
			/*(*it)->lifeLeft=life;
			(*it)->pos=pos;
			(*it)->spd=spd;
			(*it)->type=type;
			(*it)->active=true;*/
			return *it;
		}
	}
	bullet*temp=new bullet(life,pos,spd,type);
	m_bulletList.push_back(temp);
	if(accessing)
	{
		addedStuff=true;
		m_backLog.push_back(temp);
	}
	else
		m_objList.push_back(temp);
	return temp;
}

void ObjHandle::PushObj(objType type,Vector3D pos)
{
	switch(type)
	{
	case SOLDIER_TYPE:
		{
			SoldierSMControl* temp=new SoldierSMControl;
			temp->SetPos(pos);
			m_AIList.push_back(temp);
			if(accessing)
			{
				addedStuff=true;
				m_backLog.push_back(temp);
			}
			else
				m_objList.push_back(temp);
		}
		break;
	case ENGINEER_TYPE:
		{
			CEngineer* temp=new CEngineer;
			temp->SetPos(pos);
			m_AIList.push_back(temp);
			if(accessing)
			{
				addedStuff=true;
				m_backLog.push_back(temp);
			}
			else
				m_objList.push_back(temp);
		}
		break;
	case TURRET_TYPE:
		{
			CTurret* temp=new CTurret;
			temp->SetPos(pos);
			m_AIList.push_back(temp);
			if(accessing)
			{
				addedStuff=true;
				m_backLog.push_back(temp);
			}
			else
				m_objList.push_back(temp);
		}
		break;
	}
}

void ObjHandle::Update(float delta)
{
	accessing=true;
	for(vector<baseObj*>::iterator it=m_objList.begin();it!=m_objList.end();++it)
	{
		if((*it)->GetActive())
		{
			(*it)->Update(delta);
		}
	}
	accessing=false;
	if(addedStuff)
	{
		for(vector<baseObj*>::iterator it=m_backLog.begin();it!=m_backLog.end();++it)
		{
			m_objList.push_back(*it);
		}
		while(m_backLog.size()>0)
		{
			m_backLog.pop_back();
		}
		addedStuff=false;
	}
	//test collision)
	for(vector<baseObj*>::iterator it=m_AIList.begin();it!=m_AIList.end();++it)
	{
		if((*it)->GetActive())
		{
			for(vector<bullet*>::iterator it2=m_bulletList.begin();it2!=m_bulletList.end();++it2)
			{
				if((*it2)->GetActive())
				{
					if((*it)->m_objType==SOLDIER_TYPE&&(*it2)->type!=SOLDIER)//the object is a soldier the bullet is not from a soldier
					{
						if(((*it)->GetPos()-(*it2)->GetPos()).GetMagnitudeSquare()<1055)
						{
							SoldierSMControl* temp=(SoldierSMControl*)(*it);
							temp->bulletHit((*it2));
							std::cout<<"soldier Damaged\n";
						}
					}
					else if((*it2)->type==SOLDIER)//thing for Engineer/turret
					{
						if(((*it)->GetPos()-(*it2)->GetPos()).GetMagnitudeSquare()<1055)
						{
							switch((*it)->m_objType)
							{
							case TURRET_TYPE:
								{
									CTurret* temp=(CTurret*)(*it);
									temp->bulletHit((*it2));
								}
								break;
							case ENGINEER_TYPE:
								{
									CEngineer* temp=(CEngineer*)(*it);
									temp->bulletHit((*it2));
								}
							}
						}
					}
				}
			}
		}
	}
}

void ObjHandle::Draw(void)
{
	accessing=true;
	for(vector<baseObj*>::iterator it=m_objList.begin();it!=m_objList.end();++it)
	{
		if((*it)->GetActive())
		{
		//draw here;
			(*it)->Draw();
		}
	}
	accessing =false;
}

ObjHandle* ObjHandle::s_instance=NULL;

ObjHandle* ObjHandle::GetInstance()
{
	if(s_instance==NULL)
	{
		s_instance=new ObjHandle;
	}
	return s_instance;
}

void ObjHandle::Drop()
{
	if(s_instance!=NULL)
	{
		while(s_instance->m_objList.size()>0)
		{
			delete (s_instance->m_objList.back());
			s_instance->m_objList.pop_back();
		}
		delete s_instance;
		s_instance=NULL;
	}
}

ObjHandle::~ObjHandle()
{
}

ObjHandle::ObjHandle()
{
	addedStuff=false;
	accessing=false;
}


int ObjHandle::BulletsInProx(source src,Vector3D pos,float dist)
{
	int counter=0;
	for(vector<bullet*>::iterator it=m_bulletList.begin();it!=m_bulletList.end();++it)
	{
		bullet* temp= *it;
		if(temp->GetActive())
		{
			if(temp->type==src)
			{
				if((pos-temp->GetPos()).GetMagnitudeSquare()<dist*dist)
				{
					counter++;
				}
			}
		}
	}
	return counter;
}