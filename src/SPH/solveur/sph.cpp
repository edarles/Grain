#include "../../utils.h"
#include "../../shaders.h"
#include <sph_shader.h>
#include <sph.h>
#include <iostream>

/****************************************************************************/
/****************************************************************************/
const float SPH::m = 0.5f;//0.02
const float SPH::rho0 = 998.29f;
const float SPH::mu = 3.5f;
const float SPH::tS = 0.0728f;
const float SPH::ltS = 7.065f;
const Vector3f SPH::gravity = Vector3f(0,-9.81f,0);

/****************************************************************************/
/****************************************************************************/
GLuint SPH::m_program = 0;
float* SPH::posP = new float[4*MAX_PARTICLES];
float* SPH::colorsP = new float[4*MAX_PARTICLES];
int    SPH::nP = 0;

/****************************************************************************/
/****************************************************************************/
SPH::SPH()
{
	h=pow(60*m/(4*M_PI*rho0),1.0/3);
//	if(m_program==0) m_program = _compileProgram(sphereVertexShader, spherePixelShader);
}
/****************************************************************************/
SPH::SPH(Vector3f origin, Vector3f min, Vector3f max)
{
	h=pow(60*m/(4*M_PI*rho0),1.0/3);
//	if(m_program==0) m_program = _compileProgram(sphereVertexShader, spherePixelShader);	
	Vector3f hV = Vector3f(h,h,h);
	minG = min - hV;
	maxG = max + hV;
	gridSPH = new GridSPH(minG,maxG,h,h,h); 
	particles.clear();
	generateParticles(minG,maxG);
}
/****************************************************************************/
SPH::~SPH()
{
	delete gridSPH; 
	for(unsigned int i=0;i<particles.size();i++)
		delete particles[i];
	particles.clear();
	vector<SPHParticle*>().swap(particles);
	particles.shrink_to_fit();
}
/****************************************************************************/
void SPH::addParticle(Vector3f pos, Vector3f vel)
{
	if(particles.size()<MAX_PARTICLES){
		SPHParticle *P = new SPHParticle(pos,vel,m,h);
		particles.push_back(P);
	}
}
/****************************************************************************/
void SPH::addParticle(SPHParticle *P2)
{
	if(particles.size()<MAX_PARTICLES){
		SPHParticle *P = new SPHParticle((*P2));
		particles.push_back(P);
	}
}
/****************************************************************************/
void SPH::deleteParticle(int index)
{
	delete particles[index];
	particles.erase(particles.begin()+index);
	particles.shrink_to_fit();
}
/****************************************************************************/
void SPH::addExternalForce(Vector3d force)
{
	externalForces.push_back(force);
}
/****************************************************************************/
void SPH::generateParticles(Vector3f cmin, Vector3f cmax)
{
	nP = 0;
	Vector3f color = Vector3f(0,0,1);
	Vector3f vel = Vector3f(0,0,0);

	for(float x=cmin[0];x<=cmax[0];x+=h){
		for(float y=cmin[1];y<=cmax[1]/2;y+=h){
			for(float z=cmin[2];z<=cmax[2];z+=h){
				addParticle(Vector3f(x,y,z),vel);
				posP[4*nP] = x;
				posP[(4*nP)+1] = y;
				posP[(4*nP)+2] = z;
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
/****************************************************************************/
void SPH::update(float dt)
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
bool SPH::computeNeighborhood()
{
	if(gridSPH)
	{
		gridSPH->computeNeighborhood(particles);
		return true;
	}
	return false;
}
/****************************************************************************/
void SPH::computeForces()
{
	#pragma omp parallel for
	for(unsigned int i=0;i<particles.size();i++)
	{
		Vector3d fP(0,0,0),fV(0,0,0),N(0,0,0);
		float fS = 0;

		Vector3f pos1 = particles[i]->getPos();
		Vector3f vel1 = particles[i]->getVel();
		float rho1 = particles[i]->getRho();
		float p1 = particles[i]->getP();
		float pV1 = p1/(rho1*rho1);

		//printf("nbVois: %d\n",particles[i]->getNbVois());

		for(int j=0;j<particles[i]->getNbVois();j++){
			int index = particles[i]->getVois(j);
			Vector3f pos2 = particles[index]->getPos();
			Vector3f P1P2 = pos2 - pos1;
			float d = P1P2.norm();

			float rho2 = particles[index]->getRho();
			float p2 = particles[index]->getP();
			float h2 = particles[index]->getRadius();
			float m2 = particles[index]->getMass();
			float pV2 = p2/(rho2*rho2);
			Vector3f vel2 = particles[index]->getVel();
			
			if(d>0) {
				// Pressure force
				Vector3d WP;
				WP[0] = P1P2[0]/d;
				WP[1] = P1P2[1]/d;
				WP[2] = P1P2[2]/d;
				fP += (-45*powf(h-d,2)/(M_PI*powf(h,6)))*m2*(pV1+pV2)*WP;

                // Viscosity force
				double WV  = (45*(h-d)/(M_PI*powf(h,6)))*m2/rho2;
				fV[0] += (vel2[0]-vel1[0])*WV;
				fV[1] += (vel2[1]-vel1[1])*WV;
				fV[2] += (vel2[2]-vel1[2])*WV;
			
				// Surface tension force
				double WS = -(945/(32*M_PI*powf(h,9)))*((h*h)-(d*d))*((3*h*h)-(7*d*d));
				fS += WS*(m2/rho2);
	
				// normale evaluation
				double WN = (-945/(32*M_PI*powf(h,9)))*(m2/rho2)*powf(h*h-d*d,2);
				N[0] += (pos1[0]-pos2[0])*WN;
				N[1] += (pos1[1]-pos2[1])*WN;
				N[2] += (pos1[2]-pos2[2])*WN;
			}
		}
		Vector3d forces = -fP*rho1 + mu*fV;
		forces[1] += gravity[1]*rho1;  
		float lN = N.norm();
		if(lN>=ltS)
			forces += -fS*tS*N/lN;

		for(unsigned int i=0;i<externalForces.size();i++)
			forces += externalForces[i]*rho1*rho1;

		particles[i]->setForces(forces);
		
	}
}
/****************************************************************************/
void SPH::integrate(float dt)
{
	#pragma omp parallel for
	for(int i=0;i<particles.size();i++){
		particles[i]->integrate(dt);
		Vector3f pos = particles[i]->getPos();
		posP[4*i] = pos[0];
		posP[(4*i)+1] = pos[1];
		posP[(4*i)+2] = pos[2];
	}
}
/****************************************************************************/
void SPH::collision(float dt)
{
	float elast = 0.1;
	float fric = 0.1;
	#pragma omp parallel for
	for(unsigned int i=0;i<particles.size();i++){

		// Detection
		Vector3f pos = particles[i]->getPos();
		Vector3f vel = particles[i]->getVel();
	
		if(pos[0]<minG[0]+h) {
			pos[0] = minG[0]+h; 
			vel[0] = -vel[0]*elast + vel[0]*fric;
		}
		if(pos[0]>maxG[0]-h) {
			pos[0] = maxG[0]-h;
			vel[0] = -vel[0]*elast + vel[0]*fric;
		}
		if(pos[1]<minG[1]+h) {
			pos[1] = minG[1]+h;
			vel[1] = -vel[1]*elast + vel[1]*fric;
		} 
		if(pos[1]>maxG[1]-h) { 
			pos[1] = maxG[1]-h;
			vel[1] = -vel[1]*elast + vel[1]*fric;
		}
		if(pos[2]<minG[2]+h) {
			pos[2] = minG[2]+h;
			vel[2] = -vel[2]*elast + vel[2]*fric;
		}
		if(pos[2]>maxG[2]-h) {
			pos[2] = maxG[2]-h;
			vel[2] = -vel[2]*elast + vel[2]*fric;
		}
		particles[i]->setPos(pos);
		particles[i]->setVel(vel);
		
		posP[4*i] = pos[0];
		posP[(4*i)+1] = pos[1];
		posP[(4*i)+2] = pos[2];
	}
}
/****************************************************************************/
void SPH::display()
{
	//glClearColor(0,0,0,1);
	// Affichage des SPH
	/*if(DISPLAY_MODE != 0){
		displayByField();
		displaySpheres(m_program,posP,colorsP,nP);
	}
	else {*/
	glPointSize(m*10);
	glBegin(GL_POINTS);
	for(unsigned i=0;i<particles.size();i++){
		SPHParticle *p = particles[i];
		Vector3f pos = p->getPos();
		glVertex3f(pos[0],pos[1],pos[2]);
	}
	glEnd();
	//}	
}
/****************************************************************************/
/****************************************************************************/
void SPH::displayByField()
{
    // DISPLAY BY MASS FIELD
    if(DISPLAY_MODE == 1){	
		float mmax, mmin;
		mmax = mmin = 0;
		#pragma omp parallel for
		for(unsigned i=0;i<particles.size();i++){
			SPHParticle *p = particles[i];
			if(mmax<=p->getMass())
				mmax = p->getMass();
		}
		if(mmin<mmax){
		#pragma omp parallel for
    	for(unsigned i=0;i<particles.size();i++){
			SPHParticle *p = particles[i];
			float m = p->getMass();
			float hue = 240 * (mmax - fmin(m,mmax)) / (mmax-mmin);
			Vector3d Hsv(hue,1,1);
			p->setColor(convertHsvToRgb(Hsv));
			Vector3f color = p->getColor();
			colorsP[4*i] = color[0];
			colorsP[4*i+1] = color[1];
			colorsP[4*i+2] = color[2];
		}
	}
   }

   //DISPLAY BY VELOCITY FIELD
   if(DISPLAY_MODE == 2){
		float vmax, vmin;
		vmax = 0; vmin = 1000000;
		#pragma omp parallel for
		for(unsigned i=0;i<particles.size();i++){
			float length = particles[i]->getVel().norm();
			if(vmax<length) vmax = length;
			if(vmin>length) vmin = length;
		}
		if(vmin<vmax){
		#pragma omp parallel for
    	for(unsigned i=0;i<particles.size();i++){
			SPHParticle *p = particles[i];
			float length = p->getVel().norm();
			float hue = 240 * (vmax - fmin(length,vmax)) / (vmax-vmin);
			Vector3d Hsv(hue,1,1);
			p->setColor(convertHsvToRgb(Hsv));
			Vector3f color = p->getColor();
			colorsP[4*i] = color[0];
			colorsP[4*i+1] = color[1];
			colorsP[4*i+2] = color[2];
		}
    	}
   }
}
/****************************************************************************/
/****************************************************************************/
Vector3f SPH::convertHsvToRgb(Vector3d Hsv)
{
   Vector3f result;
   int t = (int)(floor((double)(Hsv[0]/60)))%6;
   double f = (Hsv[0]/60)-t;
   double l = Hsv[2]*(1-Hsv[1]);
   double m = Hsv[2]*(1-f*Hsv[1]);
   double n = Hsv[2]*(1-(1-f)*Hsv[1]);
   if(t==0) {result[0] = Hsv[2]; result[1] = n; result[2] = l;};
   if(t==1) {result[0] = m; result[1] = Hsv[2]; result[2] = l;};
   if(t==2) {result[0] = l; result[1] = Hsv[2]; result[2] = n;};
   if(t==3) {result[0] = l; result[1] = m; result[2] = Hsv[2];};
   if(t==4) {result[0] = n; result[1] = l; result[2] = Hsv[2];};
   if(t==5) {result[0] = Hsv[2]; result[1] = l; result[2] = m;};
   return result;
}
/****************************************************************************/
/****************************************************************************/
float SPH::getMass() const
{
	return m;
}
/****************************************************************************/
float SPH::getH() const
{
	return h;
}
/****************************************************************************/
float SPH::getRho0() const
{
	return rho0;
}
/****************************************************************************/
float SPH::getMu() const
{
	return mu;
}
/****************************************************************************/
float SPH::getTS() const
{
	return tS;
}
/****************************************************************************/
float SPH::getLTS() const
{
	return ltS;
}
/****************************************************************************/
int  SPH::getNbParticles() const
{
	return particles.size();
}
/****************************************************************************/
GridSPH* SPH::getGrid() const
{
	return gridSPH;
}
/****************************************************************************/
SPHParticle* SPH::getParticle(int index) const
{
	assert (index < (int)particles.size());
	return particles[index];
}
/****************************************************************************/
vector<SPHParticle*> SPH::getParticles() const
{
	return particles;
}
/****************************************************************************/
/****************************************************************************/





