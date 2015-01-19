#include "MessageBoard.h"

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

MessageStruc* MessageBoard::GetMessage1(objType target)
{
	Update();
	int currentPrior=-1;
	vector<MessageStruc*>::iterator temp;
	for(vector<MessageStruc*>::iterator it;it!=messageList.end();++it)
	{
		if((*it)->target!=target||!(*it)->active||(*it)->taken)
			continue;
		if((*it)->priority>currentPrior)
		{
			temp=(it);
			currentPrior=(*it)->priority;
		}
	}
	MessageStruc* temp2=*temp;
	return temp2;
}

MessageStruc* MessageBoard::GetMessage1(MessageType targetType)
{
	Update();
	int currentPrior=-1;
	
	vector<MessageStruc*>::iterator temp;
	for(vector<MessageStruc*>::iterator it=messageList.begin();it!=messageList.end();++it)
	{
		if((*it)->m_Type!=targetType||!(*it)->active||(*it)->taken)
			continue;
		if((*it)->priority>currentPrior)
		{
			temp=(it);
			currentPrior=(*it)->priority;
		}
	}
	MessageStruc* temp2=*temp;
	return temp2;
}

MessageStruc* MessageBoard::GetMessage1(objType target,MessageType targetType)
{
	Update();
	int currentPrior=-1;
	
	vector<MessageStruc*>::iterator temp;
	temp._Ptr=NULL;
	for(vector<MessageStruc*>::iterator it=messageList.begin();it!=messageList.end();++it)
	{
		if((*it)->m_Type!=targetType||(*it)->target!=target||!(*it)->active||(*it)->taken)
			continue;
		if((*it)->priority>currentPrior)
		{
			temp=(it);
			currentPrior=(*it)->priority;
		}
	}
	if(temp._Ptr!=NULL)
	{
		MessageStruc* temp2=*temp;
		return temp2;
	}
	return NULL;
}

void MessageBoard::Update()
{
	for(vector<MessageStruc*>::iterator it=messageList.begin();it!=messageList.end();++it)
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
			swap(it,messageList.end());
			messageList.pop_back();
		}
		if(*it==NULL)
			return;

		if(!(*it)->general)
			(*it)->priority++;
	}
}

MessageBoardGlobal* MessageBoardGlobal::s_instance=NULL;

MessageBoardGlobal* MessageBoardGlobal::GetInstance()
{
	if(s_instance==NULL)
	{
		s_instance=new MessageBoardGlobal;
	}
	return s_instance;
}

void MessageBoardGlobal::Drop()
{
	if(s_instance!=NULL)
	{
		delete s_instance;
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
}

SquadBoard::~SquadBoard()
{

}

void SquadBoard::Update()
{
	//if(active)
	{
		int go=false;
		for(vector<MessageStruc*>::iterator it=messageList.end();it!=messageList.end();++it)
		{
			while(!(*it)->active&&!go)
			{
				MessageStruc* temp=*it;
				delete temp;
				temp=NULL;
				if(it==messageList.end())
				{
					go=true;
					messageList.pop_back();
				}
				swap(it,messageList.end());
				messageList.pop_back();
			}
			if(go)
				break;
		}
	
		for(vector<MessageStruc*>::iterator it=sentMessages.begin();it!=sentMessages.end();++it)
		{
			while((*it)==NULL)
			{
				swap(it,messageList.end());
				messageList.pop_back();
			}
			while(!(*it)->active)
			{
				swap(it,messageList.end());
				messageList.pop_back();
			}
		}
		if(SLeader==NULL)
		{
			bool send=true;
			for(vector<MessageStruc*>::iterator it=messageList.begin();it!=messageList.end();++it)
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
		if(SMember.size()<=0)
		{
			MessageStruc* temp=new MessageStruc;
			temp->active=true;
			temp->taken=false;
			temp->general=false;
			temp->m_Type=REPORT;
			temp->m_Content=RECRUITING;
			temp->target=objType::NOSQUAD_TYPE;
			temp->priority=50;
			temp->info=this;
			MessageBoardGlobal::GetInstance()->SendMessage1(temp);
			sentMessages.push_back(temp);
		}
		for(vector<MessageStruc*>::iterator it=messageList.begin();it!=messageList.end();++it)
		{
			if((*it)->active!=true)
				continue;
			if(!(*it)->general)
				(*it)->priority++;
		}
	}
}

void SquadBoard::PushMember(baseObj* nObj)
{

}