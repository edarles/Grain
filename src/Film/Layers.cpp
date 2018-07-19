#include <Layers.h>
/****************************************************************************/
/****************************************************************************/
Layers::Layers(unsigned int width, unsigned height, float resSpatial, unsigned int nbLayers, vector< vector<float> > intensity)
{
    vector<float> c; c.push_back(0.2); c.push_back(0.3); c.push_back(0.5); 
    layers.clear();
    for(unsigned int i=0;i<nbLayers;i++){
        Layer *L = new Layer(width,height,resSpatial,i,intensity,c[i]);
        if(i==0) L->setColorDisplay(Vector3f(1,0,0));
        if(i==1) L->setColorDisplay(Vector3f(0,1,0));
        if(i==2) L->setColorDisplay(Vector3f(0,0,1));
        layers.push_back(L);
    }

    // Grille finale de pixels
    pixels = new Pixels(width, height, resSpatial);
    for(unsigned int i=0;i<width;i++){
        for(unsigned int j=0;j<height;j++){
            Vector3f intensityRGB = Vector3f(0,0,0);
            for(unsigned int l=0;l<nbLayers;l++){
                float intL = layers[l]->getPixels()->getPixel(i,j).intensity;
                if(l<3) intensityRGB[l] += intL;
            }
            Pixel p = pixels->getPixel(i,j);
            p.intensityRGB = intensityRGB;
            pixels->setPixel(i,j,p);
            //printf("i:%d %d int:%f %f %f\n",i,j,p.intensityRGB[0],p.intensityRGB[1],p.intensityRGB[2]);
        }
    }
}
/****************************************************************************/
Layers::Layers(unsigned int width, unsigned height, float resSpatial, unsigned int nbLayers, vector< vector<Vector3f> > intensity)
{
    vector<float> c; c.push_back(0.2); c.push_back(0.3); c.push_back(0.5); 
    layers.clear();
    for(unsigned int i=0;i<nbLayers;i++){
        Layer *L = new Layer(width,height,resSpatial,i,intensity,c[i]);
        if(i==0) L->setColorDisplay(Vector3f(1,0,0));
        if(i==1) L->setColorDisplay(Vector3f(0,1,0));
        if(i==2) L->setColorDisplay(Vector3f(0,0,1));
        layers.push_back(L);
    }
  
    // Grille finale de pixels
    pixels = new Pixels(width, height, resSpatial);
    for(unsigned int i=0;i<width;i++){
        for(unsigned int j=0;j<height;j++){
            Vector3f intensityRGB = Vector3f(0,0,0);
            Pixel p = pixels->getPixel(i,j);
            p.intensityRGB = intensity[i][j];
            pixels->setPixel(i,j,p);
            //printf("i:%d %d int:%f %f %f\n",i,j,p.intensityRGB[0],p.intensityRGB[1],p.intensityRGB[2]);
        }
    }
}
/****************************************************************************/
Layers::~Layers()
{   
    for(unsigned int i=0;i<layers.size();i++)
        delete(layers[i]);
    layers.clear();
    delete(pixels);
}
/****************************************************************************/
Pixels* Layers::getPixels() const
{
    return pixels;
}
/****************************************************************************/
/****************************************************************************/
unsigned int Layers::getNbLayers() const
{
    return layers.size();
}
/****************************************************************************/
Layer* Layers::getLayer(unsigned int i) const
{
    assert(i<layers.size());
    return layers[i];
}
/****************************************************************************/
/****************************************************************************/
void Layers::update(float dt)
{
    for(unsigned int i=0;i<layers.size();i++)
        layers[i]->update(dt);
}
/****************************************************************************/
/****************************************************************************/
void Layers::computeFinalImage()
{
    for(unsigned int i=0;i<layers.size();i++){
        layers[i]->convoluate();
    }

    for(unsigned int i=0;i<layers.size();i++){
        Pixels* pix = layers[i]->getPixels();
        for(unsigned int nx=0;nx<pixels->getWidth();nx++){
            for(unsigned int ny=0;ny<pixels->getHeight();ny++){
                    Pixel pixF = pixels->getPixel(nx,ny);
                    float intensity = pix->getPixel(nx,ny).intensity;

                    //bof...
                    if(layers.size()>1)
                        pixF.intensityRGB[i]+=intensity;
                    else
                        pixF.intensity+=intensity;
                    pixels->setPixel(nx,ny,pixF);
            }
        }
    }
    pixels->saveImageRGB("final.png");
}
/****************************************************************************/
/****************************************************************************/
void Layers::display(unsigned int channel)
{
   layers[channel]->display();
}
/****************************************************************************/
/****************************************************************************/
