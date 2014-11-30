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
	glColor3f(1,1,1);
	Printw(5,30,"FPS: %.2f",MVCTime::GetInstance()->GetFPS());
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,m_theModel->background.texID);
		glTranslatef(this->m_iWindows_Width*0.5,this->m_iWindows_Height*0.5,0);
		glScalef(this->m_iWindows_Width,this->m_iWindows_Height,0);
		basicShape::drawSquare();
	glPopMatrix();	
	ObjHandle::GetInstance()->Draw();
	//m_theModel->thing.Draw();
	//m_theModel->thing2.Draw();
	//m_theModel->eng.Draw();
	glColor3f(0.1f,0.1f,0.1f);
	Printw(30,40,"Soldier Victory: %d ",m_theModel->solVictory);
	Printw(30,60,"Engineer Victory: %d ",m_theModel->engiVictory);

	for(vector<baseObj*>::iterator it=ObjHandle::GetInstance()->m_AIList.begin();it!=ObjHandle::GetInstance()->m_AIList.end();++it)
	{
		if((*it)->GetActive())
		{
			switch((*it)->m_objType)
			{
			case SOLDIER_TYPE:
				{
					SoldierSMControl* temp=(SoldierSMControl*)(*it);
					Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-40,"Current HP");
					glPushMatrix();
						glTranslatef(temp->GetPos().m_x+100+temp->m_stats.hp*5,temp->GetPos().m_y-47,0);
						glScalef(temp->m_stats.hp*10,15,0);
						glBindTexture(GL_TEXTURE_2D,NULL);
						basicShape::drawCube();
					glPopMatrix();
					Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-55,"Ammo");
					glPushMatrix();
						glTranslatef(temp->GetPos().m_x+100+temp->m_stats.ammo*5,temp->GetPos().m_y-62,0);
						glScalef(temp->m_stats.ammo*10,15,0);
						glBindTexture(GL_TEXTURE_2D,NULL);
						basicShape::drawCube();
					glPopMatrix();
					switch(temp->m_state)
					{
					case SoldierSMControl::MOVE:
						switch(temp->m_moveState)
						{
						case SoldierSMControl::MOVEFORWARD:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: MoveForward");
							break;
						case SoldierSMControl::MOVETOCOVER:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: MoveToCover");
							break;
						case SoldierSMControl::MOVETOSHOOT:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: MoveToShoot");
							break;
						}
						break;
					case SoldierSMControl::ATTACK:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: Attack");

						break;
					case SoldierSMControl::COVER:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: Cover");

						break;
					case SoldierSMControl::DAMAGE:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: TakenDamaged");

						break;
					}
				}
				break;
			case TURRET_TYPE:
				{
					CTurret* temp=(CTurret*)(*it);
					Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-40,"Current HP");
					glPushMatrix();
						glTranslatef(temp->GetPos().m_x+100+temp->m_stats.hp*10,temp->GetPos().m_y-47,0);
						glScalef(temp->m_stats.hp*20,15,0);
						glBindTexture(GL_TEXTURE_2D,NULL);
						basicShape::drawCube();
					glPopMatrix();
					Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-55,"Ammo");
					glPushMatrix();
						glTranslatef(temp->GetPos().m_x+100+temp->m_stats.ammo*15,temp->GetPos().m_y-62,0);
						glScalef(temp->m_stats.ammo*30,15,0);
						glBindTexture(GL_TEXTURE_2D,NULL);
						basicShape::drawCube();
					glPopMatrix();
					switch(temp->m_state)
					{
					case CTurret::ATTACK:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: Attack");
						break;
					case CTurret::RELOAD:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: Reload");
						break;
					case CTurret::DAMAGED:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: TakenDamaged");
						break;
					}
				}
				break;
			case ENGINEER_TYPE:
				{
					CEngineer* temp=(CEngineer*)(*it);
					Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-40,"Current HP");
					glPushMatrix();
						glTranslatef(temp->GetPos().m_x+100+temp->m_stats.hp*10,temp->GetPos().m_y-47,0);
						glScalef(temp->m_stats.hp*20,15,0);
						glBindTexture(GL_TEXTURE_2D,NULL);
						basicShape::drawCube();
					glPopMatrix();
					Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-55,"Repair charge");
					glPushMatrix();
						glTranslatef(temp->GetPos().m_x+100+temp->m_stats.charge*0.5*20,temp->GetPos().m_y-62,0);
						glScalef(temp->m_stats.charge*20,15,0);
						glBindTexture(GL_TEXTURE_2D,NULL);
						basicShape::drawCube();
					glPopMatrix();
					switch(temp->m_state)
					{
					case CEngineer::IDLE:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: Idle");
							break;
					case CEngineer::REPAIR:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: Repair");
						break;
					case CEngineer::RECHARGE:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: Recharge");					
						break;
					case CEngineer::DAMAGED:
							Printw(temp->GetPos().m_x-25,temp->GetPos().m_y-25,"Current State: TakenDamaged");
						break;
					}
				}
				break;
			}
		}
	}
	
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

