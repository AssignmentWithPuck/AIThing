#ifndef __MESSAGEBOARD_H__
#define __MESSAGEBOARD_H__

#ifndef __AI_HEADER_H__
#include "AI_header.h"
#endif

#ifndef __VECTOR_H__
#include <vector>
#define __VECTOR_H__
#endif

enum MessageType
{
	ORDER=0,//issued by command and read by squad leader
	REPORT//issued by squadleader and read by command
};

enum MessageContent
{
	ATTACKHERE,
	LEADERDOWN,
	RETREAT,
	RECRUITING,
	FINDINGSQUAD,
	HEAL,
	BUSY
};

struct MessageStruc//probably consider changing the info to templates
{
	MessageType m_Type;//this should tell where its from
	MessageContent m_Content;
	bool active;//to be deleted or not
	bool general;//if its a general message meant for everyone//priority will not increase
	bool taken;//to test if its being used by someone alreadys
	objType target;//what to target not a pointer so its a general target
	int priority;//starting priority min 0-not important, max 100-should be immediate
	void* info;//additional info(can be anything, such as a position,a target or nothing)
};

class MessageBoard
{
public:
	virtual MessageStruc* GetMessage1(objType);
	virtual MessageStruc* GetMessage1(objType, MessageType);
	virtual MessageStruc* GetMessage1(MessageType);
	virtual void SendMessage1(MessageStruc*);
	MessageBoard();
	~MessageBoard();
	virtual void Update();
	std::vector<MessageStruc*> messageList;
protected:
};

class MessageBoardGlobal:public MessageBoard//communication between squads
{
public:
	static MessageBoardGlobal* GetInstance();
	static void Drop();
private:
	static MessageBoardGlobal* s_instance;
	MessageBoardGlobal(void);
	~MessageBoardGlobal(void);
};

class SquadBoard:public MessageBoard//should also have a list of squad boards
	//this should be the squad formation itself
	//its state should be the squad leaders state
{
public:
	SquadBoard();
	~SquadBoard();
	bool active;
	void Update();
	std::vector<MessageStruc*> sentMessages;
	void PushMember(baseObj*);
	baseObj* SLeader;
	std::vector<baseObj*> SMember;
};

class Commander
{
public:
	Commander();
	~Commander();
	void Update();
private:
	void ProcessReports();
	std::vector<MessageStruc*> orderList;//to change orders as nessasary
};

#endif