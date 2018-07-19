/****************************************************************************/
/****************************************************************************/
#ifndef _GRID_SPH_H__
#define _GRID_SPH_H__

#include <gridFluid.h>
#include <sphParticle.h>

class GridSPH : public GridFluid
{
/****************************************************************************/
	public : 

		GridSPH();
		GridSPH(Vector3f min, Vector3f max, float dx, float dy, float dz);
		GridSPH(Vector3f min, Vector3f max, float dx, float dy, float dz, float intensity);
		virtual ~GridSPH();

		/**************** ROUTINE DE CALCUL DE VOISINAGE *******/
		void computeNeighborhood(vector<SPHParticle*> particles);
		void computeNeighborhood2D(vector<SPHParticle*> particles);

	
};
#endif
/****************************************************************************/
/****************************************************************************/
