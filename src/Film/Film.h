/****************************************************************************/
/****************************************************************************/
#ifndef FILM_H
#define FILM_H

#include <Layers.h>
#include <vector>
using namespace std;

class Film
{
    public :
        
        Film(unsigned int width, unsigned height, float resSpatial, unsigned int nbLayers, vector< vector<float> > intensity);
        Film(unsigned int width, unsigned height, float resSpatial, unsigned int nbLayers, vector< vector<Vector3f> > intensity);
        ~Film();

        unsigned int getWidth();
        unsigned int getHeight();
        float        getResSpatial();
        Layers*      getLayers();
        unsigned int getNbLayers();

        void update(float dt);
        void computeFinalImage();
        void display(unsigned int channel);

    private :

        Layers* layers;
        unsigned int width, height;
        float resSpatial;
};
#endif
/****************************************************************************/
/****************************************************************************/ 