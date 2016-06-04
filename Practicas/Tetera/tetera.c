#include <GL/glew.h>
#ifdef __APPLE__
# define __gl_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GL/freeglut.h>
#include <GL/glut.h>




#include <stdlib.h>
#include <math.h>
static int angulo = 0;
void myInit(char *progname)
{
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(500, 500); 
  glutInitWindowPosition(0, 0);
  glutCreateWindow(progname);
  glClearColor(0.0, 0.0, 0.0, 0.0);	
}

void myDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glPushMatrix();
  glTranslated(1.0-((double)-1), 0.0, 0.0);
  glTranslated(0,1.0,0);
  glRotated((double)angulo, 0.0, 1.0, 0.0);
  glutWireTeapot(1.0);
  glTranslated(0,-1.2,0);
  glRotated(90, 0.0, 0.0, 1.0);
  glPushMatrix();
  glScaled(1.0, 2.4, 2.5);
  glutWireCube(1.0);
  //glutSolidCube(1.0);
  glPopMatrix();

  glPopMatrix();
  glutSwapBuffers();
}

void myReshape (int width, int height)
{
	glViewport(0, 0, width, height); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)width/(double)height, 0.1, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(-2.0, 0.0, -5.0);				// move to enable viewing
}

void myKeyboard (unsigned char key, int x, int y)
{
  switch (key) {
  case 's':
    angulo = (angulo + 5) % 360;
    glutPostRedisplay();
    break;
  case 'd':
    angulo = (angulo - 5) % 360;
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
    break;
  default:
    break;
  }
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutKeyboardFunc(myKeyboard);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay); 
	glutMainLoop();
	return 0;
}

