/****************************************************************************/
/****************************************************************************/
#ifndef _GRID_FLUID_H__
#define _GRID_FLUID_H__

#include <Eigen/Dense>
#include <vector>
#include <GL/gl.h>
using namespace Eigen;
using namespace std;

#pragma once
#define INF 1000000007

class GridFluid
{
/****************************************************************************/
	public : 

		GridFluid();
		GridFluid(Vector3f min, Vector3f max, float dx, float dy, float dz);
		GridFluid(Vector3f min, Vector3f max, float dx, float dy, float dz, float intensity);
		virtual ~GridFluid();

		/**************** GETTERS ***************************/
		Vector3f getCenter();
		Vector3f getMin() ;
		Vector3f getMax();
		float    getDx();
		float    getDy();
		float    getDz();
		int      getNx();
		int      getNy();
		int      getNz();
		int      getMn();
	
		Vector3f getPos(int ix, int iy, int iz);
		Vector3f getPos(int indexCell);
		Vector4f getColor();
		
		/**************** SETTERS ***************************/
		void     setCenter(Vector3f center);
		void     setMin(Vector3f min);
		void     setMax(Vector3f max);
		void     setDx(float dx);
		void     setDy(float dy);
		void     setDz(float dz);
		void     setNx(int nx);
		void     setNy(int ny);
		void     setNz(int nz);
		void     setPos(int ix, int iy, int iz, Vector3f pos);
		void	 setColor(Vector4f color);
		
		/**************** ROUTINE DE CREATION ***************************/
		virtual void init();
		virtual void createGrid();

		/**************** ROUTINE D'AFFICHAGE ***************************/
		virtual void display();
		void displayBorders();

	protected :

		 Vector3f m_center, m_min, m_max;
		 float m_dx, m_dy, m_dz;
		 float m_sizeX, m_sizeY, m_sizeZ;
		 int m_nx, m_ny, m_nz, m_n;
		 vector<Vector3f> m_pos;

		 Vector4f color;

		 static GLuint m_program;
};
#endif
/****************************************************************************/
/****************************************************************************/
