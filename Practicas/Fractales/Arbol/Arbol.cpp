/* -------------------------------------------------------------------
 * Arbol.cpp
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
#include "Arbol.h"

/**
 * Declaracion del metodo contructor de la clase punto.
 */
Punto::Punto(float x, float y)
{
  this->x = x;
  this->y = y;
}

/**
 * Método que nos regresa el valor X de un punto.
 */
float Punto::getX()
{
  return this->x;
}

/**
 * Método que nos regresa el valor Y de un punto.
 */
float Punto::getY()
{
  return this->y;
}

/**
 * Método para asignar un nuevo valor X a un punto.
 */
void Punto::setX(float x)
{
  this->x = x;
}

/**
 * Método para asignar un nuevo valor Y a un punto.
 */
void Punto::setY(float y)
{
  this->y = y;
}

/**
 * Implementación del método contructor del fractal Arbol.
 */
Arbol::Arbol(int level,Punto *p1, Punto *p2)
{
  this->level = level;
  this->p1 = p1;
  this->p2 = p2;
}

/**
 * Método que nos regresa el nivel de iteración del fractal.
 */
int Arbol::getLevel()
{
  return this->level;
}

/**
 * Método para asignar el nivel de recursión del fractal.
 */
void Arbol::setLevel(int level)
{
  this->level = level;
}

/**
 * Método que dibuja a el fractal en la pantalla.
 */
void Arbol::draw(int next)
{
  this->level = next;
  draw(next,this->p1,this->p2,false);

}

/**
 * Método para dibujar de blanco el último nivel del fractal.
 */
void Arbol::quita_ultimo_nivel(int next)
{
  this->level = next;
  draw(next,this->p1,this->p2,true);
  
}

/**
 * Método privado para dibujar los niveles del fractal.
 */
void Arbol::draw(int next,Punto *p1, Punto *p2,bool quitar)
{
  if(next == 0)
    return;
  if(quitar && next == 1)
    {
    glColor3f(255,255,255);
    }
  else
    {
    glColor3f(0,0,0);
    }
  glBegin(GL_LINES);     
  glVertex2f(p1->getX(),p1->getY());
  glVertex2f(p2->getX(),p2->getY());
  glEnd();
  glFlush();
  
  if(next > 1){
    vector<Punto*> lista;

    float pi = 3.14159265;
    float ang1 = (60) * pi / 180;
    float ang2 = (300) * pi / 180;
    float factor = 0.6;
    float x = (p2->getX()-p1->getX());
    float y = (p2->getY()-p1->getY());
    float a1 = (cos(ang1) * x) - (sin(ang1) * y);
    float b1 = (sin(ang1) * x) + (cos(ang1) * y);
    float a2 = (cos(ang2) * x) - (sin(ang2) * y);
    float b2 = (sin(ang2) * x) + (cos(ang2) * y);

    Punto *np1 = new Punto(p2->getX()+(factor*a1),p2->getY()+((factor*b1)));
    Punto *np2 = new Punto(p2->getX()+(factor*a2),p2->getY()+((factor*b2)));

    lista.push_back(np1);
    lista.push_back(np2);    

    for(int i = 0; i < lista.size(); i++)
      {
	Punto *l = lista.at(i);
	draw(next-1,p2,l,quitar);
      }
  }
} //Fin de Arbol.cpp
