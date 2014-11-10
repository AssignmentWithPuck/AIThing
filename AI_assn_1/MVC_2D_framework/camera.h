#ifndef __CAMERA_H__
#define __CAMERA_H__

#ifndef __VECTOR_3D_H__
#include "Vector3D.h"
#endif

#ifndef __WINDOWS_H__
#include <windows.h> // Header File For Windows
#define __WINDOWS_H__
#endif
#ifndef __GL_H__
#include <gl\gl.h> // Header File For The OpenGL32 Library
#define __GL_H__
#endif

class Camera
{
public:
	enum CAM_TYPE
	{
		LAND_CAM,
		AIR_CAM
	};
	Camera(void);
	Camera(CAM_TYPE ct);
	~Camera(void);
	void SetCameraType(CAM_TYPE ct);
	void Reset(void);
	void Update(void);
	void UpdateCamera(int diffX,int diffY);
	Vector3D GetPosition();
	Vector3D GetDirection(void);
	void SetPosition(GLfloat x,GLfloat y,GLfloat z);
	void SetDirection(GLfloat x,GLfloat y,GLfloat z);
	

	// Toggle HUD mode 
	void SetHUD(bool bHUDmode); 

	void Pitch(GLfloat theta);
	void Yaw(GLfloat theta);
protected:
	CAM_TYPE m_cameraType;
	Vector3D m_position;
	Vector3D m_realPos;
	Vector3D m_along;
	Vector3D m_up;
	Vector3D m_forward;
	float m_speed;
	float m_fspeed;
	float m_yspeed;
	float m_hAngle;
	bool m_dir;
	bool m_fdir;
	bool m_jumping;
};

#endif