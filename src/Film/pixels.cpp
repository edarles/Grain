#include <pixels.h>
#include <QImage>
#include <QColor>
#include <opencv2/opencv.hpp>
using namespace cv;
/****************************************************************************/
/****************************************************************************/
Pixels::Pixels(unsigned int width, unsigned height, float resSpatial, vector< vector<float> > intensity, float coeff)
{
    this->width = width;
    this->height = height;
    this->resSpatial = resSpatial;
    this->pixels = (Pixel**)malloc(width*sizeof(Pixel*));
    
    for(unsigned int i=0;i<this->width;i++){
        this->pixels[i] =  (Pixel*) malloc(this->height*sizeof(Pixel));
        for(unsigned int j=0;j<this->height;j++){
            this->pixels[i][j].sph = NULL;
            addPixel(i,j,intensity[i][j],coeff);
        }
    }
    calculateGradientIntensity();
}

/****************************************************************************/
/****************************************************************************/
Pixels::Pixels(unsigned int width, unsigned height, float resSpatial)
{
    float tx = 1.0;//resSpatial/width;
    float ty = 1.0;//resSpatial/height;

    this->width = width;
    this->height = height;
    this->resSpatial = resSpatial;
    this->pixels = (Pixel**)malloc(width*sizeof(Pixel*));
    
    for(unsigned int i=0;i<this->width;i++){
        this->pixels[i] =  (Pixel*) malloc(this->height*sizeof(Pixel));
        for(unsigned int j=0;j<height;j++){
            this->pixels[i][j].intensityRGB = Vector3f(0,0,0);
            this->pixels[i][j].intensity = 0;
            this->pixels[i][j].center = Vector2f(i-tx/2.0,j-ty/2);
        }
    }
}
/****************************************************************************/
Pixels::~Pixels()
{
    #pragma omp parallel for
    for(unsigned int i=0; i<this->width;i++)
        free(this->pixels[i]);
    free(this->pixels);
}
/****************************************************************************/
/****************************************************************************/
unsigned int Pixels::getWidth()
{
    return this->width;
}
/****************************************************************************/
unsigned int Pixels::getHeight()
{
    return this->height;
}
/****************************************************************************/
float Pixels::getResSpatial()
{
    return this->resSpatial;
}
/****************************************************************************/
Pixel Pixels::getPixel(unsigned int i, unsigned int j)
{
    assert(i<this->width && j<this->height);
    return this->pixels[i][j];
}
/****************************************************************************/
/****************************************************************************/
void  Pixels::setPixel(unsigned int i, unsigned int j, Pixel p)
{
    assert(i<this->width && j<this->height);
    this->pixels[i][j] = p;
}
/****************************************************************************/
/****************************************************************************/
void  Pixels::addPixel(unsigned int i, unsigned int j, float intensity, float coeff)
{
    assert(i<this->width && j<this->height);
   
    float tx = 1.0;//resSpatial/width;
    float ty = 1.0;//resSpatial/height;

    Vector3f origin = Vector3f(i-tx/2.0,j-ty/2.0,0);
    Vector3f cmin = Vector3f(i,j,0);
    Vector3f cmax = Vector3f(i+tx,j+ty,0);

    this->pixels[i][j].intensity = intensity;  
    this->pixels[i][j].gradIntensity = Vector2f(0,0);
    this->pixels[i][j].center = Vector2f(origin[0],origin[1]);
    this->pixels[i][j].i = i;
    this->pixels[i][j].j = j;
    this->pixels[i][j].sph = new WCSPH2D_BROWNIAN(origin, cmin, cmax, intensity*coeff);
}
/****************************************************************************/
/****************************************************************************/
void Pixels::update(float dt)
{
    #pragma omp parallel for
    for(unsigned int i=0; i<this->width; i++){
        for(unsigned int j=0;j<this->height;j++){
            if(this->pixels[i][j].sph!=NULL)
                this->pixels[i][j].sph->update(dt);
        }
    }
}
/****************************************************************************/
/****************************************************************************/
void Pixels::convoluate()
{
    #pragma omp parallel for
    // parcours de la grille de pixels
    for(unsigned int i=0; i<this->width; i++){
        for(unsigned int j=0;j<this->height;j++){
            float dens = 0;
            float Wij_t = 0;
            Vector2f pos1 = this->pixels[i][j].center;
            // parcours du voisinage de chaque pixels -> 9 voisins en 2D
            for(unsigned int k=i-1;k<i+1;k++){
                for(unsigned int l=j-1;j<j+1;l++){
                    // calcul de la densité au centre du pixels par méthode SPH
                    // on récupère la somme des densités des particules du système SPH que l'on pondère avec une fonction de distance
                    vector<SPHParticle*>  particles = this->pixels[k][l].sph->getParticles();
                    if(particles.size()>0){
                        for(unsigned int index=0;index<particles.size();index++){
                            Vector3f pp = particles[index]->getPos();
                            Vector2f pos2 = Vector2f(pp[0],pp[1]); 
			                float h = particles[index]->getRadius();
			                float d = (pos1-pos2).norm();
			                float Wij = 315*powf((h*h)-(d*d),3)/(64*M_PI*powf(h,9));
                            Wij_t += Wij;
                            dens+=particles[index]->getRho()*Wij; 
                        }
                    }
                }
            }
            dens/=Wij_t;
            printf("i:%d j:%d dens:%f\n",i,j,dens);
            //this->pixels[i][j].intensity*=dens;
        }
    }
}
/****************************************************************************/
/****************************************************************************/
void Pixels::display(Vector3f color)
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_FRONT_AND_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    glClearColor(1,1,1,1);
    float tx = 1.0;//resSpatial/width;
    float ty = 1.0;//resSpatial/height;
    float s = 0.5;
    for(unsigned int i=0; i<this->width; i++){
        for(unsigned int j=0; j<this->height; j++){
                Vector2f pos = this->pixels[i][j].center;
                Vector2f center0 =  pos; center0[0]-=tx/2.0f; center0[1]-=ty/2.0f;
                Vector2f center1 =  pos; center1[0]-=tx/2.0f; center1[1]+=ty/2.0f;
                Vector2f center2 =  pos; center2[0]+=tx/2.0f; center2[1]+=ty/2.0f;
                Vector2f center3 =  pos; center3[0]+=tx/2.0f; center3[1]-=ty/2.0f;
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable( GL_BLEND );
                float intensity = this->pixels[i][j].intensity;
                glColor4f(intensity*color[0],intensity*color[1],intensity*color[2],0.5);

                glBegin(GL_QUADS);
				glVertex3f(center0[0]+tx,center0[1]+ty,0);
				glVertex3f(center1[0]+tx,center1[1]+ty,0);
				glVertex3f(center2[0]+tx,center2[1]+ty,0);
				glVertex3f(center3[0]+tx,center3[1]+ty,0);
				glEnd();

                if(this->pixels[i][j].sph!=NULL){
                    glColor3f(1,1,1);
                    this->pixels[i][j].sph->display();
                    this->pixels[i][j].sph->getGrid()->displayBorders();
                    // Affichage du vecteur du gradient d'intensité du pixel
                    /* Vector3f pos = this->pixels[i][j].sph->getGrid()->getCenter();
                    
                
                    glPointSize(10);
                    glBegin(GL_POINTS);
                    glVertex3f(pos[0],pos[1],0);
                    glEnd();
                    
                    glLineWidth(10.0);
                    glBegin(GL_LINES);
                    glVertex3f(pos[0],pos[1],0);
                    glVertex3f(pos[0]+this->pixels[i][j].gradIntensity[0]*s,pos[1]+this->pixels[i][j].gradIntensity[1]*s,0);
                    glEnd();
                    
                    glLineWidth(1.0);
                    glPointSize(1.0);*/
            }
        }
    }
    glColor3f(1,1,1);
    glDisable(GL_BLEND);
}
/****************************************************************************/
/****************************************************************************/
void Pixels::display()
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_FRONT_AND_BACK);

    float tx = 1.0;//resSpatial/width;
    float ty = 1.0;//resSpatial/height;

    for(unsigned int i=0; i<this->width; i++){
        for(unsigned int j=0; j<this->height; j++){
                Vector2f pos = this->pixels[i][j].center;
                Vector2f center0 =  pos; center0[0]-=tx/2.0f; center0[1]+=ty/2.0f;
                Vector2f center1 =  pos; center1[0]+=tx/2.0f; center1[1]+=ty/2.0f;
                Vector2f center2 =  pos; center2[0]+=tx/2.0f; center2[1]-=ty/2.0f;
                Vector2f center3 =  pos; center3[0]-=tx/2.0f; center3[1]-=ty/2.0f;

                Vector3f intensity = this->pixels[i][j].intensityRGB;
                glColor3f(intensity[0],intensity[1],intensity[2]);

                glBegin(GL_QUADS);
				glVertex3f(center0[0],center0[1],0);
				glVertex3f(center1[0],center1[1],0);
				glVertex3f(center2[0],center2[1],0);
				glVertex3f(center3[0],center3[1],0);
				glEnd();
        }
    }
    glColor3f(1,1,1);
}
/****************************************************************************/
/****************************************************************************/
void Pixels::saveImage(const char* nameImage)
{
    QImage image((int)width,(int)height,QImage::Format_RGB32);

    for(unsigned int i=0; i<this->height; i++){
        for(unsigned int j=0; j<this->width; j++){
            Pixel p = this->pixels[j][i];
            float gray = p.intensity*255.0f;
            QColor c(gray, gray, gray);
            image.setPixel(j,height-i-1, c.rgba());
            //image.setPixelColor(j,height-i-1,QColor(gray,gray,gray,1.0));
        }
    }
    image.save(nameImage,"PNG");
}
/****************************************************************************/
/****************************************************************************/
void Pixels::saveImageRGB(const char* nameImage)
{
    QImage image((int)width,(int)height,QImage::Format_RGB32);

    for(unsigned int i=0; i<this->height; i++){
        for(unsigned int j=0; j<this->width; j++){
            Pixel p = this->pixels[j][i];
            Vector3f intensity = p.intensityRGB;
            QColor c(intensity[0], intensity[1], intensity[2]);
            image.setPixel(j,height-i-1, c.rgba());
        }
    }
    image.save(nameImage,"PNG");
}
/****************************************************************************/
/****************************************************************************/
void Pixels::calculateGradientIntensity()
{
    std::string nameInput;
    nameInput = "output/tmp/tmp.png";
       
    saveImage(nameInput.c_str());
    Mat input = imread("output/tmp/tmp.png", CV_LOAD_IMAGE_COLOR);
	if(input.empty())
	{
		std::cout<<"Image Not Found: "<< nameInput << std::endl;
        exit(0);
	}
    GaussianBlur( input, input, Size(3,3), 0, 0, BORDER_DEFAULT );
    
    Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	int scale = 1;
	int delta = 0;
    int ddepth = CV_64F; // use 16 bits unsigned to avoid overflow
    /// Gradient X
    Sobel( input, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    //cout << grad_x << endl;
    /// Gradient Y
    Sobel( input, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    //cout << grad_y << endl;

    for(int i=0;i<grad_x.rows;i++)
    {
        for(int j=0;j<grad_x.cols*3;j+=3){
            int j_index = (int)ceil(j/3.0);
            pixels[j_index][height-i-1].gradIntensity[0] = grad_x.at<double>(i,j)/255.f;
            pixels[j_index][height-i-1].gradIntensity[1] = -grad_y.at<double>(i,j)/255.f;
            pixels[j_index][height-i-1].sph->addExternalForce(Vector3d(pixels[j_index][height-i-1].gradIntensity[0],pixels[j_index][height-i-1].gradIntensity[1],0.0));
        }
    }
}
/****************************************************************************/
/****************************************************************************/
