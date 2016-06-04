/* -------------------------------------------------------------------
 * Alfombra_main.cpp
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

#include "Alfombra.cpp"
#define tamanio 500

static int generacion = 0;

static Alfombra * fractal;

void drawScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  Punto * p4 = new Punto(-25,-25);
  Punto * p3 = new Punto(25,-25);
  Punto * p1 = new Punto(-25,25);
  Punto * p2 = new Punto(25,25);
  fractal = new Alfombra(0,p1,p2,p3,p4); 
  fractal->draw(generacion);
  glFlush();
}

void setup(void)
{
  glClearColor(1.0,1.0,1.0,0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-100, 100.0, -100.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "Interaction:" << endl;
	cout << "Press +/- to increase/decrease the number of vertices on the circle." << endl;
}

void keySpecial (int key, int x, int y) {
  switch (key){
  case GLUT_KEY_RIGHT:
    fractal->draw(++generacion);
    break;
  case GLUT_KEY_LEFT:
    if(generacion > 0)
      fractal->quita_ultimo_nivel(generacion--);
    break;
  }
}  


int main(int argc, char **argv)
{
   glutInit(&argc, argv);

   glutInitContextVersion(3, 0);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
   glutInitWindowSize(tamanio, tamanio);
   glutInitWindowPosition(tamanio/2,tamanio/2); 
   glutCreateWindow("Alfombra de Sierpinski");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutSpecialFunc(keySpecial);
   glewExperimental = GL_TRUE;   
   glewInit();
   setup(); 
   glutMainLoop(); 
  
} //Fin de Alfombra_main.cpp
