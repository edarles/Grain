/****************************************************************************/
/****************************************************************************/
#ifndef LAYER_H
#define LAYER_H

#include <pixels.h>
using namespace std;

class Layer
{
    public :
        
        Layer(unsigned int width, unsigned height, float resSpatial, unsigned int channel, vector< vector<float> > intensity, float coeff);
        Layer(unsigned int width, unsigned height, float resSpatial, unsigned int channel, vector< vector<Vector3f> > intensity, float coeff);
        ~Layer();

        Pixels* getPixels();
        unsigned int getChannel();

        void setColorDisplay(Vector3f);
        
        void update(float dt);
        void convoluate();
        void display();

    private :

        unsigned int channel;
        Pixels* pixels;
        Vector3f color_display;
};
#endif
/****************************************************************************/
/****************************************************************************/ 