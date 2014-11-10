#ifndef __AI_HEADER_H__
#define __AI_HEADER_H__

#ifndef __VECTOR_3D__
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
	SOLDIER,
	TURRET,
};


class BulletHandle
{
public:
	typedef struct
	{
		int lifeLeft;
		Vector3D pos,spd;
		source type;
	}bullet;//bullet struct
	static BulletHandle* getInstance();
	bullet* getBullet(int life,Vector3D pos,Vector3D spd,source type);
	void update();
	void draw();
private:
	std::vector<bullet*> m_bulletList;
	static BulletHandle* s_instance();
	BulletHandle();
	~BulletHandle();
};

#endif