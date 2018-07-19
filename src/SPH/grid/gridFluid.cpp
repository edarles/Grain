#include <utils.h>
#include <shaders.h>

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <gridFluid.h>
#include <GL/gl.h>

using namespace std;

GLuint GridFluid::m_program = 0;
/****************************************************************************/
/****************************************************************************/
GridFluid::GridFluid()
{
	m_center << 0,0,0;
	m_min << -1.0,-1.0,-1.0;
	m_max << 1.0,1.0,1.0;
	m_dx = m_dy = m_dz = 0.01;
	m_pos.clear();
	createGrid();
	color << 1,1,1,1;
}
/****************************************************************************/
GridFluid::GridFluid(Vector3f min, Vector3f max, float dx, float dy, float dz)
{
	this->m_center = Vector3f((max[0]+min[0])/2,(max[1]+min[1])/2,(max[2]+min[2])/2);
	this->m_min = min;
	this->m_max = max;
	this->m_dx = dx;
	this->m_dy = dy;
	this->m_dz = dz;
	m_pos.clear();
	createGrid();
	color << 1,1,1,1;
}
/****************************************************************************/
GridFluid::GridFluid(Vector3f min, Vector3f max, float dx, float dy, float dz, float intensity)
{
	this->m_center = Vector3f((max[0]+min[0])/2,(max[1]+min[1])/2,(max[2]+min[2])/2);
	this->m_min = min;
	this->m_max = max;
	this->m_dx = dx;
	this->m_dy = dy;
	this->m_dz = dz;
	m_pos.clear();
	createGrid();
	color=Vector4f(intensity,intensity,intensity,0.3);
}
/****************************************************************************/
GridFluid::~GridFluid()
{
	m_pos.clear();
	vector<Vector3f>().swap(m_pos);
	m_pos.shrink_to_fit();
}
/*************************************************/
/*************************************************/
Vector3f GridFluid::getCenter()
{
	return m_center;
}
/*************************************************/
Vector3f GridFluid::getMin()
{
	return m_min;
}
/*************************************************/
Vector3f GridFluid::getMax()
{
	return m_max;
}
/*************************************************/
float    GridFluid::getDx()
{
	return m_dx;
}
/*************************************************/
float    GridFluid::getDy()
{
	return m_dy;
}
/*************************************************/
float    GridFluid::getDz()
{
	return m_dz;
}
/*************************************************/
int      GridFluid::getNx()
{
	return m_nx;
}
/*************************************************/
int      GridFluid::getNy()
{
	return m_ny;
}
/*************************************************/
int      GridFluid::getNz()
{
	return m_nz;
}
/*************************************************/
int      GridFluid::getMn()
{
	return m_n;
}
/*************************************************/
Vector3f GridFluid::getPos(int ix, int iy, int iz)
{
	assert(ix<m_nx && iy<m_ny && iz<m_nz);
	int indexCell = ix + iy*m_nx + iz*m_nx*m_ny;
	return m_pos[indexCell];
}
/*************************************************/
Vector3f GridFluid::getPos(int indexCell)
{
	assert(indexCell < m_n);
	return m_pos[indexCell];
}
/*************************************************/
Vector4f GridFluid::getColor()
{
	return color;
}
/*************************************************/
/*************************************************/
void GridFluid::setCenter(Vector3f center)
{
	m_center = center;
}
/*************************************************/
void GridFluid::setMin(Vector3f min)
{
	m_min = min;
}
/*************************************************/
void GridFluid::setMax(Vector3f max)
{
	m_max = max;
}
/*************************************************/
void GridFluid::setDx(float dx)
{
	m_dx = dx;
}
/*************************************************/
void GridFluid::setDy(float dy)
{
	m_dy = dy;
}
/*************************************************/
void GridFluid::setDz(float dz)
{
	m_dz = dz;
}
/*************************************************/
void GridFluid::setNx(int nx)
{
	m_nx = nx;
}
/*************************************************/
void GridFluid::setNy(int ny)
{
	m_ny = ny;
}
/*************************************************/
void GridFluid::setNz(int nz)
{
	m_nz = nz;
}
/*************************************************/
void GridFluid::setPos(int ix, int iy, int iz, Vector3f pos)
{
	int indexCell = ix + iy*m_nx + iz*m_nx*m_ny;
	m_pos[indexCell] = pos;
}
/*************************************************/
void GridFluid::setColor(Vector4f color)
{
	this->color = color;
}
/*************************************************/
void GridFluid::init()
{
}
/****************************************************************************/
/****************************************************************************/
void GridFluid::createGrid()
{
	m_pos.clear();
	vector<Vector3f>().swap(m_pos);

	float l = m_max[0]-m_min[0]; m_sizeX = l;
	float w = m_max[1]-m_min[1]; m_sizeY = w;
	float d = m_max[2]-m_min[2]; m_sizeZ = d;

	m_nx = ceil(l/m_dx); 
	m_ny = ceil(w/m_dy); 
	if(m_dz!=0) m_nz = ceil(d/m_dz); 
	else m_nz = 0;

	m_n = m_nx + m_nx*m_ny + m_nx*m_ny*m_nz;

	for(int i=0;i<=m_n;i++)
		m_pos.push_back(Vector3f(0,0,0));
		
	for(int ix=0; ix<=m_nx; ix++){
		for(int iy=0; iy<=m_ny; iy++){
				if(m_nz>0){
				for(int iz=0; iz<=m_nz; iz++){
					int indexCell = ix + iy*m_nx + iz*m_nx*m_ny;
					Vector3f pos(m_center[0]-(l/2)+(m_dx/2)+ix*m_dx,
								 m_center[1]-(w/2)+(m_dy/2)+iy*m_dy,
					             m_center[2]-(d/2)+(m_dz/2)+iz*m_dz);
					m_pos[indexCell] = pos;
				}
				}
				else
				{
					int indexCell = ix + iy*m_nx ;
					Vector3f pos(m_center[0]-(l/2)+(m_dx/2)+ix*m_dx,
								 m_center[1]-(w/2)+(m_dy/2)+iy*m_dy,
					             0);
					m_pos[indexCell] = pos;
				}
			}
		}
}

/****************************************************************************/
/****************************************************************************/
void GridFluid::display()
{
	if(m_nz>0)
	{
	glColor4f((float)color[0],(float)color[1],(float)color[2],(float)color[3]);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	for(int i=0;i<m_nx-1;i++){
		for(int j=0;j<m_nz-1;j++){
				for(int k=0;k<m_ny-1;k++){
					int indexs[8];
					indexs[0] = i + j*m_nx + k*m_nx*m_nz;
					indexs[1] = i + (j+1)*m_nx + k*m_nx*m_nz;
					indexs[2] = (i+1) + (j+1)*m_nx + k*m_nx*m_nz;
					indexs[3] = (i+1) + j*m_nx + k*m_nx*m_nz;

					indexs[4] = i + j*m_nx + (k+1)*m_nx*m_nz;
					indexs[5] = i + (j+1)*m_nx + (k+1)*m_nx*m_nz;
					indexs[6] = (i+1) + (j+1)*m_nx + (k+1)*m_nx*m_nz;
					indexs[7] = (i+1) + j*m_nx + (k+1)*m_nx*m_nz;

					glBegin(GL_QUADS);
					glVertex3f(m_pos[indexs[0]][0],m_pos[indexs[0]][1],m_pos[indexs[0]][2]);
					glVertex3f(m_pos[indexs[1]][0],m_pos[indexs[1]][1],m_pos[indexs[1]][2]);
					glVertex3f(m_pos[indexs[2]][0],m_pos[indexs[2]][1],m_pos[indexs[2]][2]);
					glVertex3f(m_pos[indexs[3]][0],m_pos[indexs[3]][1],m_pos[indexs[3]][2]);
					glEnd();

					glBegin(GL_QUADS);
					glVertex3f(m_pos[indexs[4]][0],m_pos[indexs[4]][1],m_pos[indexs[4]][2]);
					glVertex3f(m_pos[indexs[5]][0],m_pos[indexs[5]][1],m_pos[indexs[5]][2]);
					glVertex3f(m_pos[indexs[6]][0],m_pos[indexs[6]][1],m_pos[indexs[6]][2]);
					glVertex3f(m_pos[indexs[7]][0],m_pos[indexs[7]][1],m_pos[indexs[7]][2]);
					glEnd();

					glBegin(GL_QUADS);
					glVertex3f(m_pos[indexs[0]][0],m_pos[indexs[0]][1],m_pos[indexs[0]][2]);
					glVertex3f(m_pos[indexs[1]][0],m_pos[indexs[1]][1],m_pos[indexs[1]][2]);
					glVertex3f(m_pos[indexs[5]][0],m_pos[indexs[5]][1],m_pos[indexs[5]][2]);
					glVertex3f(m_pos[indexs[4]][0],m_pos[indexs[4]][1],m_pos[indexs[4]][2]);
					glEnd();

					glBegin(GL_QUADS);
					glVertex3f(m_pos[indexs[3]][0],m_pos[indexs[3]][1],m_pos[indexs[3]][2]);
					glVertex3f(m_pos[indexs[2]][0],m_pos[indexs[2]][1],m_pos[indexs[2]][2]);
					glVertex3f(m_pos[indexs[6]][0],m_pos[indexs[6]][1],m_pos[indexs[6]][2]);
					glVertex3f(m_pos[indexs[7]][0],m_pos[indexs[7]][1],m_pos[indexs[7]][2]);
					glEnd();

					glBegin(GL_QUADS);
					glVertex3f(m_pos[indexs[0]][0],m_pos[indexs[0]][1],m_pos[indexs[0]][2]);
					glVertex3f(m_pos[indexs[3]][0],m_pos[indexs[3]][1],m_pos[indexs[3]][2]);
					glVertex3f(m_pos[indexs[7]][0],m_pos[indexs[7]][1],m_pos[indexs[7]][2]);
					glVertex3f(m_pos[indexs[4]][0],m_pos[indexs[4]][1],m_pos[indexs[4]][2]);
					glEnd();

					glBegin(GL_QUADS);
					glVertex3f(m_pos[indexs[1]][0],m_pos[indexs[1]][1],m_pos[indexs[1]][2]);
					glVertex3f(m_pos[indexs[2]][0],m_pos[indexs[2]][1],m_pos[indexs[2]][2]);
					glVertex3f(m_pos[indexs[6]][0],m_pos[indexs[6]][1],m_pos[indexs[6]][2]);
					glVertex3f(m_pos[indexs[5]][0],m_pos[indexs[5]][1],m_pos[indexs[5]][2]);
					glEnd();
			}
		}
	}
	}
	else
	{
		//glColor4f((float)color[0],(float)color[1],(float)color[2],(float)color[3]);
		glColor3f(1.,0.,0.);
		glLineWidth(3.0);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		for(int i=0;i<m_nx-1;i++){
				for(int j=0;j<m_ny-1;j++){
					int indexs[8];
					indexs[0] = i + j*m_nx ;
					indexs[1] = i + (j+1)*m_nx ;
					indexs[2] = (i+1) + (j+1)*m_nx ;
					indexs[3] = (i+1) + j*m_nx ;

					glBegin(GL_QUADS);
					glVertex3f(m_pos[indexs[0]][0]-m_dx,m_pos[indexs[0]][1]-m_dy,0);
					glVertex3f(m_pos[indexs[1]][0]+m_dx,m_pos[indexs[1]][1]-m_dy,0);
					glVertex3f(m_pos[indexs[2]][0]+m_dx,m_pos[indexs[2]][1]+m_dy,0);
					glVertex3f(m_pos[indexs[3]][0]-m_dx,m_pos[indexs[3]][1]+m_dy,0);
					glEnd();
			}
		}	
	}
}
/****************************************************************************/
/****************************************************************************/
void GridFluid::displayBorders()
{
	//printf("n:%d %d %d\n",m_nx,m_ny,m_nz);
	if(m_nz>0)
	{
	glColor4f((float)color[0],(float)color[1],(float)color[2],(float)color[3]);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	int indexs[8];
	
	indexs[0] = 0; // i=0 j=0 k=0
	indexs[1] = m_ny*m_nx; // i=0 j=m_ny k=0
	indexs[2] = m_nx; //i=m_nx j=0 k=0
	indexs[3] = m_nx + m_ny*m_nx; // i=m_nx j=m_ny k=0

	indexs[4] = m_ny*m_nx*m_nz;
	indexs[5] = m_ny*m_nx + m_ny*m_nx*m_nz;
	indexs[6] = m_nx + m_ny*m_nx*m_nz;
	indexs[7] = m_nx + m_ny*m_nx + m_ny*m_nx*m_nz;

	glBegin(GL_QUADS);
	glVertex3f(m_pos[indexs[0]][0],m_pos[indexs[0]][1],m_pos[indexs[0]][2]);
	glVertex3f(m_pos[indexs[1]][0],m_pos[indexs[1]][1],m_pos[indexs[1]][2]);
	glVertex3f(m_pos[indexs[2]][0],m_pos[indexs[2]][1],m_pos[indexs[2]][2]);
	glVertex3f(m_pos[indexs[3]][0],m_pos[indexs[3]][1],m_pos[indexs[3]][2]);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(m_pos[indexs[4]][0],m_pos[indexs[4]][1],m_pos[indexs[4]][2]);
	glVertex3f(m_pos[indexs[5]][0],m_pos[indexs[5]][1],m_pos[indexs[5]][2]);
	glVertex3f(m_pos[indexs[6]][0],m_pos[indexs[6]][1],m_pos[indexs[6]][2]);
	glVertex3f(m_pos[indexs[7]][0],m_pos[indexs[7]][1],m_pos[indexs[7]][2]);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(m_pos[indexs[0]][0],m_pos[indexs[0]][1],m_pos[indexs[0]][2]);
	glVertex3f(m_pos[indexs[1]][0],m_pos[indexs[1]][1],m_pos[indexs[1]][2]);
	glVertex3f(m_pos[indexs[5]][0],m_pos[indexs[5]][1],m_pos[indexs[5]][2]);
	glVertex3f(m_pos[indexs[4]][0],m_pos[indexs[4]][1],m_pos[indexs[4]][2]);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(m_pos[indexs[3]][0],m_pos[indexs[3]][1],m_pos[indexs[3]][2]);
	glVertex3f(m_pos[indexs[2]][0],m_pos[indexs[2]][1],m_pos[indexs[2]][2]);
	glVertex3f(m_pos[indexs[6]][0],m_pos[indexs[6]][1],m_pos[indexs[6]][2]);
	glVertex3f(m_pos[indexs[7]][0],m_pos[indexs[7]][1],m_pos[indexs[7]][2]);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(m_pos[indexs[0]][0],m_pos[indexs[0]][1],m_pos[indexs[0]][2]);
	glVertex3f(m_pos[indexs[3]][0],m_pos[indexs[3]][1],m_pos[indexs[3]][2]);
	glVertex3f(m_pos[indexs[7]][0],m_pos[indexs[7]][1],m_pos[indexs[7]][2]);
	glVertex3f(m_pos[indexs[4]][0],m_pos[indexs[4]][1],m_pos[indexs[4]][2]);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(m_pos[indexs[1]][0],m_pos[indexs[1]][1],m_pos[indexs[1]][2]);
	glVertex3f(m_pos[indexs[2]][0],m_pos[indexs[2]][1],m_pos[indexs[2]][2]);
	glVertex3f(m_pos[indexs[6]][0],m_pos[indexs[6]][1],m_pos[indexs[6]][2]);
	glVertex3f(m_pos[indexs[5]][0],m_pos[indexs[5]][1],m_pos[indexs[5]][2]);
	glEnd();
	}
	else
	{
	//glColor4f((float)color[0],(float)color[1],(float)color[2],(float)color[3]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(color[0],color[1],color[2],color[3]);
	glLineWidth(5.0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINES);

	glBegin(GL_QUADS);
	glVertex3f(m_min[0]+m_dx,m_min[1]+m_dy,0);
	glVertex3f(m_max[0]-m_dx,m_min[1]+m_dy,0);
	glVertex3f(m_max[0]-m_dx,m_max[1]-m_dy,0);
	glVertex3f(m_min[0]+m_dx,m_max[1]-m_dy,0);
	glEnd();

	glColor3f(1,1,1);
	glDisable(GL_BLEND);
	}
}
/****************************************************************************/
/****************************************************************************/
