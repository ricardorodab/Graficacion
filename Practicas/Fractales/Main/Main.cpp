/* -------------------------------------------------------------------
 * Main.cpp
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

//#include "Alfombra.cpp"
#include "Arbol.cpp"
#define tamanio 500

static int generacion_alfombra = 0;
static int generacion_arbol = 0;

static Alfombra * fractal_alfombra;
static Arbol * fractal_arbol;
static bool barra_atr;

void drawScene(void)
{
  barra_atr = true;
  glClear(GL_COLOR_BUFFER_BIT);
  Punto * p4 = new Punto(-25,-25);
  Punto * p3 = new Punto(25,-25);
  Punto * p1 = new Punto(-25,25);
  Punto * p2 = new Punto(25,25);
  fractal_alfombra = new Alfombra(0,p1,p2,p3,p4); 
  fractal_alfombra->draw(generacion_alfombra);
  Punto * p1_ar = new Punto(0,-100);
  Punto * p2_ar = new Punto(0,-20);
  fractal_arbol = new Arbol(0,p1_ar,p2_ar); 
  fractal_arbol->draw_arbol(generacion_arbol);

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

void keySpecial (int key, int x, int y)
{
  switch (key){
  case GLUT_KEY_RIGHT:
    if(barra_atr)
      fractal_alfombra->draw(++generacion_alfombra);
    else
      fractal_arbol->draw_arbol(++generacion_arbol);
    break;
  case GLUT_KEY_LEFT:
    if(generacion_alfombra > 0 ){
      fractal_alfombra->quita_ultimo_nivel(generacion_alfombra--);
    }if(generacion_arbol > 0 ){
      fractal_arbol->quita_ultimo_nivel_arbol(generacion_arbol--);
    }
    break;
  }
}

void barra(bool b)
{
  if(b){
    glClear(GL_COLOR_BUFFER_BIT);
    fractal_alfombra->setLevel(0);
    generacion_alfombra = 0;
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    }else{
    glClear(GL_COLOR_BUFFER_BIT);
    fractal_arbol->setLevel_arbol(0);
    generacion_arbol = 0;
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    }
}

   
void keyboard (unsigned char key, int x, int y)
{
  switch (key){
  case '+':
    if(barra_atr)
      fractal_alfombra->draw(++generacion_alfombra);
    else
      fractal_arbol->draw_arbol(++generacion_arbol);
    break;
  case '-':
    if(generacion_alfombra > 0 ){
      fractal_alfombra->quita_ultimo_nivel(generacion_alfombra--);
    }if(generacion_arbol > 0 ){
      fractal_arbol->quita_ultimo_nivel_arbol(generacion_arbol--);
    }
    break;
  case ' ':
    barra(barra_atr);
    barra_atr = !barra_atr;
    break;
  case 27:
    exit(0);
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
   glutCreateWindow("Fractales");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutSpecialFunc(keySpecial);
   glutKeyboardFunc(keyboard);
   glewExperimental = GL_TRUE;   
   glewInit();
   setup(); 
   glutMainLoop(); 
  
} //Fin de Main.cpp
