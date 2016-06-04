/* List: p3-robot.c
 * Robot arm with two links and two joints.
 * Examination of world-local coordinates, modeling transfomation and 
 * operation of matrix stack.
 */
#include <stdlib.h>
#include <GL/glut.h>

static int shoulder = 0, elbow = 0;

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
/* 1st link */
	glRotated((double)shoulder, 0.0, 0.0, 1.0);
	glTranslated(1.0, 0.0, 0.0);
	glPushMatrix();
	glScaled(2.0, 0.4, 1.0);
	glutWireCube(1.0);
	glPopMatrix();
/* 2nd link */
	glTranslated(1.0, 0.0, 0.0);				//move to the end of 1st link
	glRotated((double)elbow, 0.0, 0.0, 1.0);
	glTranslated(1.0, 0.0, 0.0);
	glPushMatrix();
	glScaled(2.0, 0.4, 1.0);
	glutWireCube(1.0);
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
			shoulder = (shoulder + 5) % 360;
			glutPostRedisplay();
			break;
		case 'S':
			shoulder = (shoulder - 5) % 360;
			glutPostRedisplay();
			break;
		case 'e':
			elbow = (elbow + 5) % 360;
			glutPostRedisplay();
			break;
		case 'E':
			elbow = (elbow - 5) % 360;
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
