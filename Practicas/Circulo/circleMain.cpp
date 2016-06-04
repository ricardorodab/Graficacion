#include "Circle.cpp"

Circle * c;

void drawScene(void)
{  
	c = new Circle(10, 50, 50, 50);
	c->Draw();
	glFlush();
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

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
  
   glutMainLoop(); 
}
