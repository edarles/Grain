/****************************************************************************/
/****************************************************************************/
#ifndef _SPH_H__
#define _SPH_H__

#include <sphParticle.h>
#include <gridSph.h>

class SPH
{
/****************************************************************************/
	public : 

		SPH();
		SPH(Vector3f center, Vector3f min, Vector3f max);
		virtual ~SPH();

		/**************** GETTERS ***************************/		
		SPHParticle* 	      getParticle(int index) const;
		vector<SPHParticle*>  getParticles() const;
		int        	          getNbParticles() const;
		GridSPH*    	      getGrid() const;
		float 				  getMass() const;
		float 				  getH() const;
		float 	   	   		  getRho0() const;
		float 	   	   		  getMu() const;
		float 	   	   		  getTS() const;
		float 	   	   		  getLTS() const; 

		/**************** ROUTINE AJOUT / SUPRESSION PARTICULES  *****/
		virtual void addParticle(Vector3f pos, Vector3f vel);
		virtual void addParticle(SPHParticle *P2);
		virtual void deleteParticle(int index);

		/**************** ROUTINE GENERATION PARTICULES  *****/
		virtual void generateParticles(Vector3f cmin, Vector3f cmax);
		
		/**************** ROUTINE POUR AJOUT FORCE EXTERIEUR **/
		virtual void addExternalForce(Vector3d force);

		/**************** ROUTINES DU SOLVER ****************/
		virtual void update(float dt);

		/**************** ROUTINE D'AFFICHAGE ***************/
		virtual void display();


/****************************************************************************/
	protected : 

		// Grille de voisinage
		GridSPH *gridSPH;

		Vector3f minG, maxG;

		// Statics attributes
		static const float m;
		float h;
		static const float rho0;
		static const float mu;
		static const float tS;
		static const float ltS;
		static const Vector3f gravity;
		
		// Statics members for sphere shader
		static GLuint m_program ;
		static float* posP;
		static float* colorsP;
		static int    nP;

		vector<SPHParticle*> particles;
	
		// Protected Methods
		virtual bool computeNeighborhood();
		virtual void computeRhoP() = 0;
		virtual void computeForces();
		virtual void integrate(float dt);
		virtual void collision(float dt);

		virtual void displayByField();
		Vector3f     convertHsvToRgb(Vector3d Hsv);

		vector<Vector3d> externalForces;
};
#endif
/****************************************************************************/
/****************************************************************************/
