#include <GL/glew.h>

// Affichage des particules avec shaders GLSL
extern const char *sphereVertexShader;
extern const char *spherePixelShader;

#pragma once
inline void displaySpheres(GLuint m_program, float* pos, float* colors, int nbPos)
{
	glEnable(GL_POINT_SPRITE_ARB);
	glEnable(GL_BLEND);                               
   	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);  // how transparency acts
       	glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
			
        glUseProgram(m_program);
        glUniform1f( glGetUniformLocation(m_program, "pointScale"), 768 / tanf(60*0.5f*(float)M_PI/180.0f) );
	glUniform1f( glGetUniformLocation(m_program, "near"), 0.000000001 );
	glUniform1f( glGetUniformLocation(m_program, "far"), 0.00001 );

   	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glColor3f(1,1,1);
	glEnableClientState(GL_VERTEX_ARRAY); 
	glEnableClientState(GL_COLOR_ARRAY);
	
	glColorPointer(4, GL_FLOAT, 0, colors);
        glVertexPointer(4, GL_FLOAT, 0, pos);
	
	glDrawArrays(GL_POINTS, 0, nbPos);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
        glUseProgram(0);
	glDisable(GL_BLEND);
        glDisable(GL_POINT_SPRITE_ARB);
}