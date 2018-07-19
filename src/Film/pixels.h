/****************************************************************************/
/****************************************************************************/
#ifndef PIXELS_H
#define PIXELS_H

#include <types.h>
#include <vector>
using namespace std;

class Pixels
{
    public :
        Pixels(unsigned int width, unsigned height, float resSpatial, vector< vector<float> > intensity, float coeff);
        Pixels(unsigned int width, unsigned height, float resSpatial);
        ~Pixels();

        unsigned int getWidth();
        unsigned int getHeight();
        float        getResSpatial();

        Pixel getPixel(unsigned int i, unsigned int j);
        void  setPixel(unsigned int i, unsigned int j, Pixel p);
        void  addPixel(unsigned int i, unsigned int j, float intensity, float coeff );

        void update(float dt);
        void convoluate();

        void display(Vector3f color);
        void display();
        
        void saveImage(const char* nameImage);
        void saveImageRGB(const char* nameImage);
        
    private :

        Pixel** pixels;
        unsigned int width, height;
        float resSpatial;

        void calculateGradientIntensity();
};
#endif
/****************************************************************************/
/****************************************************************************/ 