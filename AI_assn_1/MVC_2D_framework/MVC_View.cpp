#ifndef __MVC_VIEW_H__
#include "MVC_View.h"
#endif

#ifndef __MVC_MODEL_H__
#include "MVC_Model.h"
#endif

#ifndef __BASIC_H__
#include "basicShape.h"
#endif

#ifndef __MVCTIME_H__
#include "MVCtime.h"
#endif

#ifndef __VECTOR_H__
#include <vector>
#define __VECTOR_H__
#endif

#ifndef __WINDOWS_H__
#include <Windows.h> // Header File For Windows
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

using namespace std;

MVC_View::MVC_View(MVC_Model* theModel)
{
	m_theModel = theModel;

	m_MouseInfo.init();
	m_iWindows_Width = 1; 
	m_iWindows_Height = 1;
	m_Sheight=100;
	m_Swidth=150;

	m_bActive = true; // Window Active Flag Set To TRUE By Default
	m_bFullScreen = false; // Fullscreen Flag Set To Fullscreen Mode By Default

	m_hDC=NULL;
	m_hRC=NULL;
	m_hWnd=NULL;
	timeRef=MVCTime::GetInstance()->PushNewTime(1000);
	for (int i=0; i<256; i++)
	{
		m_keys[i] = false;
	}
}

MVC_View::~MVC_View(void)
{
	m_theModel = NULL;
}

// Draw the view
BOOL MVC_View::Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer
	glLoadIdentity(); // ReSet The Current Modelview Matrix
	glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,m_theModel->background.texID);
		glTranslatef(this->m_iWindows_Width*0.5,this->m_iWindows_Height*0.5,0);
		glScalef(this->m_iWindows_Width,this->m_iWindows_Height,0);
		basicShape::drawSquare();
	glPopMatrix();	
	
	ObjHandle::GetInstance()->Draw(Printw);
	
	glColor3f(0,0,0);
	Printw(5,30,"FPS: %.2f",MVCTime::GetInstance()->GetFPS());
	static string from,to,content;
	MessageStruc temp=*(MessageBoardGlobal::GetInstance(0)->messageList.back());
	if(MVCTime::GetInstance()->TestTime(timeRef,false))
	{
		switch(temp.target)
		{
		case SOLDIER_TYPE:
			to="soldier";
			break;
		case LEADER_TYPE:
		case SQUAD_TYPE:
			to="leader";
			break;
		}
		if(temp.m_Type==REPORT)
		{
			to="commander";
		}
		switch(temp.m_Content)
		{
		case FINDINGSQUAD:
			content="finding squad";
			from="soldier";
			break;
		case RECRUITING:
			content="recruiting";
			from="leader";
			MVCTime::GetInstance()->ResetTime(timeRef);
			break;
		case ATTACKHERE:
			content="attack here";
			from="commander";
			break;
		}
	}
	
	static string from2,to2,content2;
	MessageStruc temp2;
	temp2.active=false;
	if(m_theModel->m_commander->squadList.size()>0)
		if(m_theModel->m_commander->squadList[0]->SMember.size()>2)
		{
			if(m_theModel->m_commander->squadList[0]->SMember[1]->m_currentOrders!=NULL)
			{
				temp2=*m_theModel->m_commander->squadList[0]->SMember[1]->m_currentOrders;
				temp2.active=true;
			}
		}
	if(temp2.active)
	{
		switch(temp2.target)
		{
		case SOLDIER_TYPE:
			to2="soldier";
			break;
		case LEADER_TYPE:
		case SQUAD_TYPE:
			to2="leader";
			break;
		}
		switch(temp2.m_Content)
		{
		case RETREAT:
			content2="retreat";
			from2="leader";
			MVCTime::GetInstance()->ResetTime(timeRef);
			break;
		case ATTACKHERE:
			content2="attack here";
			from2="leader";
			break;
		}
	}
	
	
	Printw(5,50,"Message Content: %s  From:%s  To:%s",content.c_str(),from.c_str(),to.c_str());
	Printw(5,75,"Message Content: %s  From:%s  To:%s",content2.c_str(),from2.c_str(),to2.c_str());
	glColor3f(1,1,1);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	SwapBuffers(m_hDC); // Swap Buffers (Double Buffering)

	return TRUE; // Keep Going
}

int MVC_View::InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH); // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Black Background
	glClearDepth(1.0f); // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations

	BuildFont();
	
	return TRUE; // Initialization Went OK
}

unsigned MVC_View::m_base=0;