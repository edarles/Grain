#include <sph2d.h>
#include <iostream>
using namespace std;

/****************************************************************************/
/****************************************************************************/
SPH2D::SPH2D():SPH()
{
}
/****************************************************************************/
/****************************************************************************/
SPH2D::SPH2D(Vector3f origin, Vector3f min, Vector3f max):SPH()
{	
	Vector3f hV = Vector3f(2*h,2*h,0);
	minG = Vector3f(min[0]-2*h, min[1]-2*h,0);
	maxG = Vector3f(max[0]+2*h, max[1]+2*h,0);
	gridSPH = new GridSPH(minG,maxG,2*h,2*h,0); 
	particles.clear();
	generateParticles(minG,maxG);
}
/****************************************************************************/
SPH2D::~SPH2D()
{
}
/****************************************************************************/
void SPH2D::generateParticles(Vector3f cmin, Vector3f cmax)
{
	nP = 0;
	Vector3f color = Vector3f(1,1,1);
	Vector3f vel = Vector3f(0,0,0);

	for(float x=cmin[0]+h;x<cmax[0]-h;x+=0.5*h){
		for(float y=cmin[1]+h;y<cmax[1]-h;y+=0.5*h){
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
	cout << "Nb particles: " << nP << endl;
}
/****************************************************************************/
/****************************************************************************/
void SPH2D::update(float dt)
{
	if(particles.size()>0){
		// Boucle de simulation
		if(computeNeighborhood()){
			computeRhoP();
			computeForces();
			integrate(dt);
			collision(dt);
		}
	}
}
/****************************************************************************/
bool SPH2D::computeNeighborhood()
{
	if(gridSPH)
	{
		gridSPH->computeNeighborhood2D(particles);
		return true;
	}
	return false;
}
/****************************************************************************/
void SPH2D::computeForces()
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
		if(lN>=ltS)
			forces += -fS*tS*N/lN;

		// External forces
		printf("nbFX:%d\n",externalForces.size());
		for(unsigned int i=0;i<externalForces.size();i++){
			forces += externalForces[i]*rho1*rho1;
			printf("externes:%f %f %f\n", externalForces[i].x()*rho1*rho1,externalForces[i].y()*rho1*rho1,0);
		}
			

		particles[i]->setForces(forces);
		
	}
}
/****************************************************************************/
void SPH2D::integrate(float dt)
{
	#pragma omp parallel for
	for(int i=0;i<particles.size();i++){
		particles[i]->integrate(dt);
		Vector3f pos = particles[i]->getPos();
		posP[4*i] = pos[0];
		posP[(4*i)+1] = pos[1];
		posP[(4*i)+2] = 0;
	}
}
/****************************************************************************/
void SPH2D::collision(float dt)
{
	float elast = 0.5;
	float fric = 0.1;
	#pragma omp parallel for
	for(unsigned int i=0;i<particles.size();i++){

		// Detection
		Vector3f pos = particles[i]->getPos();
		Vector3f vel = particles[i]->getVel();
		bool collide = false;
		Vector3f N(0,0,0);

		if(pos[0]<minG[0]+h) {
			pos[0] = minG[0]+h; 
			collide = true;
			N = Vector3f(1,0,0);
		}
		if(pos[0]>maxG[0]-h) {
			pos[0] = maxG[0]-h;
			collide = true;
			N = Vector3f(-1,0,0);
		}
		if(pos[1]<minG[1]+h) {
			pos[1] = minG[1]+h;
			collide = true;
			N = Vector3f(0,1,0);
		} 
		if(pos[1]>maxG[1]-h) { 
			pos[1] = maxG[1]-h;
			collide = true;
			N = Vector3f(0,-1,0);
		}
		particles[i]->setPos(pos);
		if(collide)
		{
			Vector3f vN = vel.dot(N)*N;
			Vector3f vT = vel - vN;
			vel = fric*vT-elast*vN;
		}
		particles[i]->setVel(vel);
		
		posP[4*i] = pos[0];
		posP[(4*i)+1] = pos[1];
		posP[(4*i)+2] = pos[2];
	}
}
/****************************************************************************/
/****************************************************************************/

