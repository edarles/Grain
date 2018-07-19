/****************************************************************************/
/****************************************************************************/
#ifndef LAYERS_H
#define LAYERS_H

#include <Layer.h>
#include <vector>
using namespace std;

class Layers
{
    public :
        
        Layers(unsigned int width, unsigned height, float resSpatial, unsigned int nbLayers, vector< vector<float> > intensity);
        Layers(unsigned int width, unsigned height, float resSpatial, unsigned int nbLayers, vector< vector<Vector3f> > intensity);
        ~Layers();

        Pixels* getPixels() const;
        unsigned int getNbLayers() const;
        Layer* getLayer(unsigned int i) const;
      
        void update(float dt);
        void computeFinalImage();
        void display(unsigned int channel);

    private :

        vector<Layer*> layers;
        Pixels* pixels;
};
#endif
/****************************************************************************/
/****************************************************************************/ 