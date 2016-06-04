#include <cstdlib>
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#ifdef __APPLE__
# define __gl_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif
//#include "Dependencies\freeglut\freeglut.h"// cambienlo de acuerdo a como ustedes lo tengan
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GL/freeglut.h>

//#include "Dependencies\glew\glew.h"// cambienlo de acuerdo a como ustedes lo tengan
//#include "Dependencies\freeglut\freeglut.h"// cambienlo de acuerdo a como ustedes lo tengan

#define PI 3.14159265358979324

using namespace std;

class Circle{

	public:
		unsigned int radius;
		float centerX;
		float centerY;
		float *vertices;
		unsigned int numVertices;

		Circle(unsigned int radius, float cx, float cy, unsigned int numertices);
		~Circle();
		void Draw(void);

};
