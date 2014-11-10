#ifndef __CAMERA_H__
#include"camera.h"
#endif

#ifndef __WINDOWS_H__
#include <windows.h> // Header File For Windows
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

Camera::Camera(CAM_TYPE ct)
{
	SetCameraType(ct);
	Reset();
}

Camera::~Camera()
{

}

void Camera::SetCameraType(CAM_TYPE ct)
{
	m_cameraType=ct;
}

void Camera::Reset(void)
{
	m_position=Vector3D(0.0,0.0f,-10.0f);
	m_realPos=m_position;
	m_along=Vector3D(1.0,0.0,0.0);
	m_up=Vector3D(0.0,1.0,0.0);
	m_forward=Vector3D(0.0,0.0,1.0);
	m_fspeed=0;
	m_speed=0;
	m_yspeed=0;
	m_hAngle=0;
	m_dir=false;
	m_fdir=false;
}

void Camera::Update()
{
	gluLookAt(m_position.m_x,m_position.m_y,m_position.m_z,
		m_position.m_x+m_forward.m_x,m_position.m_y+m_forward.m_y,
		m_position.m_z+m_forward.m_z,
		0.0,0.1,0.0);
}

void Camera::UpdateCamera(int diffX,int diffY)
{
	Pitch(diffY*3.142f/180.0f);
	////Update on x and z axis
	m_hAngle += (float) diffX * 3.142f / 180.0f;
	if (m_hAngle > 6.284f)
	m_hAngle-=6.284f;
	else if (m_hAngle<-6.284f)
	m_hAngle += 6.284f;
	Yaw(-m_hAngle);
}

Vector3D Camera::GetPosition()
{
	return m_position;
}

void Camera::SetPosition(GLfloat x,GLfloat y,GLfloat z)
{
	this->m_position.Set(x,y,z);
}

void Camera::SetDirection(GLfloat x,GLfloat y,GLfloat z)
{
	this->m_forward.Set(x,y,z);
}

void Camera::Pitch(GLfloat theta)
{
	m_forward.m_y-=theta;
	if(m_forward.m_y>3.142f)
		m_forward.m_y=3.142f;
	else if(m_forward.m_y<-3.142f)
		m_forward.m_y=-3.142f;
}

void Camera::Yaw(GLfloat theta)
{
	m_forward.m_x=sin(-theta);
	m_forward.m_z=-cos(-theta);
}

// Toggle HUD mode 
void Camera::SetHUD(bool bHUDmode) 
{ 
	if(bHUDmode) 
	{ 
		glMatrixMode(GL_PROJECTION); 
		glPushMatrix(); 
		glLoadIdentity(); 
		glOrtho( 0, 800 , 600, 0, -1, 1 ); 
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity(); 
		glDisable(GL_DEPTH_TEST); 
		glDisable(GL_TEXTURE_2D); // Disable Texture Mapping ( NEW ) 
	} 
	else 
	{ 
		glEnable(GL_TEXTURE_2D); // Enable Texture Mapping ( NEW ) 
		glEnable(GL_DEPTH_TEST); 
		glMatrixMode(GL_PROJECTION); 
		glPopMatrix(); 
		glMatrixMode(GL_MODELVIEW); 
		glEnable( GL_DEPTH_TEST ); 
	} 
}


Vector3D Camera::GetDirection()
{ 
	return m_forward; 
} 

