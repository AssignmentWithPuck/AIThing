#include "Soldier2.h"

#ifndef __BASIC_H__
#include "basicShape.h"
#endif

#include "AI_header.h"

#include "MVCtime.h"

using namespace std;

Soldier2::Soldier2(void)
{
}


Soldier2::~Soldier2(void)
{
}

bool Soldier2::Init()
{
	m_objType=objType::NOSQUAD_TYPE;
	m_isLeader=false;
	m_currentOrders=NULL;
	reqSquad=false;
	reqRepair = false;
	squadBoard=NULL;
	m_state=OTHER;
	m_stats.hp=5;
	m_stats.ammo=7;
	m_stats.reloading=false;
	m_stats.timeRef=MVCTime::GetInstance()->PushNewTime(2000);
	m_stats.bulletRef=MVCTime::GetInstance()->PushNewTime(500);
	m_underFireLimit=MVCTime::GetInstance()->PushNewTime(200);
	m_stats.proning=false;
	m_underFire=false;
	recruiting=true;
	if(!LoadTGA(&soldierTex,"Soldier.tga"))
	{
		return false;
		//cout<<"error loading textures\n";
	}
	return true;
}

void Soldier2::Draw()
{
	if(IsAlive())
	{
		//if(m_stats.proning==true)
		//{
		//	glColor3f(0.0,0.0,1.0);
		//}
		//else if(m_state==MOVE&&m_moveState==MOVETOCOVER)
		//{
		//	glColor3f(1.0,0.0,1.0);
		//}
		//else
		if(side==0)
		{
			if(m_isLeader==true)
			{
				glColor3f(1,0,0);
			}
			else if(m_objType==SOLDIER_TYPE)
			{
				glColor3f(1,1,0);
			}
			else if(m_objType==NOSQUAD_TYPE)
			{
				glColor3f(0.0f,0.0f,1.0f);
			}
			else
			{
				glColor3f(0.0f,0.0f,0.f);
			}
		}
		else
		{
			if(m_isLeader==true)
			{
				glColor3f(0,1,0);
			}
			else if(m_objType==SOLDIER_TYPE)
			{
				glColor3f(1,1,1);
			}
			else if(m_objType==NOSQUAD_TYPE)
			{
				glColor3f(0.0f,0.0f,1.0f);
			}
			else
			{
				glColor3f(0.0f,0.0f,0.f);
			}

		}
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D,soldierTex.texID);

		glPushMatrix(); 
			
			glTranslatef(pos.m_x,pos.m_y,pos.m_z);
			/*if(spd.m_y>0)
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
			}*/
			glScalef(25,25,25);
			basicShape::drawSquare();
		glPopMatrix();

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor3f(1,1,1);
	}
}

bool Soldier2::IsAlive()
{
	return true;
}

void Soldier2::Update(float delta)
{
	if(active)
	{
		if(m_isLeader)
		{
			MessageBoardGlobal* messageBoardG=MessageBoardGlobal::GetInstance(side);
			if(squadBoard->SMember.size()==1)
			{
				recruiting =true;
			}
			if(squadBoard->SMember.size()>4||recruiting==false)
			{
				recruiting=false;
				if(m_currentOrders==NULL)
				{
					m_currentOrders=messageBoardG->GetMessage1(SQUAD_TYPE,ORDER,this);
					if(m_currentOrders!=NULL)
						if(m_currentOrders->general==false)
							m_currentOrders->taken=true;
				}
				else
				{
					MessageStruc* temp=messageBoardG->GetMessage1(SQUAD_TYPE,ORDER,this);
					if(temp!=NULL)
						if(temp->priority>m_currentOrders->priority)
						{
							temp->taken=true;
							m_currentOrders->taken=false;
							m_currentOrders=temp;
						}
				}
				if(m_currentOrders!=NULL)
				{
					for(vector<MessageStruc*>::iterator it=squadBoard->sentMessages.begin();it!=squadBoard->sentMessages.end();)
					{
						if((*it)->m_Content==RECRUITING)
						{
							(*it)->active=false;
							it=squadBoard->sentMessages.erase(it);
							break;
						}
						++it;
					}
				}
			}
			if(m_stats.hp<=0)
			{
				active=false;
				MessageStruc* temp=new MessageStruc;
				temp->active=true;
				temp->taken=false;
				temp->general=false;
				temp->m_Type=ORDER;
				temp->m_Content=LEADERDOWN;
				temp->target=objType::SOLDIER_TYPE;
				temp->priority=100;
				if(m_currentOrders!=NULL)
					m_currentOrders->taken=false;
				squadBoard->SendMessage1(temp);
				squadBoard->SLeader=NULL;
				for(vector<baseObj*>::iterator it=squadBoard->SMember.begin();it!=squadBoard->SMember.end();)
				{
					if((*it)==this)
					{
						(*it)->SetActive(false);
						squadBoard->SMember.erase(it);
						break;
					}
					++it;
				}
				//death code here;
			}
			else//still alive
			{
				//send orders to squad
				if(m_currentOrders!=NULL)//if not null
				{
					if(m_currentOrders->active==true)
					{
						if(!squadBoard->sentOrders)
						{
							switch(m_currentOrders->m_Content)
							{
							case ATTACKHERE:
								{
									MessageStruc* temp=new MessageStruc;
									temp->active=true;
									temp->taken=false;
									temp->general=true;
									temp->m_Type=ORDER;
									temp->m_Content=ATTACKHERE;
									temp->target=objType::SOLDIER_TYPE;
									temp->priority=0;
									temp->info=m_currentOrders->info;
									squadBoard->SendMessage1(temp);
								}
								break;
							case RETREAT:
								{
									//if case for if near base
									MessageStruc* temp=new MessageStruc;
									temp->active=true;
									temp->taken=false;
									temp->general=false;
									temp->m_Type=ORDER;
									temp->m_Content=RECRUITING;
									temp->priority=50;
									temp->info=squadBoard;
									messageBoardG->SendMessage1(temp);
									squadBoard->sentMessages.push_back(temp);
									//else 
									//carry on retreating
								}
							}
							squadBoard->sentOrders=true;
						}
						bool changedState=false;//to check if state has been changed
				
						SwitchState();
						//shooting part
				
						if(m_stats.reloading)
						{
							if(m_stats.ammo>5)
							{
								m_stats.reloading=0;
							}
							else
							{
								if(MVCTime::GetInstance()->TestTime(m_stats.bulletRef))
								{
									m_stats.ammo++;
								}
							}
						}
						if(ObjHandle::GetInstance()->EnemyinProx(side,pos,100)>1)
						{
							if(m_stats.ammo<=0&&!m_stats.reloading)
							{
								MVCTime::GetInstance()->SetLimit(m_stats.bulletRef,1000);
								m_stats.reloading=true;
							}
							else if(MVCTime::GetInstance()->TestTime(m_stats.bulletRef))
							{
								//m_stats.ammo--;
								atkTarget=ObjHandle::GetInstance()->EnemytoShoot(side,pos,100);
								Shoot();
							}
							changedState=true;
						}
						//do testing for normal states
						//will be a normal state machine
						//idle will be use orders

						//switch state(only test conditions)

						if(!changedState)
						{
							switch(m_currentOrders->m_Content)
							{
							case ATTACKHERE:
								{
									Vector3D atkPos=*(Vector3D*)(m_currentOrders->info);
									if((this->pos-atkPos).GetMagnitudeSquare()>100)//within 10 units
									{
										Vector3D dir=atkPos-this->pos;
										dir.NormalizeVector3D();
										this->pos=pos+dir*delta*50;
										//state = movestate
									}
									else
									{
										for(vector<MessageStruc*>::iterator it=squadBoard->messageList.begin();it!=squadBoard->messageList.end();)
										{
											if((*it)->m_Content==ATTACKHERE)
											{
												(*it)->active=false;
											}
											++it;
										}
										m_currentOrders->taken=false;
										m_currentOrders->priority=30+rand()%40;
										m_currentOrders=NULL;
										squadBoard->sentOrders=false;
									}
								}
								break;
							}
						}
					}
				}
				else //testing for near base
				{
					recruiting=true;
					float distSquare;
					if(side==0)
					{
						distSquare=(pos-Vector3D(150,400,0)).GetMagnitudeSquare();
					}
					else
						distSquare=(pos-Vector3D(800,400,0)).GetMagnitudeSquare();
					if(distSquare<40000)///sqrt of 40000 is 200
						//there are no orders and its near the base
					{
						bool send=true;
						for(vector<MessageStruc*>::iterator it=squadBoard->sentMessages.begin();it!=squadBoard->sentMessages.end();)
						{
							if((*it)->m_Content==RECRUITING)
							{
								send=false;
								break;
							}
						}
						for(vector<MessageStruc*>::iterator it=squadBoard->messageList.begin();it!=squadBoard->messageList.end();++it)
						{
							if((*it)->m_Content==RETREAT)
							{
								(*it)->active=false;
							}
						}
						if(send)
						{
							MessageStruc* temp=new MessageStruc;
							temp->active=true;
							temp->taken=false;
							temp->general=true;
							temp->m_Type=REPORT;
							temp->m_Content=RECRUITING;
							temp->target=objType::NOSQUAD_TYPE;
							temp->priority=50;
							temp->info=squadBoard;
							MessageBoardGlobal::GetInstance(side)->SendMessage1(temp);
							squadBoard->sentMessages.push_back(temp);
						}
					}
					else
					{
						bool sentRetreat=false;
						for(vector<MessageStruc*>::iterator it=squadBoard->messageList.begin();it!=squadBoard->messageList.end();++it)
						{
							if((*it)->m_Content==RETREAT)
							{
								sentRetreat=true;
							}
						}
						if(!sentRetreat)
						{
							MessageStruc* temp=new MessageStruc;
							temp->active=true;
							temp->taken=false;
							temp->general=true;
							temp->m_Type=ORDER;
							temp->m_Content=RETREAT;
							temp->target=objType::SOLDIER_TYPE;
							temp->priority=80;
							squadBoard->SendMessage1(temp);
							squadBoard->sentOrders=true;
						}
						Vector3D dir;
						if(side==0)
						{
							dir=Vector3D(150,400,0)-this->pos;
						}
						else
						{
							dir=Vector3D(800,400,0)-this->pos;
						}
							dir.NormalizeVector3D();
							this->pos=pos+dir*delta*50;
					}
				}
			}
		}
		else//not leader
		{
			if(!(m_stats.hp>0))
			{
				for(vector<baseObj*>::iterator it=squadBoard->SMember.begin();it!=squadBoard->SMember.end();)
				{
					if((*it)==this)
					{
						(*it)->SetActive(false);
						squadBoard->SMember.erase(it);
						break;
					}
					++it;
				}
			}
			else
			{
				//switch state
			
				bool changedState=false;//to check if state has been changed
				
				SwitchState();
				//shooting part
				
				if(m_stats.reloading)
				{
					if(m_stats.ammo>5)
					{
						m_stats.reloading=false;
					}
					else
					{
						//if(MVCTime::GetInstance()->TestTime(m_stats.bulletRef))
						{
							m_stats.ammo++;
						}
					}
				}
				if(ObjHandle::GetInstance()->EnemyinProx(side,pos,100)>1)
				{
					if(m_stats.ammo<=0&&!m_stats.reloading)
					{
						MVCTime::GetInstance()->SetLimit(m_stats.bulletRef,1000);
						m_stats.reloading=true;
					}
					else if(MVCTime::GetInstance()->TestTime(m_stats.bulletRef))
					{
						m_stats.ammo--;
						atkTarget=ObjHandle::GetInstance()->EnemytoShoot(side,pos,100);
						Shoot();
					}
					changedState=true;
				}
				if(squadBoard!=NULL)
				{
					if(m_currentOrders==NULL)
					{
						m_currentOrders=this->squadBoard->GetMessage1(this,ORDER);
						if(m_currentOrders!=NULL)
							if(m_currentOrders->general==false)
							{
								m_currentOrders->taken=true;
							}
					}
					else
					{
						MessageStruc* temp=squadBoard->GetMessage1(this,ORDER);
						if(temp!=NULL)
						{
							if(temp->priority>m_currentOrders->priority)
							{
								if(temp->general==false)
									temp->taken=true;
								m_currentOrders->taken=false;
								m_currentOrders=temp;
							}
						}
					}

					
					//do testing for normal states
					//will be a normal state machine
					//idle will be use orders

					if(m_currentOrders!=NULL)
					{
						switch(m_currentOrders->m_Content)
						{
						case LEADERDOWN:
							m_currentOrders->active;
							m_isLeader=true;
							squadBoard->SLeader=this;//this is to set the leader in the squadboard
							squadBoard->pos=&this->pos;
							m_currentOrders=NULL;
							recruiting=false;
							break;
						case ATTACKHERE:
							{
								if(!changedState)
								{
									Vector3D atkPos=*(Vector3D*)(m_currentOrders->info);
									if((this->pos-atkPos).GetMagnitudeSquare()>100)//within 10 units
									{
										Vector3D dir=atkPos-this->pos;
										dir.NormalizeVector3D();
										this->pos=pos+dir*delta*50;
										//state = movestate
									}
									else
									{
										m_currentOrders=NULL;
									}
								}
							}
							break;
						case RETREAT:
							{
								float distSquare;
								if(side==0)
								{
									distSquare=(pos-Vector3D(150,400,0)).GetMagnitudeSquare();
								}
								else
									distSquare=(pos-Vector3D(800,400,0)).GetMagnitudeSquare();
								if(distSquare>=40000)///sqrt of 40000 is 200
								{
									Vector3D dir;
									if(side==0)
									{
										dir=Vector3D(150,400,0)-this->pos;
									}
									else
									{
										dir=Vector3D(800,400,0)-this->pos;
									}
										dir.NormalizeVector3D();
									this->pos=pos+dir*delta*50;
								}
							}
						}
					}
				}
				else//no squadboard aka nosquad type
				{
					if(!reqSquad)
					{
						MessageStruc* temp=new MessageStruc;
						temp->active=true;
						temp->taken=false;
						temp->general=false;
						temp->m_Type=REPORT;
						temp->m_Content=FINDINGSQUAD;
						temp->priority=50;
						temp->info=this;
						MessageBoardGlobal::GetInstance(side)->SendMessage1(temp);
						reqSquad=true;
					}
				}
			}
		}
	}
	//update stuff


	/*psudo code for this method
	test if leader
	{
		check global messages

		send message to squad board(message should be based on current state)
	
		switch state based on message
	}
	if not leader
	{
		check squad messages

		switch state based on message
	}

	update here

	*/
}

bool Soldier2::SwitchState()
{
	if(MVCTime::GetInstance()->TestTime(m_underFireLimit))
	{
		int i=ObjHandle::GetInstance()->BulletsInProx(side,pos,100.0f);
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
			m_state=OTHER;
			m_stats.reloading=true;
			return true;
		}
		break;
	}
}

bool Soldier2::UnderFire(float priority)//state changer
{
	return false;
}

void Soldier2::bulletHit(bullet* bul)
{
	if(bul->GetActive())
	{
		bul->SetActive(false);
		m_stats.hp-=1;
	}
}

void Soldier2::Shoot()
{
	if(IsAlive())
	{
		Vector3D aim;
		if(atkTarget!=NULL)
		{
			aim=*atkTarget-pos;
		}
		else
		{
			aim=spd;
		}
		aim.NormalizeVector3D();
		float recoil =rand()%50-25;
		recoil=recoil/180*3.142;
		Vector3D side;
		aim.m_z=0;
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
			side.Set(-aim.m_x,aim.m_y,0);
		}
		side=side*(tan(recoil));
		aim=(side+aim);
		aim.NormalizeVector3D();
		aim=aim*300;
		ObjHandle::GetInstance()->GetBullet(rand()%500+250,pos,aim,this->side);
	}
}