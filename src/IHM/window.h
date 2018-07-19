#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QtGui>
#include <QMainWindow>
#include <QtCore/QDir>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMenuBar>
#include <QFileDialog>
#include <glWidget.h>

//************************************************************************/
//************************************************************************/
class Window : public QMainWindow
{
    Q_OBJECT

//************************************************************************/
public:
    Window();
    Window(float dt);
    virtual ~Window();

    GLWidget* getGLWidget();
    void setGLWidget(GLWidget* glWidget);

    void createWindow();

//************************************************************************/
public slots:

     // slots du menu Animation
     void play();
     void stop();
     void init();
     void computeFinalImage();
     void deleteWidget();

private :

    //************************************************************************/
    GLWidget *glWidget1;
   
    QMenuBar *menuBar;
    QMenu *menuAnimation;
    
     // ACTION MENU ANIMATION
    QAction *play_act;
    QAction *stop_act;
    QAction *init_act;
    QAction *compute_final_image_act;
    QAction *close_act;

    QHBoxLayout *layout2;
    QVBoxLayout *mainLayout;
    QDir current_dir;

    void alertBox(QString text, QMessageBox::Icon);
};
//************************************************************************/
//************************************************************************/
#endif

