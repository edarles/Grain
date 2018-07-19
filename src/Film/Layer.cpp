#include <Layer.h>
/****************************************************************************/
/****************************************************************************/
Layer::Layer(unsigned int width, unsigned height, float resSpatial, unsigned int channel, vector< vector<float> > intensities, float coeff)
{
    this->channel = channel;
   /* if(channel>0)
    {
        for(unsigned int i=0;i<intensities.size();i++)
            for(unsigned int j=0;j<intensities[i].size();j++)
                intensities[i][j] = 1.0-intensities[i][j];
    }*/
    pixels = new Pixels(width, height, resSpatial, intensities, coeff);
    if(this->channel == 0) color_display = Vector3f(1,1,1);
    if(this->channel == 1) color_display = Vector3f(1,0,0);
    if(this->channel == 2) color_display = Vector3f(0,1,0);
    if(this->channel == 3) color_display = Vector3f(0,0,1);

}
/****************************************************************************/
Layer::Layer(unsigned int width, unsigned height, float resSpatial, unsigned int channel, vector< vector<Vector3f> > intensities, float coeff)
{
    this->channel = channel;
   /* if(channel>0)
    {
        for(unsigned int i=0;i<intensities.size();i++)
            for(unsigned int j=0;j<intensities[i].size();j++)
                intensities[i][j] = 1.0-intensities[i][j];
    }*/
    vector< vector<float> > intensitiesChannel;
    for(unsigned int i=0;i<intensities.size();i++){
        vector<float> intens;
        for(unsigned int j=0;j<intensities[i].size();j++)
            intens.push_back(intensities[i][j][channel]);
        intensitiesChannel.push_back(intens);
    }
    pixels = new Pixels(width, height, resSpatial, intensitiesChannel, coeff);
    if(this->channel == 0) color_display = Vector3f(1,1,1);
    if(this->channel == 1) color_display = Vector3f(1,0,0);
    if(this->channel == 2) color_display = Vector3f(0,1,0);
    if(this->channel == 3) color_display = Vector3f(0,0,1);

}
/****************************************************************************/
Layer::~Layer()
{   
    delete(pixels);
}
/****************************************************************************/
/****************************************************************************/
Pixels* Layer::getPixels()
{
    return pixels;
}
/****************************************************************************/
unsigned int Layer::getChannel()
{
    return channel;
}
/****************************************************************************/
void Layer::setColorDisplay(Vector3f c)
{
    color_display = c;
}
/****************************************************************************/
/****************************************************************************/
void Layer::update(float dt)
{
   pixels->update(dt);
}
/****************************************************************************/
/****************************************************************************/
void Layer::convoluate()
{
    pixels->convoluate();
}
/****************************************************************************/
/****************************************************************************/
void Layer::display()
{
   pixels->display(color_display);
}
/****************************************************************************/
/****************************************************************************/
