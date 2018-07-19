#include <wcsph2d_brownian.h>
#include <iostream>
/****************************************************************************/
/****************************************************************************/
WCSPH2D_BROWNIAN::WCSPH2D_BROWNIAN(Vector3f origin, Vector3f min, Vector3f max, float Intensity)
      :SESPH2D_BROWNIAN(origin,min,max,rho0 * 88.5 * 88.5 / (7.0 * 1000.0),Intensity)
{
}
/****************************************************************************/
WCSPH2D_BROWNIAN::~WCSPH2D_BROWNIAN()
{
}
/****************************************************************************/
/****************************************************************************/
void WCSPH2D_BROWNIAN::computeRhoP()
{
	#pragma omp parallel for
	for(unsigned int i=0;i<particles.size();i++)
	{
		double dens = 0;
		Vector3f pos1 = particles[i]->getPos(); 
        pos1[2] = 0;
		float h1 = particles[i]->getRadius();

		for(unsigned int j=0;j<particles[i]->getNbVois();j++){
			int index = particles[i]->getVois(j);
			Vector3f pos2 = particles[index]->getPos(); 
            pos2[2] = 0;
			float m2 = particles[index]->getMass();
			float h2 = particles[index]->getRadius();
            float h = (h1+h2)/2;
			float d = (pos1-pos2).norm();
			dens += m2*315.0*powf((h*h)-(d*d),3.0)/(64.0*M_PI*powf(h,9));
		}
		particles[i]->setRho((float)dens);
		// Calcul de la pression en utilisation l'équation de Tait
		particles[i]->setP((powf((float)(dens/rho0), 7.0) - 1) * k);
	}	
}
/****************************************************************************/
/****************************************************************************/
