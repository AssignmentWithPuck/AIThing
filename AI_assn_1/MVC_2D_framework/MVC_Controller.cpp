//*************************************************************************************************************************************//
//
//	Original MVC framework made by Mr Toh Da Jun for DM2231 Game Development Techniques
//	Adapted and modified by Kennard Kee Wei Sheng
//
//************************************************************************************************************************************//

#ifndef __MVC_CONTROLLER_H__
#include "MVC_Controller.h"
#endif

#ifndef __MVC_MODEL_H__
#include "MVC_Model.h"
#endif

#ifndef __MVC_VIEW_H__
#include "MVC_View.h"
#endif

#include "MessageBoard.h"

using namespace std;

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

MVC_Controller::MVC_Controller(MVC_Model* theModel, MVC_View* theView)
: m_theModel(NULL)
, m_theView(NULL)
, m_bContinueLoop(false)
{
	m_theModel = theModel;
	m_theView = theView;
}

MVC_Controller::~MVC_Controller(void)
{
	m_theView = NULL;
	m_theModel = NULL;
}

bool MVC_Controller::Init(void)
{


	if(!m_theModel->Init(1000))
		return false;

	return true;
}


// Get the status of the stop game boolean flag
BOOL MVC_Controller::RunMainLoop(void)
{

	MSG msg; // Windows Message Structure
	BOOL done=FALSE; // Bool Variable To Exit Loop
		// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		m_theView->SetFullScreen( false );
		// Create Our OpenGL Window
		if (!m_theView->CreateGLWindow("NeHe's OpenGL Framework",1024,800,16))
		{
			return false;									// Quit If Window Was Not Created
		}
	}
	else
	{
		m_theView->SetFullScreen( true );
		int height,width;
		m_theView->GetDefaultRes(height,width);
		// Create Our OpenGL Window
		if (!m_theView->CreateGLWindow("NeHe's OpenGL Framework",width,height,16))
		{
			return false;									// Quit If Window Was Not Created
		}
	}
	
	m_theModel->InitPhase2();

	while(!done) // Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) // Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT) // Have We Received A Quit Message?
			{
				done=TRUE; // If So done=TRUE
			}
			else // If Not, Deal With Window Messages
			{
				TranslateMessage(&msg); // Translate The Message
				DispatchMessage(&msg); // Dispatch The Message
			}
		}
		else // If There Are No Messages
		{
			if (ProcessInput())
			{
				m_theModel->Update();
				m_theView->Draw();
			}
			else
			{
				done=TRUE; // ESC or DrawGLScene Signalled A Quit
			}
		}
	}

	// Shutdown
	m_theView->KillGLWindow(); // Kill The Window
	return (msg.wParam); // Exit The Program
}

// Process input from I/O devices
bool MVC_Controller::ProcessInput(void)
{
	// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
	if (m_theView->IsQuitGame())				// Was ESC Pressed?
	{
		m_bContinueLoop=false;						// ESC Signalled A Quit
		return false;
	}

	if (m_theView->RunFullScreen())						// Is F1 Being Pressed?
	{
		if (m_theView->ToggleFullScreen() == false)
		{
			m_bContinueLoop=false;						// ESC Signalled A Quit
			return false;
		}
	}
	
	ProcKeyboard();
	
	ProcMouse();
	
	return true;
}

void MVC_Controller::ProcMouse()
{
	int w;
	int h;
	m_theView->GetSize(&w,&h);
	if(m_theView->m_MouseInfo.m_LButtonDown)
	{
		m_theView->m_MouseInfo.m_LButtonDown=false;
	}
	else if(m_theView->m_MouseInfo.m_LButtonUp)
	{
		m_theView->m_MouseInfo.m_LButtonUp=false;
	}

	if(m_theView->m_MouseInfo.m_RButtonDown)
	{
		m_theView->m_MouseInfo.m_RButtonDown=false;
	}
	else if(m_theView->m_MouseInfo.m_RButtonUp)
	{
		m_theView->m_MouseInfo.m_RButtonUp=false;
	}
}

int MVC_Controller::ProcKeys(int key)
{
	if(key>=97&&key<122)
	{
		return 0x41+(key-97);
	}
	else if(key>=48&&key<57)
	{
		return 0x30+(key-48);
	}
	return 0;
}

void MVC_Controller::ProcKeyboard()
{
	bool* temp = m_theView->GetKeyBuffer();
	if(temp[VK_SPACE])
	{
		//m_theModel->thing.UnderFire();
	}
	if(temp['A'])
	{
		ObjHandle::GetInstance()->PushObj(SOLDIER_TYPE,Vector3D(rand()%400+50,rand()%400+50,0),0);
		temp['A']=false;
	}
	if(temp['D'])
	{
		temp['D']=false;
	}
	if(temp['S'])
	{
		for(vector<MessageStruc*>::iterator it=m_theModel->theSquad.sentMessages.begin();it!=m_theModel->theSquad.sentMessages.end();++it)
		{
			if((*it)->m_Content==RECRUITING);
			{
				(*it)->taken=!(*it)->taken;
				m_theModel->theSquad.full=!m_theModel->theSquad.full;
			}
		}
		temp['S']=false;
	}

}