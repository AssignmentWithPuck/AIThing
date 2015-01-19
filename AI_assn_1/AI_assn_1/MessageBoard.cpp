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
	int currentPrior=-1;
	vector<MessageStruc*>::iterator temp;
	for(vector<MessageStruc*>::iterator it;it!=messageList.end();++it)
	{
		if((*it)->target!=target)
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
	int currentPrior=-1;
	
	vector<MessageStruc*>::iterator temp;
	for(vector<MessageStruc*>::iterator it;it!=messageList.end();++it)
	{
		if((*it)->m_Type!=targetType)
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
	int currentPrior=-1;
	
	vector<MessageStruc*>::iterator temp;
	for(vector<MessageStruc*>::iterator it;it!=messageList.end();++it)
	{
		if((*it)->m_Type!=targetType||(*it)->target!=target)
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

void MessageBoard::Update()
{
	for(vector<MessageStruc*>::iterator it;it!=messageList.end();++it)
	{
		if(!(*it)->active)
			continue;
		if(!(*it)->general)
			(*it)->priority++;
	}
}

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

}

SquadBoard::~SquadBoard()
{

}

void SquadBoard::Update()
{
	if(SMember.size()<=0)
	{
		active=false;
	}
	else
		active=true;
	if(active)
	{
		if(SLeader==NULL)
		{
			bool send=true;
			for(vector<MessageStruc*>::iterator it;it!=messageList.end();++it)
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
		for(vector<MessageStruc*>::iterator it;it!=messageList.end();++it)
		{
			if((*it)->active!=true)
				continue;
			if(!(*it)->general)
				(*it)->priority++;
		}
	}
}