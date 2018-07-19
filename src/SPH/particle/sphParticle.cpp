#include <sphParticle.h>
/****************************************************************************/
/****************************************************************************/
SPHParticle::SPHParticle():Particle()
{
	rho = p = radius = 0;
	color = Vector3f(0,0,1);
	vois.clear();
}
/****************************************************************************/
SPHParticle::SPHParticle(Vector3f pos, Vector3f vel, float mass, float radius):Particle(pos,vel,mass)
{
	this->radius = radius;
	this->rho = 0;
	this->p = 0;
	this->color = Vector3f(0,0,1);
	vois.clear();
}
/****************************************************************************/
SPHParticle::SPHParticle(const SPHParticle &P)
{
	pos = P.pos;
	vel = P.vel;
	forces =  P.forces;
	mass = P.mass;
	radius = P.radius;
	rho = P.rho;
	p = P.p;
	color = P.color;
	vois.clear();
	for(int i=0;i<P.vois.size();i++)
		vois.push_back(P.vois[i]);
}
/****************************************************************************/
SPHParticle::~SPHParticle()
{
	vois.clear();
	vector<int>().swap(vois);
}
/****************************************************************************/
/****************************************************************************/
float SPHParticle::getRadius()
{
	return radius;
}
/****************************************************************************/
float SPHParticle::getRho()
{
	return rho;
}
/****************************************************************************/
float SPHParticle::getP()
{
	return p;
}
/****************************************************************************/
int SPHParticle::getVois(int index)
{
	return vois[index];
}
/****************************************************************************/
int SPHParticle::getNbVois()
{
	return vois.size();
}
/****************************************************************************/
/****************************************************************************/
void SPHParticle::setRadius(float radius)
{
	this->radius = radius;
}
/****************************************************************************/
void SPHParticle::setRho(float rho)
{
	this->rho = rho;
}
/****************************************************************************/
void SPHParticle::setP(float p)
{
	this->p = p;
}
/****************************************************************************/
void SPHParticle::setVois(int vois)
{
	this->vois.push_back(vois);
}
/****************************************************************************/
void SPHParticle::clearVois()
{
	this->vois.clear();
	vector<int>().swap(this->vois);
	this->vois.shrink_to_fit();
}
/****************************************************************************/
/****************************************************************************/
void SPHParticle::integrate(float dt)
{
	if(this->rho!=0)
	{
		//printf("forces:%f %f %f\n",forces[0]/rho,forces[1]/rho,forces[2]/rho);
		vel[0] = vel[0] + (forces[0]/rho)*dt;
		vel[1] = vel[1] + (forces[1]/rho)*dt;
		vel[2] = vel[2] + (forces[2]/rho)*dt;
		pos = pos + vel*dt;
	//	ev = 0.5*(ev + vel);
	}
}
/****************************************************************************/
/****************************************************************************/