#include <sesph2d_brownian.h>
using namespace Eigen;
/****************************************************************************/
/****************************************************************************/
SESPH2D_BROWNIAN::SESPH2D_BROWNIAN(Vector3f origin, Vector3f min, Vector3f max, float k, float Intensity)
				 :SPH2D_BROWNIAN(origin, min,max, Intensity)
{
	this->k = k;
}
/****************************************************************************/
SESPH2D_BROWNIAN::~SESPH2D_BROWNIAN()
{
}
/****************************************************************************/
/****************************************************************************/
float SESPH2D_BROWNIAN::getK()
{
	return k;
}
/****************************************************************************/
/****************************************************************************/
void SESPH2D_BROWNIAN::setK(float k)
{
	this->k = k;
}
/****************************************************************************/
/****************************************************************************/
void SESPH2D_BROWNIAN::computeRhoP()
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
			dens += m2*315*powf((h*h)-(d*d),3)/(64*M_PI*powf(h,9));
		}
		particles[i]->setRho((float)dens);
		// Calcul de la pression en utilisation l'équation des gaz parfaits
		particles[i]->setP(k*(dens-rho0));
		//printf("dens: %f p:%f\n",particles[i]->getRho(),particles[i]->getP());
	}	
}
/****************************************************************************/
/****************************************************************************/