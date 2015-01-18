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
	HEALME
};

struct MessageStruc
{
	MessageType mType;//this should tell where its from
	MessageContent mContent;
	bool active;//to be deleted or not
	objType target;//what to target not a pointer so its a general target
	int priority;//starting priority min 0-not important, max 100-should be immediate
	void* info;//additional info(can be anything, such as a position,a target or nothing)
};

class MessageBoard
{
public:
	MessageStruc* GetMessage(objType);
	void SendMessage(MessageStruc*);
	MessageBoard();
	~MessageBoard();
	virtual void Update();
protected:
	std::vector<MessageStruc*> messageList;
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
private:
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