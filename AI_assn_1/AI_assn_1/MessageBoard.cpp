#include "MessageBoard.h"
#include <vector>

using namespace std;

MessageBoard::MessageBoard(void)
{
}


MessageBoard::~MessageBoard(void)
{
}

void MessageBoard::SendMessage1(MessageStruc* nMessage)
{
	messageList.push_back(nMessage);
}

MessageStruc* MessageBoard::GetMessage1(baseObj* from)//from is the person that called this function
{
	//Update();
	objType target=from->m_objType;
	int currentPrior=-1;
	MessageStruc* temp=NULL;
	for(vector<MessageStruc*>::iterator it;it!=messageList.end();++it)
	{
		if((*it)->target!=target||!(*it)->active||(*it)->taken)
			continue;
		if((*it)->priority>currentPrior)
		{
			temp=(*it);
			currentPrior=(*it)->priority;
		}
	}
	if(temp!=NULL)
		(temp)->takenBy=from;
	return temp;
}

MessageStruc* MessageBoard::GetMessage1(objType target,MessageType targetType,baseObj* from)//from is the person that called this function
{
	//Update();
	int currentPrior=-1;
	MessageStruc* temp=NULL;
	for(vector<MessageStruc*>::iterator it=messageList.begin();it!=messageList.end();++it)
	{
		if((*it)->target!=target||!(*it)->active||(*it)->taken)
			continue;
		if((*it)->priority>currentPrior)
		{
			temp=(*it);
			currentPrior=(*it)->priority;
		}
	}
	if(temp!=NULL)
		(temp)->takenBy=from;
	return temp;
}

MessageStruc* MessageBoard::GetMessage1(baseObj* from,MessageType targetType)
{
	//Update();
	int currentPrior=-1;
	objType target=from->m_objType;	
	MessageStruc* temp=NULL;
	for(vector<MessageStruc*>::iterator it=messageList.begin();it!=messageList.end();++it)
	{
		if((*it)->m_Type!=targetType||(*it)->target!=target||!(*it)->active||(*it)->taken)
			continue;
		if((*it)->priority>currentPrior)
		{
			temp=(*it);
			currentPrior=(*it)->priority;
		}
	}
	if(temp!=NULL)
		(temp)->takenBy=from;//above cause it temp2 can be null
	return temp;
}

void MessageBoard::Update()
{
	for(vector<MessageStruc*>::iterator it=messageList.begin();it!=messageList.end();)
	{
		while(!(*it)->active)
		{
			MessageStruc* temp=*it;
			delete temp;
			temp=NULL;
			if(it==messageList.end()-1)
			{
				messageList.pop_back();
				return;
			}			
			it = messageList.erase(it);
			continue;
		}
		//if(*it==NULL)
		//	continue;

		if(!(*it)->general)
			(*it)->priority++;
		++it;
	}
}

MessageBoardGlobal* MessageBoardGlobal::s_instance=NULL;

MessageBoardGlobal* MessageBoardGlobal::GetInstance(int i)
{
	if(s_instance==NULL)
	{
		s_instance=new MessageBoardGlobal[2];
	}
	if(i<2&&i>=0)
		return s_instance+i;
	else
		return NULL;
}

void MessageBoardGlobal::Drop()
{
	if(s_instance!=NULL)
	{
		delete[] s_instance;
		s_instance=NULL;
	}
}

MessageBoardGlobal::MessageBoardGlobal()
{

}

MessageBoardGlobal::~MessageBoardGlobal()
{
	
}

SquadBoard::SquadBoard()
{
	SLeader=NULL;
	full=false;
}

SquadBoard::~SquadBoard()
{

}

void SquadBoard::Update()
{
	//if(active)
	{
		int go=false;
		bool stopRecruit=false;
		if(SMember.size()>9)
			stopRecruit=true;
		for(vector<MessageStruc*>::iterator it=messageList.end();it!=messageList.end();)
		{
			if(!(*it)->active)
			{
				MessageStruc* temp=*it;
				delete temp;
				it=messageList.erase(it);
				continue;
			}
			++it;
		}
	
		for(vector<MessageStruc*>::iterator it=sentMessages.begin();it!=sentMessages.end();)
		{
			if(!(*it)->active)
			{
				it=sentMessages.erase(it);
				continue;
			}
			else if((*it)->m_Content==RECRUITING&&stopRecruit)
			{
				(*it)->active=false;
				it=sentMessages.erase(it);
				continue;
			}
			++it;
		}
		if(SLeader==NULL)
		{
			bool send=true;
			for(vector<MessageStruc*>::iterator it=messageList.begin();it!=messageList.end();)
			{
				if((*it)->m_Content==LEADERDOWN&&!(*it)->taken)
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
				temp->general=false;
				temp->m_Type=ORDER;
				temp->m_Content=LEADERDOWN;
				temp->target=objType::SOLDIER_TYPE;
				temp->priority=100;
				SendMessage1(temp);
			}
		}
		for(vector<baseObj*>::iterator it=SMember.begin();it!=SMember.end();)
		{
			if(!(*it)->GetActive())
			{
				it=SMember.erase(it);
				continue;
			}
			++it;
		}
		if(SMember.size()<=0)
		{
			bool send=true;
			for(vector<MessageStruc*>::iterator it=sentMessages.begin();it!=sentMessages.end();)
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
				temp->info=this;
				MessageBoardGlobal::GetInstance(side)->SendMessage1(temp);
				sentMessages.push_back(temp);
			}
		}
		for(vector<MessageStruc*>::iterator it=messageList.begin();it!=messageList.end();)
		{
			if((*it)->active!=true)
			{
				it=messageList.erase(it);
				continue;
			}
			if(!(*it)->general)
				(*it)->priority++;
			++it;
		}
	}
}

void SquadBoard::PushMember(baseObj* nObj)
{
	SMember.push_back(nObj);
}

Commander::Commander()
{


}

Commander::~Commander()
{

}

void Commander::ProcessReports()
{
	MessageBoardGlobal* mb=MessageBoardGlobal::GetInstance(side);
	for(vector<MessageStruc*>::iterator it=mb->messageList.begin();it!=mb->messageList.end();)
	{
		MessageStruc* temp=*it;
		if(temp->m_Type==REPORT)
		{
			//process reports
			switch(temp->m_Content)
			{
			case HIGH_DANGER:
				if((*it)->active)
				{
					for(vector<SquadBoard*>::iterator it2=squadList.begin();it2!=squadList.end();)
					{
						baseObj* temp2=(baseObj*)temp->info;
						if((*it2)->SLeader==temp2)
						{
							(*it2)->SLeader->m_currentOrders->active=false;
							MessageStruc* temp=new MessageStruc;
							temp->active=true;
							temp->taken=true;
							temp->general=false;
							temp->m_Type=ORDER;
							temp->m_Content=RETREAT;
							temp->priority=500;
							mb->SendMessage1(temp);
							(*it2)->SLeader->m_currentOrders=temp;
							(*it)->active=false;//tell messageboard this is not needed anymore
						}
					}
				}
				break;
			case MEDIUM_DANGER:
				{
					if((*it)->active)
					{
						MessageStruc* temp=new MessageStruc;
						temp->active=true;
						temp->taken=false;
						temp->general=false;
						temp->m_Type=ORDER;
						temp->m_Content=ATTACKHERE;
						temp->priority=60;
						temp->info=(*it)->info;
						mb->SendMessage1(temp);
						(*it)->active=false;
					}
				}
				break;
			case FINDINGSQUAD:
				{
					bool move=false;
					SquadBoard* theSquad;
					for(vector<MessageStruc*>::iterator it2=mb->messageList.begin();it2!=mb->messageList.end();++it2)
					{
						if((*it2)->active&&(*it2)->m_Content==RECRUITING&&!(*it2)->taken)//cycle through all messages again to look for recruiting
						{
							theSquad=(SquadBoard*)(*it2)->info;
							move=true;
							break;//stop the cycle and move on cause i found a squad recruiting
						}
					}
					if(!move)//means no squad is looking for a member so create a new squad to put this fella in
					{
						theSquad=new SquadBoard;
						theSquad->side=side;
						squadList.push_back(theSquad);
					}
					baseObj* temp = (baseObj*)(*it)->info;
					theSquad->SMember.push_back(temp);
					temp->squadBoard=theSquad;
					//below is a questionable line
					temp->m_objType=SOLDIER_TYPE;
					it=mb->messageList.erase(it);
					continue;
				}
				break;
			default:
				break;
			}
		}
		++it;
	}
}

void Commander::Update()
{
	MessageBoardGlobal::GetInstance(side)->Update();
	for(vector<SquadBoard*>::iterator it=squadList.begin();it!=squadList.end();)
	{
		(*it)->Update();
		++it;
	}
	ProcessReports();
	//send/update orders
	//if there are enemy squads send attack here move priority on enemy squads i.e 50 on enemy squad vs 40 on base
	//if there are enemy base send attack here move
	//
}