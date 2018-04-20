#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "ELocal.h"
#include "Camera.h"

extern	const float	SCENE_CUBE_SIZE;
extern	const float	SCENE_HALF_SIZE;

const nsVec3	windForce( 5, 0, 0 );

typedef struct 
{
	nsVec3	pos;
	nsVec3	vel;
	nsColor	color;
	float	size;
	float	squreDist;
}
particle_t;

int		CmpPartDist( const void* item1, const void* item2 );

//////////////////////////////////////////////////////////////////////////

template<uint MAX_PARTICLES, class INIT_FUNC, class MOVE_FUNC, bool SORT>
class nsParticleSystem
{
private:
	particle_t	m_parts[MAX_PARTICLES];
	MOVE_FUNC	m_funcMove;
	INIT_FUNC	m_funcInit;

public:
	//-----------------------------------------------------
	//  nsParticleSystem:  
	//-----------------------------------------------------
	nsParticleSystem()
	{
		particle_t	*p = m_parts;
		for ( int i = 0; i < MAX_PARTICLES; i++, p++ )
			m_funcInit( p );
	}
	
	//-----------------------------------------------------
	//  nsParticleSystem:  
	//-----------------------------------------------------
	virtual ~nsParticleSystem() {}

	//-----------------------------------------------------
	//  Loop:  
	//-----------------------------------------------------
	inline void Loop()
	{
		particle_t	*p = m_parts;
		for ( int i = 0; i < MAX_PARTICLES; i++, p++ )
		{
			m_funcMove( p );
			if ( SORT ) p->squreDist = p->pos.Dot( g_mainCam.GetPos() );
		}

		
		if ( SORT )
			qsort( m_parts, MAX_PARTICLES, sizeof(particle_t), CmpPartDist );
	}

	//-----------------------------------------------------
	//  GetParticles:  
	//-----------------------------------------------------
	inline const particle_t* GetParticles()
	{
		return m_parts;
	}

	//-----------------------------------------------------
	//  GetParticlesCount:  
	//-----------------------------------------------------
	inline uint GetParticlesCount()
	{
		return MAX_PARTICLES;
	}
};

//////////////////////////////////////////////////////////////////////////

struct nsSpriteInit
{
	inline void operator() ( particle_t* p )
	{
		p->size = SCENE_CUBE_SIZE * 0.05f;
		p->pos.Rand();
		p->pos *= (SCENE_CUBE_SIZE - p->size) * 0.5f;
		p->vel.Rand();
		p->color.Rand();
	}
};

struct nsSpriteMove
{
	inline void operator() ( particle_t* p )
	{
		p->vel += windForce * g_frameTime;
		p->pos += p->vel * g_frameTime;
		float	size2 = p->size * 0.5f;
		if ( MathAbsf( p->pos.x ) + size2 > SCENE_HALF_SIZE ||
			MathAbsf( p->pos.y ) + size2 > SCENE_HALF_SIZE ||
			MathAbsf( p->pos.z ) + size2 > SCENE_HALF_SIZE )
		{
			p->pos.Rand();
			p->pos *= (SCENE_CUBE_SIZE - p->size) * 0.5f;
			p->vel.Rand();
		}//*/
	}
};

struct nsCubeInit
{
	inline void operator() ( particle_t* p )
	{
		p->size = SCENE_CUBE_SIZE * 0.07f;
		p->pos.Rand();
		p->pos *= (SCENE_CUBE_SIZE - p->size) * 0.5f;
		p->vel.Rand();
	}
};

struct nsCubeMove
{
	inline void operator() ( particle_t* p )
	{
		p->pos += p->vel * g_frameTime;
		float	size2 = p->size * 0.5f;

		if ( p->pos.x > SCENE_HALF_SIZE - size2 )
		{
			p->pos.x = SCENE_HALF_SIZE - size2;
			p->vel.x = -p->vel.x;
		}
		else if ( p->pos.x < -SCENE_HALF_SIZE + size2 )
		{
			p->pos.x = -SCENE_HALF_SIZE + size2;
			p->vel.x = -p->vel.x;
		}

		if ( p->pos.y > SCENE_HALF_SIZE - size2 )
		{
			p->pos.y = SCENE_HALF_SIZE - size2;
			p->vel.y = -p->vel.y;
		}
		else if ( p->pos.y < -SCENE_HALF_SIZE + size2 )
		{
			p->pos.y = -SCENE_HALF_SIZE + size2;
			p->vel.y = -p->vel.y;
		}

		if ( p->pos.z > SCENE_HALF_SIZE - size2 )
		{
			p->pos.z = SCENE_HALF_SIZE - size2;
			p->vel.z = -p->vel.z;
		}
		else if ( p->pos.z < -SCENE_HALF_SIZE + size2 )
		{
			p->pos.z = -SCENE_HALF_SIZE + size2;
			p->vel.z = -p->vel.z;
		}
	}
};


#endif
