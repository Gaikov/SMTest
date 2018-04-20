#include "Camera.h"

nsCamera	g_mainCam;

nsCamera::nsCamera()
{
	m_up = nsVec3( 0, 1, 0 );	
	m_at = nsVec3( 0, 0, 0 );
}

nsCamera::~nsCamera()
{

}

void nsCamera::Move()
{
	m_angle += g_frameTime;// * 0.5f;

	m_pos.y = 1.2f * cosf( m_angle );
	m_pos.x = 4 * sinf( m_angle );
	m_pos.z = 2.5f * cosf( m_angle );
}
