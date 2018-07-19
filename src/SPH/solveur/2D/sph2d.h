/****************************************************************************/
/****************************************************************************/
#ifndef _SPH_2D_H__
#define _SPH_2D_H__

#include <sph.h>

class SPH2D : public SPH
{
/****************************************************************************/
	public : 

		SPH2D();
		SPH2D(Vector3f center, Vector3f min, Vector3f max);
		virtual ~SPH2D(); 

		virtual void generateParticles(Vector3f cmin, Vector3f cmax);
		virtual void update(float dt);
/****************************************************************************/
	protected : 
		virtual bool computeNeighborhood();
		virtual void computeForces();
		virtual void integrate(float dt);
		virtual void collision(float dt);
};
#endif
/****************************************************************************/
/****************************************************************************/
