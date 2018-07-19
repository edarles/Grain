#include <sesph2d.h>
using namespace Eigen;
/****************************************************************************/
/****************************************************************************/
SESPH2D::SESPH2D(Vector3f origin, Vector3f min, Vector3f max, float k):SPH2D(origin, min,max)
{
	this->k = k;
}
/****************************************************************************/
SESPH2D::~SESPH2D()
{
}
/****************************************************************************/
/****************************************************************************/
float SESPH2D::getK()
{
	return k;
}
/****************************************************************************/
/****************************************************************************/
void SESPH2D::setK(float k)
{
	this->k = k;
}
/****************************************************************************/
/****************************************************************************/
void SESPH2D::computeRhoP()
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
