/****************************************************************************/
/****************************************************************************/
#ifndef _SPH_PARTICLE_H__
#define _SPH_PARTICLE_H__

#include <particle.h>

class SPHParticle : public Particle
{
	public:

		SPHParticle();
		SPHParticle(Vector3f pos, Vector3f vel, float mass, float radius);
		SPHParticle(const SPHParticle &P);
		~SPHParticle();

		float    getRadius();
		float    getRho();
		float    getP();
		int      getVois(int index);
		int 	 getNbVois();

		void     setRadius(float);
		void     setRho(float);
		void     setP(float);
		void     setVois(int);
		void     clearVois();

		/*************** INTEGRATE *************************/
		virtual void  integrate(float dt);

	protected:

		float radius, rho, p;
		vector<int> vois;
};
#endif
