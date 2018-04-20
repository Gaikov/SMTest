#ifndef _NSCAMERA_H_
#define _NSCAMERA_H_

#include "elocal.h"

class nsCamera  
{
friend class nsRenderScene;
public:
	nsCamera();
	virtual ~nsCamera();
	void	Move();
	inline const nsVec3& GetPos() { return m_pos; };

private:
	nsVec3	m_pos, m_at, m_up;
	float	m_angle;
};

extern	nsCamera	g_mainCam;

#endif
