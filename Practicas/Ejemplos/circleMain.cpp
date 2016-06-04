#include "Circle.h"

Circle *c;

unsigned int noVertices = 10;

void drawScene(void)
{  
	c = new Circle(10, 50, 50, noVertices);
	c->Draw();
	glFlush();
	c->~Circle();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case '+':
		++noVertices;
		glutPostRedisplay();
		break;
	case '-':
		if (noVertices > 3){ 
			--noVertices;
			glutPostRedisplay();
		}
		
		break;
	default:
		break;
	}
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "Interaction:" << endl;
	cout << "Press +/- to increase/decrease the number of vertices on the circle." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
  
   glutInit(&argc, argv);

   glutInitContextVersion(3, 0);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("circle.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   
   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
  
   glutMainLoop(); 
}
