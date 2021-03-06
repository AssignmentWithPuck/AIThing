#ifndef __AI_HEADER_H__
#define __AI_HEADER_H__

#ifndef __VECTOR_3D_H__
#include "vector3D.h"
#endif

#include "texture.h"

#ifndef __VECTOR_H__
#include <vector>
#define __VECTOR_H__
#endif

#ifndef __WINDOWS_H__
#include <Windows.h>// Header File For Windows
#define __WINDOWS_H__
#endif
#ifndef __GL_H__
#include <gl\gl.h> // Header File For The OpenGL32 Library
#define __GL_H__
#endif
#ifndef __GLU_H__
#include <gl\glu.h> // Header File For The GLu32 Library
#define __GLU_H__
#endif

class SquadBoard;

enum source
{
	NONE=0,
	SOLDIER,
	TURRET
};

enum objType
{
	SOLDIER_TYPE,
	TURRET_TYPE,
	ENGINEER_TYPE,
	LEADER_TYPE,
	NOSQUAD_TYPE,
	SQUAD_TYPE,
	BULLET
};

struct stats2
{
	int hp;
	int ammo;
	int timeRef;//for misc operations
	int bulletRef;//for reloading and shooting
	bool reloading;
	bool proning;
};
struct node
{
	Vector3D pos;
	std::vector<node*> neighbour;
};

struct MessageStruc;

class baseObj
{
public:
	virtual void Update(float delta)=0;
	virtual void Draw()=0;
	virtual bool Init()=0;
	virtual void SetPos(Vector3D pos);
	virtual void SetSpd(Vector3D spd);
	Vector3D GetPos();
	Vector3D GetSpd();
	bool GetActive();
	void SetActive(bool nActive);
	stats2 m_stats;
	baseObj();
	~baseObj();
	objType m_objType;
	SquadBoard* squadBoard;
	MessageStruc* m_currentOrders;
	int side;//determines which side the obj is in
	Vector3D pos,spd;
protected:
	bool active;
	Vector3D scale;
};

class bullet:public baseObj
{
public:
	bool Init();
	int lifeLeft;
	TextureImage bulletTex;
	void Update(float delta);
	void Draw();
	void Set(int life,Vector3D pos,Vector3D spd,Vector3D scale,int type);
	bullet();
	bullet(int life,Vector3D pos,Vector3D spd,int type);
	~bullet();
};

class ObjHandle
{
public:
	static ObjHandle* GetInstance();
	bullet* GetBullet(int life,Vector3D pos,Vector3D spd,int side);
	void Update(float delta);
	void PushObj(objType type,Vector3D pos,int side);

	int BulletsInProx(int mySide,Vector3D pos,float dist);
	Vector3D* EnemytoShoot(int mySide,Vector3D pos,float dist);//finds an enemy to shoot around the pos dist away
	int EnemyinProx(int mySide,Vector3D pos,float dist);//finds number of enemies around a pos dist away

	void Draw(void (*Printw)(float x, float y,const char* format, ...));// function pointer to print on screen function
	static void Drop();

	std::vector<baseObj*> m_AIList;// for each side
private:
	bool addedStuff;
	bool accessing;
	std::vector<baseObj*> m_objList;
	std::vector<baseObj*> m_backLog;
	std::vector<bullet*> m_bulletList;
	static ObjHandle* s_instance;
	ObjHandle();
	~ObjHandle();
};


#endif