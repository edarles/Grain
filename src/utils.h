#include <GL/glew.h>
#include <Eigen/Dense>

using namespace Eigen;

#pragma once
#define MAX_PARTICLES 5000

#pragma once
#define GRID_DISPLAY 0

#pragma once
#define GRID_BORDERS 0

// DISPLAY_MODE : Mode d'affichage des particules
// 0 : Pas de couleur (affichage avec la couleur des particules donné à l'init)
// 1 : Couleur en fonction de l'importance de la masse
// 2 : Couleur en fonction de l'importance de la vitesse
// @ Todo 3 : Couleur en fonction de l'importance de la densité
// @ Todo 4 : Couleur en fonction de l'importance de la pression 
#pragma once
#define DISPLAY_MODE 0

