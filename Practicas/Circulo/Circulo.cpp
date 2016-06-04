#include "Circle.h"

Circle::Circle(unsigned int radius, float cx, float cy, unsigned int numvertices)
{
	this->radius = radius;
	centerX = cx;
	centerY = cy;
	numVertices = numvertices;
	vertices = new float[2 * numVertices];
	float t = 0; // Angle parameter.
	int i;
	for (i = 0; i < numVertices * 2; i+=2)
	{
		vertices[i] = centerX + radius * cos(t);
		vertices[i+1] = centerY + radius * sin(t);
		t += 2 * PI / numVertices;
	}
}

Circle::~Circle(){
	delete[] vertices;
}

void Circle::Draw(void){
	
	int i;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	//glBegin(GL_LINE_LOOP); //Original
	glBegin(GL_TRIANGLE_FAN); //Modificado
	for (i = 0; i < numVertices * 2; i+=2)
	{
		glVertex2f(vertices[i], vertices[i+1]);
	}
	glEnd();
	
}

