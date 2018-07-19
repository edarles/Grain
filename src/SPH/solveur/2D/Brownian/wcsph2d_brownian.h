/****************************************************************************/
/****************************************************************************/
#ifndef _WCSPH_2D_BROWNIAN_H__
#define _WCSPH_2D_BROWNIAN_H__

#include <sesph2d_brownian.h>

// Weakly Compressible SPH Solver with Tait's equation to compute pressure
// Low compressible effect

class WCSPH2D_BROWNIAN : public SESPH2D_BROWNIAN
{
/****************************************************************************/
	public : 

		WCSPH2D_BROWNIAN(Vector3f origin,Vector3f min, Vector3f max, float Intensity);
		virtual ~WCSPH2D_BROWNIAN();


/****************************************************************************/
	protected : 

		virtual void computeRhoP();
};
#endif
/****************************************************************************/
/****************************************************************************/