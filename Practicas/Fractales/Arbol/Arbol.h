/* -------------------------------------------------------------------
 * Arbol.h
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
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#ifdef __APPLE__
# define __gl_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif
//#include "Dependencies\freeglut\freeglut.h"// cambienlo de acuerdo a como ustedes lo tengan
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <vector>

//#include "Dependencies\glew\glew.h"// cambienlo de acuerdo a como ustedes lo tengan
//#include "Dependencies\freeglut\freeglut.h"// cambienlo de acuerdo a como ustedes lo tengan

using namespace std;

/**
 * Definimos una clase punto.
 */
class Punto
{
  /**
   * Atributos x, y del punto.
   */
 private:
  float x,y;

  /**
   * Constructor y métodos.
   */
 public:
  Punto(float x, float y);
  float getX();
  float getY();
  void setX(float x);
  void setY(float y);
};

/**
 * Definimos nuestra clase alfombra que será el fractal.
 */
class Arbol
{

  /**
   * Aquí van los atributos y los métodos privados.
   */
 private:
  unsigned int level;
  Punto *p1,*p2;
  void draw(int next,Punto *p1, Punto *p2,bool quitar);

  /**
   * Métodos contructores y de la clase.
   */
 public:
  Arbol(int level,Punto *p1, Punto *p2);
  int getLevel();
  void setLevel(int level);  
  void draw(int next);
  void quita_ultimo_nivel(int next);
}; //Fin de Arbol.h
