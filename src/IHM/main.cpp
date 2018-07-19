#include <window.h>
#include <qapplication.h>

int main(int argc,char **argv)
{
  printf("\nGrains Film Simulation \n\n");
  setenv ("LC_NUMERIC","POSIX",1);
  QApplication app(argc,argv);
  Window *window = new Window();
  window->show();
  return app.exec();
}

