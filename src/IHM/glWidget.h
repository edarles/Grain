#ifndef GLWIDGET_H
#define GLWIDGET_H

//************************************************************************/
//************************************************************************/
#include <QWidget>
#include <math.h>
#include <QGLViewer/qglviewer.h>
#include <Eigen/Dense>
#include <Film.h>

using namespace std;
using namespace Eigen;

//************************************************************************/
//************************************************************************/

using namespace qglviewer;

//************************************************************************/
//************************************************************************/
 class GLWidget : public QGLViewer
 {
  
 public:

  GLWidget();
  GLWidget(float dt);
  GLWidget(unsigned int width, unsigned heigth, float resSpatial, int nbLayers, float dt);
  GLWidget(Film* film);
  GLWidget(Film* film, float dt, unsigned int indexLayer);

  virtual ~GLWidget();
   //******************************************************************************
   // GETTERS ET SETTERS
   //******************************************************************************

   Film* getFilm();
   float getDt();

   void  setFilm(Film*);
   void  setDt(float dt);
   void  setChannel(unsigned int channel);

   void  addViewer(GLWidget* widget);
   vector<GLWidget*> getViewers();

  //******************************************************************************
  // SURCHARGE DES FONCTIONS
  //******************************************************************************
     virtual void  init();
     virtual void  draw();
     virtual void  animate();
     virtual QSize minimumSizeHint() const;
     virtual QSize sizeHint() const;
     //virtual void keyPressEvent(QKeyEvent *e);

  //******************************************************************************
  // FONCTIONS PROPRES
  //******************************************************************************
     void clean();
     void initRender();

     void createFilmGray(unsigned int width, unsigned int height, float resSpatial);
     void createFilmRGB(unsigned int width, unsigned int height, float resSpatial);
     
     void computeFinalImage();

  private :

     //******************************************************************************
    Film *film;
    float dt;
    unsigned int width, height;
    float resSpatial;
    unsigned int channel;

    vector<GLWidget*> viewers_layers;

    void createViewers();
 };

#endif
