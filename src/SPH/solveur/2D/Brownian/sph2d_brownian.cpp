#include <sph2d_brownian.h>
#include <iostream>
using namespace std;
/****************************************************************************/
/****************************************************************************/
SPH2D_BROWNIAN::SPH2D_BROWNIAN(Vector3f origin, Vector3f min, Vector3f max, float Intensity)
               :SPH2D()
{	
	Vector3f hV = Vector3f(h,h,0);
	minG = Vector3f(min[0]-h, min[1]-h,0);
	maxG = Vector3f(max[0]+h, max[1]+h,0);
	gridSPH = new GridSPH(minG,maxG,h,h,0,Intensity); 
	particles.clear();
    this->intensity = Intensity;
	generateParticles(minG,maxG);
	first = true;
}
/****************************************************************************/
SPH2D_BROWNIAN::~SPH2D_BROWNIAN()
{
}
/****************************************************************************/
void SPH2D_BROWNIAN::generateParticles(Vector3f cmin, Vector3f cmax)
{
	nP = 0;
	Vector3f color = Vector3f(1,1,1);
	Vector3f vel = Vector3f(0,0,0);
	float dx = (cmax[0]-cmin[0])-2*h;
	float dy = (cmax[1]-cmin[1])-2*h;
	//printf("dx:%f dy:%f\n",dx,dy);
	for(float x=cmin[0]+h;x<=cmax[0]-h;x+=0.4*h*dx){
		for(float y=cmin[1]+h;y<=cmax[1]-h;y+=0.4*h*dy){
            float roulette = drand48();
            if(roulette<=this->intensity)
            {
				addParticle(Vector3f(x,y,0),vel);
				posP[4*nP] = x;
				posP[(4*nP)+1] = y;
				posP[(4*nP)+2] = 0;
				posP[(4*nP)+3] = h;
				colorsP[4*nP] = color[0];
				colorsP[(4*nP)+1] = color[1];
				colorsP[(4*nP)+2] = color[2];
				colorsP[(4*nP)+3] = 1;
				nP++;
            }
		}
	}
	cout << "Nb particles: " << nP << endl;
}

/****************************************************************************/
void SPH2D_BROWNIAN::computeForces()
{
	#pragma omp parallel for
	for(unsigned int i=0;i<particles.size();i++)
	{
		Vector3d fP(0,0,0),fV(0,0,0),N(0,0,0);
		float fS = 0;

		Vector3f pos1 = particles[i]->getPos(); pos1[2] = 0;
		Vector3f vel1 = particles[i]->getVel(); vel1[2] = 0;
		float rho1 = particles[i]->getRho();
		float p1 = particles[i]->getP();
		float m1 = particles[i]->getMass();
		float pV1 = m1/rho1;

		//printf("nbVois: %d\n",particles[i]->getNbVois());

		for(int j=0;j<particles[i]->getNbVois();j++){
			int index = particles[i]->getVois(j);
			Vector3f pos2 = particles[index]->getPos(); pos2[2] = 0;
			Vector3f P1P2 = pos1 - pos2;
			float d = P1P2.norm();

			float rho2 = particles[index]->getRho();
			float p2 = particles[index]->getP();
			float h2 = particles[index]->getRadius();
			float m2 = particles[index]->getMass();
			float pV2 = m2/rho2;
			Vector3f vel2 = particles[index]->getVel(); vel2[2] = 0;
			
			if(d>0) {
				// Pressure force
				Vector3d WP;
				WP[0] = P1P2[0]/d;
				WP[1] = P1P2[1]/d;
				WP[2] = 0;
				fP += (-45*powf(h-d,2)/(M_PI*powf(h,6)))*0.5*pV2*(p1+p2)*WP;

                // Viscosity force
				double WV  = (45*(h-d)/(M_PI*powf(h,6)))*m2/rho2;
				fV[0] += (vel2[0]-vel1[0])*WV;
				fV[1] += (vel2[1]-vel1[1])*WV;
				fV[2] = 0;
				// Surface tension force
				double WS = -(945/(32*M_PI*powf(h,9)))*((h*h)-(d*d))*((3*h*h)-(7*d*d));
				fS += WS*(m2/rho2);
	
				// normale evaluation
				double WN = (-945/(32*M_PI*powf(h,9)))*(m2/rho2)*powf(h*h-d*d,2);
				N[0] += (pos1[0]-pos2[0])*WN;
				N[1] += (pos1[1]-pos2[1])*WN;
				N[2] = 0;
			}
		}
		Vector3d forces = -fP + mu*fV;
		//forces[1] += gravity[1]*rho1;  
		float lN = N.norm();
		//if(lN>=ltS)
		//	forces += -fS*tS*N/lN;
			// External forces
	//	if(first){
		for(unsigned int i=0;i<externalForces.size();i++){
			forces += externalForces[i]*rho1*externalForces[i].norm();
		//	printf("externes:%f %f %f\n", externalForces[i].x()*rho1,externalForces[i].y()*rho1,0);
		}
	//	first = false;
	//	}

		particles[i]->setForces(forces);
		
	}
}

/****************************************************************************/
/****************************************************************************/

