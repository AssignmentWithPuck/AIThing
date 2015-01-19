#include "Soldier2.h"


Soldier2::Soldier2(void)
{
}


Soldier2::~Soldier2(void)
{
}

bool Soldier2::Init()
{
	type=objType::NOSQUAD_TYPE;
	
}

void Soldier2::Update(float delta)
{
	if(active)
	{
		if(m_isLeader)
		{
			MessageBoardGlobal* messageBoardG=MessageBoardGlobal::GetInstance();
			if(m_currentOrders==NULL)
			{
				m_currentOrders=messageBoardG->GetMessage1(ORDER);//no obj type cause
				if(m_currentOrders->general==false)
					m_currentOrders->taken=true;
			}
			else
			{
				MessageStruc* temp=messageBoardG->GetMessage1(ORDER);
				if(temp->priority>m_currentOrders->priority)
				{
					temp->taken=true;
					m_currentOrders->taken=false;
					m_currentOrders=temp;
				}
			}
			if(m_stats.hp<=0)
			{
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
		}
		else//not leader
		{
			if(m_currentOrders==NULL)
			{
				m_currentOrders=squadBoard->GetMessage1(type,ORDER);
				if(m_currentOrders->general==false)
				{
					m_currentOrders->taken=true;
				}
			}
			else
			{
				MessageStruc* temp=squadBoard->GetMessage1(type,ORDER);
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
				switch(m_currentOrders->m_Content)
				{
				case LEADERDOWN:
					m_isLeader=true;
					squadBoard->SLeader=this;//this is to set the leader in the squadboard
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
		//update stuff


		/*test if leader
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