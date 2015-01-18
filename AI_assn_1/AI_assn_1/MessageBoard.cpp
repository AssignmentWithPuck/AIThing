#include "MessageBoard.h"

using namespace std;

MessageBoard::MessageBoard(void)
{
}


MessageBoard::~MessageBoard(void)
{
}

void MessageBoard::SendMessage(MessageStruc* nMessage)
{
	messageList.push_back(nMessage);
}

MessageStruc* MessageBoard::GetMessage(objType target)
{
	int currentPrior=0;
	MessageStruc* temp=NULL;
	for(vector<MessageStruc*>::iterator it;it!=messageList.end();++it)
	{
		if((*it)->target!=target)
			continue;
		if((*it)->priority>currentPrior)
		{
			temp=(*it);
			currentPrior=(*it)->priority;
		}
	}
	return temp;
}

void MessageBoard::Update()
{
	for(vector<MessageStruc*>::iterator it;it!=messageList.end();++it)
	{
		if((*it)->active!=true)
			continue;
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
	
	for(vector<MessageStruc*>::iterator it;it!=messageList.end();++it)
	{
		if((*it)->active!=true)
			continue;
		(*it)->priority++;
	}
}