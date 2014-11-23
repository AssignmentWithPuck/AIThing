#ifndef __AI_HEADER_H__
#define __AI_HEADER_H__

#ifndef __VECTOR_3D_H__
#include "vector3D.h"
#endif

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
	ENGINEER_TYPE
};

struct node
{
	Vector3D pos;
	std::vector<node*> neighbour;
};

class baseObj
{
public:
	virtual void Update(float delta)=0;
	virtual void Draw()=0;
	virtual void SetPos(Vector3D pos);
	virtual void SetSpd(Vector3D spd);
	Vector3D GetPos();
	Vector3D GetSpd();
	bool GetActive();
	baseObj();
	~baseObj();
protected:
	bool active;
	Vector3D pos,spd;
	Vector3D scale;
};

class bullet:public baseObj
{
public:
	int lifeLeft;
	source type;
	void Update(float delta);
	void Draw();
	void Set(int life,Vector3D pos,Vector3D spd,Vector3D scale,source type);
	bullet();
	bullet(int life,Vector3D pos,Vector3D spd,source type);
	~bullet();
};

class ObjHandle
{
public:
	static ObjHandle* GetInstance();
	bullet* GetBullet(int life,Vector3D pos,Vector3D spd,source type);
	void Update(float delta);
	void PushObj(objType type,Vector3D pos);
	int BulletsInProx(source src,Vector3D pos,float dist);
	void Draw();
	std::vector<Vector3D*> FindPath(baseObj* start,Vector3D end);
	std::vector<Vector3D*> FindPath(baseObj* start,baseObj* end);
	std::vector<Vector3D*> FindPath(Vector3D start,Vector3D end);
	static void Drop();
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