/* walkingDog.c
 */
#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>
#include "dogParts.h"


unsigned char	mouseFlag = FALSE;			// flag for moving or not
int				mouseXStart, mouseYStart;	// start position when drag begins
double			mouseXAngle = 0.0, mouseYAngle = 0.0;
double			mouseDistance = 0.7;
//double     M_PI = 3.1416;
#define deg2rad(deg) deg*(3.1416)/180.0

unsigned char	moveFlag = FALSE;
int				frameCounter = 0;
int				keyFrameInterval = 10, keyFrameNumber = 7;
int				walkCounter = 0;			// count up it when dog walks

void generateAngleSub(int fc, double the[2][3], unsigned char *flag)
{
	int		i, j, mm;
	double	dd = 1/(double)keyFrameInterval;       
	
	if (fc >=   0 && fc < keyFrameInterval){
		mm = fc;
		the[0][0] = 45;
		the[0][1] = -90 + (double)(-100-(-90))*mm*dd;
		the[0][2] = 45;
		
		the[1][0] = -45;
		the[1][1] =  90;
		the[1][2] = -45;

		*flag = FALSE;
	}
	if (fc >= keyFrameInterval && fc < 2*keyFrameInterval){
		mm = fc- keyFrameInterval;
		the[0][0] =   45 + (double)(-45-(45))*mm*dd;
		the[0][1] = -100 + (double)(-20-(-100))*mm*dd;
		the[0][2] =  45;
		
		the[1][0] = -45;
		the[1][1] =  90;
		the[1][2] = -45;

		*flag = FALSE;
	}
	if (fc >= 2*keyFrameInterval && fc < 3*keyFrameInterval){
		mm = fc - 2*keyFrameInterval;
		the[0][0] = - 45;
		the[0][1] = - 20 + (double)(0-(-20))*mm*dd;
		the[0][2] =  45;
		
		the[1][0] = -45;
		the[1][1] =  90;
		the[1][2] = -45;

		*flag = FALSE;
	}
	if (fc >= 3*keyFrameInterval && fc < 4*keyFrameInterval){
		mm = fc - 3*keyFrameInterval;
		the[0][0] = - 45 + (double)(45 - (-45))*mm*dd;
		the[0][1] =    0 + (double)(-90-(0))*mm*dd;
		the[0][2] =  45;
		
		the[1][0] = -45 + (double)(45 - (-45))*mm*dd;
		the[1][1] =  90 + (double)(0  - ( 90))*mm*dd;
		the[1][2] = -45;

		*flag = TRUE;
	}
	if (fc >= 4*keyFrameInterval && fc < 5*keyFrameInterval){
		mm = fc - 4*keyFrameInterval;
		the[0][0] = + 45;
		the[0][1] = - 90;
		the[0][2] =   45;

		the[1][0] =  45;
		the[1][1] =   0 + (double)(95  - (0))*mm*dd;
		the[1][2] = -45 + (double)(-60 - (-45))*mm*dd;		

		*flag = TRUE;
	}
	if (fc >= 5*keyFrameInterval && fc < 6*keyFrameInterval){
		mm = fc - 5*keyFrameInterval;
		the[0][0] = + 45;
		the[0][1] = - 90;
		the[0][2] =   45;

		the[1][0] =  45 + (double)(-65 - (45))*mm*dd;
		the[1][1] =  95;
		the[1][2] = -60;

		*flag = FALSE;
	}
	if (fc >= 6*keyFrameInterval && fc < 7*keyFrameInterval){
		mm = fc - 6*keyFrameInterval;
		the[0][0] = + 45;
		the[0][1] = - 90;
		the[0][2] =   45;

		the[1][0] = - 65 + (double)(-45 - (-65))*mm*dd;
		the[1][1] =   95 + (double)( 90 - (95))*mm*dd;
		the[1][2] = - 60 + (double)(-45 - (-60))*mm*dd;

		*flag = TRUE;
	}
}

void generateAngleOfLegParts(double the[4][3])
{
  int		i, j, fc, fc2; //Variable propia fc2
	double	tt[2][3];
	unsigned char	flag_left, flag_right;

	fc2 = fc = frameCounter % (keyFrameNumber*keyFrameInterval);

	generateAngleSub(fc, tt, &flag_left);
	for (j = 0 ; j <=2 ; j++ ){
		the[0][j] = tt[0][j];
		the[1][j] = tt[0][j]; //Yo coloqué esta
		
		the[2][j] = tt[1][j];  //Yo coloqué esta
		the[3][j] = tt[1][j];
	}
	/*	To shift in half period */
	fc = (frameCounter + (keyFrameNumber*keyFrameInterval)/7) %
			(keyFrameNumber*keyFrameInterval);
	generateAngleSub(fc, tt, &flag_right); 
	for (j = 0 ; j <=2 ; j++ ){
		the[1][j] = tt[0][j];
		generateAngleSub(fc2,tt, &flag_left); //Y esta. 
		the[2][j] = tt[1][j];
		the[0][j] = tt[0][j]; //Y esta
		generateAngleSub(fc,tt, &flag_right); //Y esta. 

		the[3][j] = tt[1][j]; // Y esta.
	}
	moveFlag = flag_left || flag_right;
}

/***********************************************************************
 * Floor where the dog walk around
 ***********************************************************************/
void walkFloor()
{
	int		i, j, idx, range = 20;
	double	x, z, d = 0.05;
	
	for (i = -range ; i <= range ; i++ ){
		x = i * d;
		for (j = -range ; j <= range ; j++){
			z = j * d;
			idx = 0;
			if ( (abs(i)%2) == (abs(j)%2) ) idx = 1;

			glBegin(GL_QUADS);
				if (idx) glColor3d(0.0, 0.6, 0.2);
				else	 glColor3d(0.6, 0.6, 0.6);
				glNormal3d(0.0, 1.0, 0.0);
				glVertex3d(x  , 0.0, z);
				glVertex3d(x  , 0.0, z+d);
				glVertex3d(x+d, 0.0, z+d);
				glVertex3d(x+d, 0.0, z);
			glEnd();
		}
	}
}

void xyzAxes(double length)
{
	glBegin(GL_LINES);
		glColor3d(1.0, 1.0, 0.0);	// yellow
		glVertex3d(0.0, 0.0, 0.0); glVertex3d(length, 0.0, 0.0);	//x-axis
		glColor3d(1.0, 0.0, 0.0);	// red
		glVertex3d(0.0, 0.0, 0.0); glVertex3d(0.0, length, 0.0);	//y-axis
		glColor3d(0.0, 0.0, 1.0);	// blue
		glVertex3d(0.0, 0.0, 0.0); glVertex3d(0.0, 0.0, length);	//z-axis
	glEnd();
}

void polarView(double distance, double twist, double elevation, double azimuth)
{
	glTranslated(0.0, 0.0, -distance);
	glRotated(-twist, 		0.0, 0.0, 1.0);
	glRotated( elevation, 	1.0, 0.0, 0.0);
	glRotated( azimuth, 	0.0, 1.0, 0.0);
}

void myDisplay(void)
{
/*
 * The array cr[4][3][3] represents coordinates
 * at the rotation center of each leg part. 
 */
	static const double	cr[4][3][3] = {
	{{ 0.04, 0.02, -0.04}, { 0.04, -0.05, -0.04}, { 0.04, -0.11, -0.04}}, //f,l
	{{-0.04, 0.02, -0.04}, {-0.04, -0.05, -0.04}, {-0.04, -0.11, -0.04}}, //f,r
	{{-0.04, 0.02, -0.16}, {-0.04, -0.05, -0.16}, {-0.04, -0.11, -0.16}}, //h,r
	{{ 0.04, 0.02, -0.16}, { 0.04, -0.05, -0.16}, { 0.04, -0.11, -0.16}}};//h,l
/*
 * The array len[4][3] represents lengths of each leg part. 
 */
	static double	len[4][3] ={{0.07,0.06,0.03},{0.07,0.06,0.03},
								{0.07,0.06,0.03},{0.07,0.06,0.03}};
/*
 * The array theta[4][3] represents angulars of each leg part. 
 */
	static double	theta[4][3];
/*
 * The parameters of the leg represents
 *		"fr" be fore leg,	"hd" be hind leg,
 *		"l"  be left, 		"r" be right,
 *		"0" be upper leg, 	"1" be lower leg, "2" be instep
 */
	static void		(*varLegPart[4][3])()={
					{dog_leg_fr_l0, dog_leg_fr_l1, dog_leg_fr_l2},
					{dog_leg_fr_r0, dog_leg_fr_r1, dog_leg_fr_r2},
					{dog_leg_hd_r0, dog_leg_hd_r1, dog_leg_hd_r2},
					{dog_leg_hd_l0, dog_leg_hd_l1, dog_leg_hd_l2}};
	int				leg, part;
	double			x, y, z, sumth;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glPushMatrix();
	polarView(mouseDistance, 0.0, mouseXAngle, mouseYAngle);
	
	walkFloor();
	xyzAxes(10.0);
/* lift up the body so that back of the foot may bound to ground. */
	glTranslated(0.0, 0.09, 0.0);

/* walkCounter means the position in walking */
	if (moveFlag) glTranslated(0.0, 0.0, 0.002*(walkCounter++));
	else          glTranslated(0.0, 0.0, 0.002*(walkCounter));

	dog_body();
	dog_ear();
	dog_eye();
	dog_face();
	dog_tail();

/* Generate Angles of each leg part */	
	generateAngleOfLegParts(theta);
	
/*
 *  leg = 0 -> fore left,  leg = 1 -> fore right
 *  leg = 2 -> hind right, leg = 3 -> hind left
 */
	for (leg = 0; leg <= 3; leg++){
		sumth = 0.0;
/*
 * part = 0 -> upper leg, part = 1 -> lower leg, part = 2 -> instep
 * The translation is considered in world coorinates.
 */
		for (part = 0; part <= 2; part++){
		  if (part == 0){
				sumth = theta[leg][part];
				glPushMatrix();
				glTranslated(0.0,  cr[leg][part][1], cr[leg][part][2]);			       
				glRotated(sumth, 1.0, 0.0, 0.0);
				glTranslated(0.0, -cr[leg][part][1],-cr[leg][part][2]);
				(*(varLegPart[leg][part]))();
				glPopMatrix();
				y = cr[leg][part][1] - len[leg][part]*cos(deg2rad(sumth));
				z = cr[leg][part][2] - len[leg][part]*sin(deg2rad(sumth));
				}
		  if (part == 1 ){
				sumth += theta[leg][part];
				glPushMatrix();
				glTranslated(0.0, y, z);
				glRotated(sumth, 1.0, 0.0, 0.0);
				glTranslated(0.0, -cr[leg][part][1],-cr[leg][part][2]);
				(*(varLegPart[leg][part]))();
				glPopMatrix();
				y = y - len[leg][part]*cos(deg2rad(sumth));
				z = z - len[leg][part]*sin(deg2rad(sumth));
				}
			if (part == 2 ){
				sumth += theta[leg][part];
				glPushMatrix();
				glTranslated(0.0, y, z);
				glRotated(sumth, 1.0, 0.0, 0.0);
				glTranslated(0.0, -cr[leg][part][1],-cr[leg][part][2]);
				(*(varLegPart[leg][part]))();
				glPopMatrix();
				}
		}
	}

	glPopMatrix();
	glutSwapBuffers();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

}

void myKeyboard(unsigned char key, int x, int y)
{
	if ( key == 27) exit (0);
}

void myMouseMotion(int x, int y)
{
	int		xdis, ydis;
	float	a = 0.5;

	if ( mouseFlag == GL_FALSE ) return;
	xdis = x - mouseXStart;
	ydis = y - mouseYStart;

// To match mouse's movement and its rotation axis

	if (mouseFlag == 0x01){
		mouseXAngle += (double)ydis *a;
		mouseYAngle += (double)xdis *a;
	}
	if (mouseFlag == 0x10){
		mouseDistance -= (double)ydis*0.01;
	}
	
	mouseXStart = x;
	mouseYStart = y;
}

void myMouseFunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN ){
		if (button == GLUT_LEFT_BUTTON){
			mouseXStart = x;
			mouseYStart = y;
			mouseFlag = 0x01;
		}
		if (button == GLUT_RIGHT_BUTTON){
			mouseXStart = x;
			mouseYStart = y;
			mouseFlag = 0x10;
		}
	}
	else {
		mouseFlag = GL_FALSE;
	}
}

void myTimer(int value)
{
	if (value == 1){
		glutTimerFunc(100, myTimer, 1);
		frameCounter++;
		glutPostRedisplay();
	}
}

void myReshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)width / (double)height, 0.1, 20000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0.0, 0.0, -0.5);
	gluLookAt(0.0, 0.4, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void myInit(char *progname)
{
	float light_position[] = {0.0, 1.0, 1.0, 0.0};
	float light_ambient[] = {.1, .1, .1, 1.0};
	float light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(progname);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_COLOR_MATERIAL);	//be needed to render the dog
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glEnable(GL_LIGHT0);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	myInit(argv[0]);

	glutKeyboardFunc(myKeyboard);
	glutTimerFunc(100, myTimer, 1);
	glutMouseFunc(myMouseFunc);
	glutMotionFunc(myMouseMotion);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);

	glutMainLoop();
	return 0;
}
