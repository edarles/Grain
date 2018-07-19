#include <utils.h>
#include <GL/glew.h>
#include <iostream>
#include <unistd.h>
#include <glWidget.h>

//******************************************************************************
//******************************************************************************
GLWidget::GLWidget():QGLViewer()
{
	this->dt = 0.001;
	this->film = NULL;
	this->width = 8;
	this->height = 8;
	this->resSpatial = 1.0;
	this->channel = -1;
	createFilmGray(width,height,resSpatial);
}
//******************************************************************************
//******************************************************************************
GLWidget::GLWidget(float dt):QGLViewer()
{
	this->dt = dt;
	this->film = NULL;
	this->width = 8;
	this->height = 8;
	this->resSpatial = 1.0;
	this->channel = -1;
	createFilmGray(width,height,resSpatial);
}
//******************************************************************************
//******************************************************************************
GLWidget::GLWidget(unsigned int width, unsigned height, float resSpatial, int nbLayers, float dt):QGLViewer()
{
	this->dt = dt;
	this->width = width;
	this->height = height;
	this->resSpatial = resSpatial;
	this->film = NULL;
	this->channel = -1;
	if(nbLayers==1) createFilmGray(width,height,resSpatial);
	if(nbLayers==3) createFilmRGB(width,height,resSpatial);
}
//******************************************************************************
//******************************************************************************
GLWidget::GLWidget(Film* film):QGLViewer()
{
	this->dt = 0.001;
	this->film = film;
	this->channel = -1;
}
//******************************************************************************
//******************************************************************************
GLWidget::GLWidget(Film* film, float dt, unsigned int indexLayer):QGLViewer()
{
	this->dt = dt;
	this->film = film;
	this->channel = indexLayer;
}
//******************************************************************************
//******************************************************************************
GLWidget::~GLWidget()
{
	delete(film);
	for(unsigned int i=0;i<this->viewers_layers.size();i++)
		delete(this->viewers_layers[i]);
}
//******************************************************************************
//******************************************************************************
Film* GLWidget::getFilm()
{
	return this->film;
}
//******************************************************************************
float GLWidget::getDt()
{
	return this->dt;
}
//******************************************************************************
void GLWidget::addViewer(GLWidget* viewer)
{
	this->viewers_layers.push_back(viewer);
}
//******************************************************************************
vector<GLWidget*> GLWidget::getViewers()
{
	return this->viewers_layers;
}
//******************************************************************************
void GLWidget::setFilm(Film* film)
{
	this->film = film;
}
//******************************************************************************
void GLWidget::setDt(float dt)
{
	this->dt = dt;
}
//******************************************************************************

void GLWidget::setChannel(unsigned int channel)
{
	this->channel = channel;
}
//******************************************************************************
//******************************************************************************
QSize GLWidget::minimumSizeHint() const
{
	return QSize(51, 51);
}
//******************************************************************************
QSize GLWidget::sizeHint() const 
{
	return QSize(512, 512);
}
//******************************************************************************
//******************************************************************************
void GLWidget::init() 
{
	printf("init\n");
	setTextIsEnabled (true);
	setShortcut(ANIMATION, Qt::CTRL + Qt::Key_A);
	camera()->setType(Camera::ORTHOGRAPHIC);
	camera()->setSceneRadius(2.0);
	camera()->showEntireScene();
	//camera()->setPosition(Vec(-38.644604,5.909318,25.212061));
	//camera()->setViewDirection(Vec(-0.061944,-0.243702,-0.967870));
	initRender();
	setAnimationPeriod(1);
	setFPSIsDisplayed(true);
}

//******************************************************************************
//******************************************************************************
void GLWidget::initRender() 
{
	glewInit();
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	srand ( time ( 0x0 ) );
	glClearColor( 0,0,0, 1.0 );
	glShadeModel( GL_SMOOTH );
	glEnable ( GL_COLOR_MATERIAL );
	glEnable (GL_DEPTH_TEST);
	glDepthMask ( 1 );
}
//******************************************************************************
//******************************************************************************
void GLWidget::clean()
{
	delete(this->film);
  	init();
}
//******************************************************************************
//******************************************************************************
void GLWidget::draw() 
{
	if(this->channel==-1)
		this->film->getLayers()->getPixels()->display();
	else 
	if(this->channel>=0)
		this->film->display(this->channel);

	for(int i=0;i<this->viewers_layers.size();i++)
		this->viewers_layers[i]->setCamera(this->camera());
}
//******************************************************************************
//******************************************************************************
void GLWidget::animate()
{
	this->dt = 0.005;
	this->film->update(this->dt);
	for(int i=0;i<this->viewers_layers.size();i++)
		this->viewers_layers[i]->update();
}
//******************************************************************************
//******************************************************************************
void GLWidget::computeFinalImage()
{
	this->film->computeFinalImage();
}
//******************************************************************************
//******************************************************************************
void GLWidget::createFilmGray(unsigned int width, unsigned int height, float resSpatial)
{
	printf("create film Gray\n");
	vector< vector<float> > intensities;
	for(unsigned int i=0; i<this->width;i++)
	{
		vector<float> iL;
		for(unsigned int j=0; j<this->height; j++)
		{
			float intens = drand48();//max(i,j)/(double)max(width,height);
			iL.push_back(intens);
		}
		intensities.push_back(iL);
	}
	this->film = new Film(width,height,resSpatial,1,intensities);
	createViewers();
	printf("Fin create film\n");
}
//******************************************************************************
//******************************************************************************
void GLWidget::createFilmRGB(unsigned int width, unsigned int height, float resSpatial)
{
	printf("create film RGB\n");
	vector< vector<Vector3f> > intensities;
	for(unsigned int i=0; i<this->width;i++)
	{
		vector<Vector3f> iL;
		for(unsigned int j=0; j<this->height; j++)
		{
			float intensR = drand48();//max(i,j)/(double)max(width,height);
			float intensG = drand48();//max(i,j)/(double)max(width,height);
			float intensB = drand48();//max(i,j)/(double)max(width,height);
			iL.push_back(Vector3f(intensR,intensG,intensB));
		}
		intensities.push_back(iL);
	}
	this->film = new Film(width,height,resSpatial,3,intensities);
	createViewers();
	printf("Fin create film\n");
}
//******************************************************************************
//******************************************************************************
void GLWidget::createViewers()
{
	for(unsigned int i=0;i<this->film->getNbLayers();i++){
		GLWidget *widget = new GLWidget(this->film,this->dt,i);
		addViewer(widget);
	}
}