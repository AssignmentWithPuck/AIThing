#include "Soldier2.h"

#ifndef __BASIC_H__
#include "basicShape.h"
#endif

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
	m_stats.hp=5;
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
		if(m_isLeader==true)
		{
			glColor3f(1.0f,0.0,1.0f);
		}
		else if(m_objType==SOLDIER_TYPE)
		{
			glColor3f(1.0f,1.0f,1.f);
		}
		else if(m_objType==NOSQUAD_TYPE)
		{
			glColor3f(0.0f,0.0f,1.0f);
		}
		else
		{
			glColor3f(0.0f,0.0f,0.f);
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
				squadBoard->SendMessage1(temp);
				//death code here;
			}
			else//still alive
			{
				//send orders to squad
				if(m_currentOrders!=NULL)//if not null
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
					bool changedState=false;//to check if state has been changed
				
					//switch state(only test conditions)

					if(!changedState)
					{
						switch(m_currentOrders->m_Content)
						{
						case ATTACKHERE:
							{
								Vector3D atkPos=*(Vector3D*)(m_currentOrders->info);
								if((this->pos-atkPos).GetMagnitudeSquare()>10000)
								{
									//state = movestate
								}
								else
								{
									//state = atkstate
								}
							}
							break;
						}
					}
				}
				else if(true)//testing for near base
				{
					if(squadBoard->SMember.size()<10)
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
				}
			}
			
		}
		else//not leader
		{
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
			
				//switch state(only test conditions)

				if(!changedState)
				{
					if(m_currentOrders!=NULL)
					{
						switch(m_currentOrders->m_Content)
						{
						case LEADERDOWN:
							m_currentOrders->active=false;
							m_isLeader=true;
							squadBoard->SLeader=this;//this is to set the leader in the squadboard
							m_currentOrders=NULL;
							break;
						case ATTACKHERE:
							{
								Vector3D atkPos=*(Vector3D*)(m_currentOrders->info);
								if((this->pos-atkPos).GetMagnitudeSquare()>10000)
								{
									//state = movestate
								}
								else
								{
									//state = atkstate
								}
							}
							break;
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