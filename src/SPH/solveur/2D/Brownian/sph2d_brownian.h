/****************************************************************************/
/****************************************************************************/
#ifndef _SPH_2D_BROWNIAN_H__
#define _SPH_2D_BROWNIAN_H__

#include <sph2d.h>

class SPH2D_BROWNIAN : public SPH2D
{
/****************************************************************************/
	public : 

		SPH2D_BROWNIAN(Vector3f center, Vector3f min, Vector3f max, float Intensity);
		virtual ~SPH2D_BROWNIAN(); 

		virtual void generateParticles(Vector3f cmin, Vector3f cmax);
		
/****************************************************************************/
	protected : 
		virtual void computeForces();
		
    private :
        float intensity;
		bool first;
};
#endif
/****************************************************************************/
/****************************************************************************/
