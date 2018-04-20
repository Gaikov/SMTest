#include "ParticleSystem.h"

int CmpPartDist( const void* item1, const void* item2 )
{
	particle_t	*p1 = (particle_t*)item1;
	particle_t	*p2 = (particle_t*)item2;
	
	if ( p1->squreDist > p2->squreDist )
		return 1;
	else if ( p1->squreDist == p2->squreDist )
		return 0;
	
	return -1;
}
