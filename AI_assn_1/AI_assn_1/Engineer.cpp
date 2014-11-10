#include "Engineer.h"


CEngineer::CEngineer(void)
{
	m_state = IDLE;
	m_moveState = MOVETORECHARGE;
	m_stats.charge = 3;
	m_stats.hp = 8;
}


CEngineer::~CEngineer(void)
{
}

void CEngineer::switchState()
{
	switch (m_state){

	}
}