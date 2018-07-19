#pragma once

#include <wcsph2d_brownian.h>
#include <Eigen/Dense>
using namespace Eigen;

typedef struct
{
    WCSPH2D_BROWNIAN *sph;
    unsigned int i, j;
    float intensity;
    Vector3f intensityRGB;
    Vector2f gradIntensity;
    Vector2f center;
} Pixel;

const unsigned int MAX_WIDTH = 2048;
const unsigned int MAX_HEIGHT = 2048;
