/****************************************************************************/
/****************************************************************************/
#ifndef _SESPH_2D_H__
#define _SESPH_2D_H__

#include <sph2d.h>

// SPH Solver with state equation to compute pressure
// No incompressible -> loi des gaz parfaits utilisée pour calculer la pression

class SESPH2D : public SPH2D
{
/****************************************************************************/
	public : 

		SESPH2D(Vector3f origin, Vector3f min, Vector3f max, float k);
		virtual ~SESPH2D();

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