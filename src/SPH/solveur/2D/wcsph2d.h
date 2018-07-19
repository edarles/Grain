/****************************************************************************/
/****************************************************************************/
#ifndef _WCSPH_2D_H__
#define _WCSPH_2D_H__

#include <sesph2d.h>

// Weakly Compressible SPH Solver with Tait's equation to compute pressure
// Low compressible effect

class WCSPH2D : public SESPH2D
{
/****************************************************************************/
	public : 

		WCSPH2D(Vector3f origin,Vector3f min, Vector3f max);
		virtual ~WCSPH2D();


/****************************************************************************/
	protected : 

		virtual void computeRhoP();
};
#endif
/****************************************************************************/
/****************************************************************************/