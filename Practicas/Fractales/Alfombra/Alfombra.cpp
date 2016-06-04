/* -------------------------------------------------------------------
 * Alfombra.cpp
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
#include "Alfombra.h"

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
 * Implementación del método contructor del fractal Alfombra.
 */
Alfombra::Alfombra(int level,Punto *p1, Punto *p2, Punto *p3, Punto *p4)
{
  this->level = level;
  this->p1 = p1;
  this->p2 = p2;
  this->p3 = p3;
  this->p4 = p4;
}

/**
 * Método que nos regresa el nivel de iteración del fractal.
 */
int Alfombra::getLevel()
{
  return this->level;
}

/**
 * Método para asignar el nivel de recursión del fractal.
 */
void Alfombra::setLevel(int level)
{
  this->level = level;
}

/**
 * Método que dibuja a el fractal en la pantalla.
 */
void Alfombra::draw(int next)
{
  this->level = next;
  float dif = (sqrt(pow(this->p1->getY()-this->p2->getY(),2)
		    +pow(this->p1->getX()-this->p2->getX(),2)))/3;
  draw(next,this->p1,this->p2,this->p3,this->p4,dif,false);

}

/**
 * Método para dibujar de blanco el último nivel del fractal.
 */
void Alfombra::quita_ultimo_nivel(int next)
{
  this->level = next;
  float dif = (sqrt(pow(this->p1->getY()-this->p2->getY(),2)
		    +pow(this->p1->getX()-this->p2->getX(),2)))/3;
  draw(next,this->p1,this->p2,this->p3,this->p4,dif,true);
  
}

/**
 * Método privado para dibujar los niveles del fractal.
 */
void Alfombra::draw(int next,Punto *p1, Punto *p2, Punto *p3, Punto *p4,float dif,bool quitar)
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
  glBegin(GL_POLYGON);     
  glVertex2f(p1->getX(),p1->getY());
  glVertex2f(p2->getX(),p2->getY());
  glVertex2f(p3->getX(),p3->getY());
  glVertex2f(p4->getX(),p4->getY());
  glEnd();
  glFlush();
  
  if(next > 1){
    float sum = sqrt(pow(p1->getY()-p2->getY(),2)+
		     pow(p1->getX()-p2->getX(),2));
    dif = sum/3;
    vector<Punto*> lista;
    Punto *c1 = new Punto(p1->getX()-(2*dif),(p1->getY()-(sum/2)));    
    Punto *c2 = new Punto(p1->getX()-(2*dif),p1->getY()+(2*dif));
    Punto *c3 = new Punto(p1->getX()+(sum/2),p1->getY()+(2*dif));
    Punto *c4 = new Punto(p2->getX()+(2*dif),p2->getY()+(2*dif));
    Punto *c5 = new Punto(p2->getX()+(2*dif),(p2->getY()-(sum/2)));
    Punto *c6 = new Punto(p3->getX()+(2*dif),p3->getY()-(2*dif));
    Punto *c7 = new Punto(p4->getX()+(sum/2),p4->getY()-(2*dif));
    Punto *c8 = new Punto(p4->getX()-(2*dif),p3->getY()-(2*dif));
    lista.push_back(c1);
    lista.push_back(c2);
    lista.push_back(c3); 
    lista.push_back(c4);
    lista.push_back(c5);
    lista.push_back(c6);
    lista.push_back(c7);
    lista.push_back(c8);
    for(int i = 0; i < lista.size(); i++)
      {
	Punto *c = lista.at(i);
	draw(next-1,
	     new Punto((c->getX())-(dif/2),(c->getY())+(dif/2)),
	     new Punto((c->getX())+(dif/2),(c->getY())+(dif/2)),
	     new Punto((c->getX())+(dif/2),(c->getY())-(dif/2)),
	     new Punto((c->getX())-(dif/2),(c->getY())-(dif/2)),dif,quitar);
      }
  }
} //Fin de Alfombra.cpp
