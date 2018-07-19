#include <gridSph.h>
#include <sphParticle.h>
#include <iostream>

/****************************************************************************/
/****************************************************************************/
GridSPH::GridSPH():GridFluid(Vector3f(-1,-1,-1),Vector3f(1,1,1),0.05,0.05,0.05)
{
	init();
}
/****************************************************************************/
GridSPH::GridSPH(Vector3f min, Vector3f max, float dx, float dy, float dz):GridFluid(min,max,dx,dy,dz)
{
	init();
}
/****************************************************************************/
GridSPH::GridSPH(Vector3f min, Vector3f max, float dx, float dy, float dz, float intensity)
:GridFluid(min,max,dx,dy,dz,intensity)
{
	init();
}
/****************************************************************************/
GridSPH::~GridSPH()
{
}
/*************************************************/
/*************************************************/
void GridSPH::computeNeighborhood(vector<SPHParticle*> particles)
{
	vector< vector<int> > indexs;

	// Store particles into cells
	for(int ix=0; ix<m_nx; ix++){
		 for(int iy=0; iy<m_ny; iy++){
			for(int iz=0; iz<m_nz; iz++){
				int indexCell = ix + iy*m_nx + iz*m_nx*m_ny;
				indexs.push_back(vector<int>());
			}
		}
	}

	for(unsigned int i=0;i<particles.size();i++){
		Vector3f pos = particles[i]->getPos();
	  	int iC = (int) floor((double)(pos[0]-m_min[0])/m_dx);
	  	int jC = (int) floor((double)(pos[1]-m_min[1])/m_dy);
	  	int kC = (int) floor((double)(pos[2]-m_min[2])/m_dz);
	  	int indexCell = iC + jC*m_nx + kC*m_nx*m_ny;
		if(indexCell>=0 && indexCell<m_n){
			indexs[indexCell].push_back(i);
		}
	}
	// Compute neighboorhood for each particle
	for(unsigned int iP=0;iP<particles.size();iP++)
	{
		particles[iP]->clearVois();	
		Vector3f pos = particles[iP]->getPos();


		// à rajouter dans omega
		//particles[iP]->setVois(iP);
		//***********************
		float h1 = particles[iP]->getRadius();
		float x = pos[0]; 
		float y = pos[1];
		float z = pos[2];

		int ix = (int)floor((double)(x-m_min[0])/m_dx);
		int iy = (int)floor((double)(y-m_min[1])/m_dy);
		int iz = (int)floor((double)(z-m_min[2])/m_dz);

	    int nbcx = floor(h1/m_dx);
	    int nbcy = floor(h1/m_dy);
	    int nbcz = floor(h1/m_dz);
	    int nbP = 0;

		if(ix>=0 && iy>=0 && iz>=0 && ix < m_nx && iy < m_ny && iz < m_nz){

			for(int i=ix-nbcx; i<=ix+nbcx; i++){
				for(int j=iy-nbcy; j<=iy+nbcy; j++){
					for(int k=iz-nbcz; k<=iz+nbcz; k++){
						 if(i>=0 && j>=0 && k>=0 && i<m_nx && j<m_ny && k<m_nz){

							int indexCell = i + j*m_nx + k*m_nx*m_ny;
							
							if(indexCell>=0 && indexCell<m_n){
								if(indexs[indexCell].size()>0){
						 		for(unsigned int n=0;n<indexs[indexCell].size();n++)
						 		{
									int iP2 = indexs[indexCell][n];
									if(iP2>particles.size())
										cout << "Pb: index: " << iP2 << " size: " << particles.size() << endl;
									else {
										Vector3f pos2 = particles[iP2]->getPos();
										Vector3f PP = pos - pos2;
										float h2 = particles[iP2]->getRadius();
										float r = max(h1,h2);
										if(PP.norm()<=r)
											particles[iP]->setVois(iP2);
									}
								}
							}
							}
						}
					}
				}
			}
		}
	}
	for(unsigned int i=0;i<indexs.size();i++){
		indexs[i].clear();
		vector<int>().swap(indexs[i]);
		indexs[i].shrink_to_fit();
	}
	indexs.clear();
	vector<vector<int>>().swap(indexs);
	indexs.shrink_to_fit();
}
/****************************************************************************/
/****************************************************************************/
void GridSPH::computeNeighborhood2D(vector<SPHParticle*> particles)
{
	vector< vector<int> > indexs;

	// Store particles into cells
	for(int ix=0; ix<=m_nx; ix++){
		 for(int iy=0; iy<=m_ny; iy++){
				int indexCell = ix + iy*m_nx;
				indexs.push_back(vector<int>());
			}
		}

	for(unsigned int i=0;i<particles.size();i++){
		Vector3f pos = particles[i]->getPos();
	  	int iC = (int) floor((double)(pos[0]-m_min[0])/m_dx);
	  	int jC = (int) floor((double)(pos[1]-m_min[1])/m_dy);
	  	int indexCell = iC + jC*m_nx ;
		if(indexCell>=0 && indexCell<=m_n){
			indexs[indexCell].push_back(i);
		}
	}
	// Compute neighboorhood for each particle
	for(unsigned int iP=0;iP<particles.size();iP++){

		particles[iP]->clearVois();
		
		Vector3f pos = particles[iP]->getPos();
		
		//particles[iP]->setVois(iP);
	
		float h1 = particles[iP]->getRadius();
		float x = pos[0]; 
		float y = pos[1];
	
		int ix = (int)floor((double)(x-m_min[0])/m_dx);
		int iy = (int)floor((double)(y-m_min[1])/m_dy);
	
	    int nbcx = floor(h1/m_dx)+1;
	    int nbcy = floor(h1/m_dy)+1;
	    int nbP = 0;

		if(ix>=0 && iy>=0 && ix <= m_nx && iy <= m_ny){

			for(int i=ix-nbcx; i<=ix+nbcx; i++){
				for(int j=iy-nbcy; j<=iy+nbcy; j++){
						 if(i>=0 && j>=0 && i<=m_nx && j<=m_ny ){

							int indexCell = i + j*m_nx ;
							
							if(indexCell>=0 && indexCell<=m_n){
								if(indexs[indexCell].size()>0){
						 		for(unsigned int n=0;n<indexs[indexCell].size();n++)
						 		{
									int iP2 = indexs[indexCell][n];
									if(iP2>particles.size())
										cout << "Pb: index: " << iP2 << " size: " << particles.size() << endl;
									else {
										Vector3f pos2 = particles[iP2]->getPos();
										Vector3f PP = pos - pos2;
										float h2 = particles[iP2]->getRadius();
										float r = max(h1,h2);
										if(PP.norm()<=r)
											particles[iP]->setVois(iP2);
									}
								}
							}
							}
						}
					}
				}
	}
	}
	for(unsigned int i=0;i<indexs.size();i++){
		indexs[i].clear();
		vector<int>().swap(indexs[i]);
		indexs[i].shrink_to_fit();
	}
	indexs.clear();
	vector<vector<int>>().swap(indexs);
	indexs.shrink_to_fit();
}
/****************************************************************************/
/****************************************************************************/