/****************************************************************************/
/****************************************************************************/
#ifndef _SESPH_2D_BROWNIAN_H__
#define _SESPH_2D_BROWNIAN_H__

#include <sph2d_brownian.h>

// SPH Solver with state equation to compute pressure
// No incompressible -> loi des gaz parfaits utilisée pour calculer la pression

class SESPH2D_BROWNIAN : public SPH2D_BROWNIAN
{
/****************************************************************************/
	public : 

		SESPH2D_BROWNIAN(Vector3f origin, Vector3f min, Vector3f max, float k, float Intensity);
		virtual ~SESPH2D_BROWNIAN();

		/**************** GETTERS / SETTERS ***************************/
		float   getK();
		void    setK(float k);
		

/****************************************************************************/
	protected : 
		float k;

		virtual void computeRhoP();
};
#endif
/****************************************************************************/
/****************************************************************************/