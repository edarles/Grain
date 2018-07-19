#include <Film.h>
/****************************************************************************/
/****************************************************************************/
Film::Film(unsigned int width, unsigned height, float resSpatial, unsigned int nbLayers, vector< vector<float> > intensity)
{
    this->width = width;
    this->height = height;
    this->resSpatial = resSpatial;
    layers = new Layers(width,height,resSpatial,nbLayers,intensity);
}
/****************************************************************************/
Film::Film(unsigned int width, unsigned height, float resSpatial, unsigned int nbLayers, vector< vector<Vector3f> > intensity)
{
    this->width = width;
    this->height = height;
    this->resSpatial = resSpatial;
    layers = new Layers(width,height,resSpatial,nbLayers,intensity);
}
/****************************************************************************/
Film::~Film()
{   
    delete(layers);
}
/****************************************************************************/
/****************************************************************************/
unsigned int Film::getWidth()
{
    return width;
}
/****************************************************************************/
unsigned int Film::getHeight()
{
    return height;
}
/****************************************************************************/
float Film::getResSpatial()
{
    return resSpatial;
}
/****************************************************************************/
Layers* Film::getLayers()
{
    return layers;
}
/****************************************************************************/
unsigned int Film::getNbLayers()
{
    return layers->getNbLayers();
}
/****************************************************************************/
/****************************************************************************/
void Film::update(float dt)
{
    layers->update(dt);
}
/****************************************************************************/
/****************************************************************************/
void Film::computeFinalImage()
{
    layers->computeFinalImage();
}
/****************************************************************************/
/****************************************************************************/
void Film::display(unsigned int channel)
{
    layers->display(channel);
}
/****************************************************************************/
/****************************************************************************/
