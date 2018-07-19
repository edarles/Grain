/****************************************************************************/
/****************************************************************************/
#ifndef _PARTICLE_H__
#define _PARTICLE_H__

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;
using namespace std;

class Particle
{
	public:

		Particle();
		Particle(Vector3f pos, Vector3f vel, float mass);
		Particle(const Particle &P);
		~Particle();

		Vector3f getPos();
		Vector3f getVel();; 
		Vector3f getEv();
		Vector3d getForces();
        Vector3f getColor();
		float 	 getMass();

		void     setPos(Vector3f pos);
		void     setVel(Vector3f vel);
		void     setEv(Vector3f ev);
		void     setForces(Vector3d forces);
        void     setColor(Vector3f color);
		void 	 setMass(float mass);
        void     addForce(Vector3d);

         /*************** INTEGRATE *************************/
		virtual void  integrate(float dt) = 0;

	protected:

		Vector3f pos, vel, ev, color;
		Vector3d forces;
		float mass;

};
#endif
