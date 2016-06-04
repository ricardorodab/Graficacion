/* -------------------------------------------------------------------
 * parts.c
 * versión 1.0
 * Copyright (C) 2016 
 * José Ricardo Rodríguez Abreu,
 * Facultad de Ciencias,
 * Universidad Nacional Autónoma de México, Mexico.
 *
 * Este programa es software libre; se puede redistribuir
 * y/o modificar en los términos establecidos por la
 * Licencia Pública General de GNU tal como fue publicada
 * por la Free Software Foundation en la versión 2 o
 * superior.
 *
 * Este programa es distribuido con la esperanza de que
 * resulte de utilidad, pero SIN GARANTÍA ALGUNA; de hecho
 * sin la garantía implícita de COMERCIALIZACIÓN o
 * ADECUACIÓN PARA PROPÓSITOS PARTICULARES. Véase la
 * Licencia Pública General de GNU para mayores detalles.
 *
 * Con este programa se debe haber recibido una copia de la
 * Licencia Pública General de GNU, de no ser así, visite el
 * siguiente URL:
 * http://www.gnu.org/licenses/gpl.html
 * o escriba a la Free Software Foundation Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * -------------------------------------------------------------------
 */
#include <GL/glew.h>
#ifdef __APPLE__
# define __gl_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GL/freeglut.h>
#include <GL/glut.h>
#include "parts.h"
#include <stdlib.h>
#include <math.h>

/* Función que dibuja la parte de la base del robot */
void base(void)
{
  glPushMatrix();
  glScaled(3.0, 0.5, 1.0);
  glColor3f(1.0,1.1,0);
  glutSolidTorus(0.2,0.2,10,50);
  glPopMatrix();
}

/* Función que dibuja la parte de la cintura del robot */
void cintura(int cinturaInt)
{
  glRotated(90, 0.0, 0.0, 1.0);
  glRotated(cinturaInt,1.0,0.0,0.0);
  glTranslated(1.0, 0.0, 0.0);
  glPushMatrix();
  glScaled(2.0, 0.4, 0.5);
  glColor3f(1.0,0,1.0);
  glutSolidCube(1.0);
  glPopMatrix();
  
}

/* Función que dibuja la parte del brazo del robot */
void brazo(int radioInt)
{
  glRotated(105+(double)radioInt, (double)0.0, 0.0, 1.0);
  //Codo rojo 1:
  glTranslated(-0.2, -1.0, 0);
  glColor3f(1.0,0.0,0.0);
  glutSolidSphere(0.5,50,40);
  glTranslated(-1.0, -0.0, 0.0);
  glPushMatrix();
  glScaled(2.0, 0.2, 0.5);
  glColor3f(0.0,1.0,1.0);
  glutSolidCube(1.0);
  glPopMatrix();
  
}

/* Función que dibuja la parte del codo del robot */
void codo(void)
{
  glTranslated(-1.0, 0.0, 0.0);
  glPushMatrix();
  glColor3f(1.0,0.0,0.0);
  glutSolidSphere(0.30,50,40);
  glPopMatrix();
}

/* Función que dibuja la parte de la mano del robot */
void mano(int largo,int radioInt)
{  
  glRotated(105-(double)radioInt, (double)0.0, 0.0, 1.0);
  glRotated(-30, 0.0, 0.0, 1.0);
  glTranslated(1.0-((double)largo/2), 0.0, 0.0);
  glPushMatrix();
  glScaled(2.0-largo,0.1,0.1);
  glColor3f(0.0,1,0.0);
  glutSolidCube(1.0);
  glPopMatrix();
}

/* Función que dibuja la parte de donde se sostienen los dedos del robot */
void nudillo(int largo)
{
  if(largo == 0)
    glTranslated(1.0, 0.0, 0.0);
  if(largo == 1)
    glTranslated(0.6, 0.0, 0.0);
  glPushMatrix();
  glScaled(0.3,0.7,0.7);
  glColor3f(1.0,0.0,0.0);
  glutSolidCube(1.0);
  glPopMatrix();
}

/* Función que dibuja los dedos del robot */
void dedos(float mvm[], int dedosCerrados)
{
  int cambio = 0;
  if(dedosCerrados)
    cambio = -45;
  //DEDO 1
  //Parte1 del dedo
  glPushMatrix();

  glTranslated(0.3, 0.5, 0.0);
  glRotated(45+cambio,0,0,1);
  glPushMatrix();
  glScaled(0.40,0.15,0.15);
  glColor3f(0.0,1.0,0.0);
  glutSolidCube(1.0);
  glPopMatrix();
  //Parte2 del dedo
  glTranslated(0.25, -0.13, 0);
  glRotated(90,0,0,1);
  glPushMatrix();
  glScaled(0.40,0.15,0.15);
  glColor3f(0.0,1.0,0.0);
  glutSolidCube(1.0);
  glPopMatrix();

  glPopMatrix();

  //DEDO 2
  //Parte1 del dedo
  glPushMatrix();

  glTranslated(0.3, -0.5, 0.0);
  glRotated(-45-cambio,0,0,1);
  glPushMatrix();
  glScaled(0.40,0.15,0.15);
  glColor3f(0.0,1.0,0.0);
  glutSolidCube(1.0);
  glPopMatrix();
  //Parte2 del dedo
  glTranslated(0.25, 0.13, 0);
  glRotated(270,0,0,1);
  glPushMatrix();
  glScaled(0.40,0.15,0.15);
  glColor3f(0.0,1.0,0.0);
  glutSolidCube(1.0);
  glPopMatrix();

  glPopMatrix();
  
  //DEDO 3
  //Parte1 del dedo
  glPushMatrix();

  glTranslated(0.3, 0.0, 0.5);
  glRotated(-45-cambio,0,1,0);
  glPushMatrix();
  glScaled(0.40,0.15,0.15);
  glColor3f(0.0,1.0,0.0);
  glutSolidCube(1.0);
  glPopMatrix();
  //Parte2 del dedo
  glTranslated(0.25, 0.0, -0.13);
  glRotated(270,0,1,0);
  glPushMatrix();
  glScaled(0.40,0.15,0.15);
  glColor3f(0.0,1.0,0.0);
  glutSolidCube(1.0);
  glPopMatrix();

  glPopMatrix();

  //DEDO 4
  //Parte1 del dedo
  glPushMatrix();

  glTranslated(0.3, 0.0, -0.5);
  glRotated(45+cambio,0,1,0);
  glPushMatrix();
  glScaled(0.40,0.15,0.15);
  glColor3f(0.0,1.0,0.0);
  glutSolidCube(1.0);
  glPopMatrix();
  //Parte2 del dedo
  glTranslated(0.25, 0.0, 0.13);
  glRotated(90,0,1,0);
  glPushMatrix();
  glScaled(0.40,0.15,0.15);
  glColor3f(0.0,1.0,0.0);
  glutSolidCube(1.0);
  glPopMatrix();

  glPopMatrix();

  glTranslated(0.5,0,0);
}

/* Dibuja la pelota. */
void pelota(double puntosPelota[])
{
  glTranslated(1.9+puntosPelota[0], 0.0+puntosPelota[1], 0+puntosPelota[2]);
  glColor3f(1.0,1.0,1.0);
  glutSolidSphere(.3,40,40);  
}
