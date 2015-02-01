#ifndef __MESSAGEBOARD_H__
#define __MESSAGEBOARD_H__

#ifndef __AI_HEADER_H__
#include "AI_header.h"
#endif

#ifndef __VECTOR_H__
#include <vector>
#define __VECTOR_H__
#endif
#include "AI_header.h"

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
	FINDINGSQUAD,//report
	HIGH_DANGER,//report tell sending squad to retreat
	MEDIUM_DANGER,//report send a squad to reinforce by an attack here move
	DISCARD_OLD_ORDERS,
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
	baseObj* takenBy;
	void* info;//additional info(can be anything, such as a position,a target or nothing)
	MessageStruc()
	{
		info=NULL;
		takenBy=NULL;
	}
};

class MessageBoard
{
public:
	virtual MessageStruc* GetMessage1(baseObj* from);
	virtual MessageStruc* GetMessage1(baseObj* from, MessageType);
	virtual MessageStruc* GetMessage1(objType target,MessageType,baseObj*from);
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
	static MessageBoardGlobal* GetInstance(int i);
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
	Vector3D SquadColor;
	void Update();
	std::vector<MessageStruc*> sentMessages;
	void PushMember(baseObj*);
	baseObj* SLeader;
	int side;
	Vector3D* pos;
	std::vector<baseObj*> SMember;
	bool full;
	bool sentOrders;
};

class Commander
{
public:
	Commander();
	~Commander();
	void Update();
	Commander* otherCommander;
	std::vector<SquadBoard*> squadList;
	int side;
	Vector3D pos;
	int timeRef;
	bool Init();
	Vector3D fixedAttackPos[5];//for the fixed attack positions
	ObjHandle* spawn;//to spawn the soldiers
private:
	void ProcessReports();
};

#endif