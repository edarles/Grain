#include <window.h>
#include <QGroupBox>
//************************************************************************/
//************************************************************************/
Window::Window():QMainWindow()
{
    // Final rendering
    glWidget1 = new GLWidget(8,8,1.0,3,0.01) ;
    createWindow();
}
//************************************************************************/
Window::Window(float dt):QMainWindow()
{
    // Final rendering
    glWidget1 = new GLWidget(8,8,1.0,1,dt) ;
    createWindow();
}
//************************************************************************/
Window::~Window()
{
    delete(glWidget1);
}
//************************************************************************/
void Window::createWindow()
{
    menuBar = new QMenuBar(0);

     // MENU ANIMATION
    menuAnimation = new QMenu(tr("&Animation"));   
    
    play_act = new QAction(tr("&Play"),this);
    play_act->setShortcut(tr("p"));
    connect(play_act, SIGNAL(triggered()), this, SLOT(play()));
    
    stop_act = new QAction(tr("&Stop"),this);
    stop_act->setShortcut(tr("s"));
    connect(stop_act, SIGNAL(triggered()), this, SLOT(stop()));
    
    init_act = new QAction(tr("&ReInit"),this);
    init_act->setShortcut(tr("i"));
    connect(init_act, SIGNAL(triggered()), this, SLOT(init()));
    
    compute_final_image_act = new QAction(tr("&Compute Final Image"),this);
    compute_final_image_act->setShortcut(tr("c"));
    connect(compute_final_image_act, SIGNAL(triggered()), this, SLOT(computeFinalImage()));

    close_act = new QAction(tr("&Close"),this);
    close_act->setShortcut(tr("q"));
    connect(close_act, SIGNAL(triggered()), this, SLOT(deleteWidget()));
    
    menuAnimation->addAction(play_act);
    menuAnimation->addAction(stop_act);
    menuAnimation->addAction(init_act);
    menuAnimation->addAction(compute_final_image_act); 
    menuAnimation->addAction(close_act); 
    
    menuBar->addMenu(menuAnimation);

    this->setMenuBar(menuBar);
    
    QGroupBox* horizontalGroupBox = new QGroupBox;
    QGridLayout *layout = new QGridLayout;
    
    QGroupBox *group1 = new QGroupBox;
    QHBoxLayout *horizontal1 = new QHBoxLayout;
    horizontal1->addWidget(glWidget1);
    
    QGroupBox* group2 = new QGroupBox;
    QHBoxLayout *horizontal2 = new QHBoxLayout;
    vector<GLWidget*> viewers = glWidget1->getViewers();
    
    for(unsigned int i=0;i<viewers.size();i++)
        horizontal2->addWidget(viewers[i]);

    group1->setLayout(horizontal1);
    group2->setLayout(horizontal2);

    layout->addWidget(group1,0,0);
    layout->addWidget(group2,0,1);

    horizontalGroupBox->setLayout(layout);
    this->setCentralWidget(horizontalGroupBox);

    this->setAttribute( Qt::WA_DeleteOnClose );
    this->setWindowTitle(tr("Grains Film Simulation"));
}
//************************************************************************/
//************************************************************************/
GLWidget* Window::getGLWidget()
{
	return glWidget1;
}
//************************************************************************/
//************************************************************************/
void Window::setGLWidget(GLWidget* G)
{
	 this->glWidget1 = G;
}

//************************************************************************/
//************************************************************************/
// SLOTS MENU ANIMATION
//************************************************************************/
//************************************************************************/
void Window::play()
{
 if(glWidget1!=NULL)
 {
	printf("fonction animate\n");
 	glWidget1->startAnimation();
 	glWidget1->animate();
 }
 else 
	alertBox("glwidget non initialisé !!",QMessageBox::Critical);
}
//************************************************************************/
void Window::stop()
{
	glWidget1->stopAnimation();
}
//************************************************************************/
void Window::computeFinalImage()
{
    glWidget1->computeFinalImage();
}
//************************************************************************/
void Window::init()
{
	glWidget1->clean();
}
//************************************************************************/
void Window::deleteWidget()
{
    delete(glWidget1);
    close();
}

//************************************************************************/
//************************************************************************/
void Window::alertBox(QString text, QMessageBox::Icon icon)
{
	QMessageBox Q;
	Q.setIcon(icon);
	Q.setText(text);
    Q.exec();
}

//************************************************************************/
//************************************************************************/
