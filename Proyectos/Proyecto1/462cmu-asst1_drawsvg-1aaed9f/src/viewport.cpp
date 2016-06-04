#include "viewport.h"

#include "CMU462.h"

namespace CMU462 {

void ViewportImp::set_viewbox( float x, float y, float span ) {

  // Task 4 (part 2): 
  // Set svg to normalized device coordinate transformation. Your input
  // arguments are defined as SVG canvans coordinates.
  /*  this->x = x;
  this->y = y;
  this->span = span; */
  /*
  x = 200.0;
  y = 150.0;
  span = 10.0;
  printf("x = %f, y = %f, span = %f\n", this->x, this->y, span);
  */
  //printf("x = %f, y = %f, span = %f\n", x, y, span);

  // Task 4 (part 2):
  // Set svg to normalized device coordinate transformation. Your input
  // arguments are defined as SVG canvans coordinates.

  //std::cout << svg_2_norm << std::endl;
  /*
   * scale: r = 1 / (span / 1.2) * 2), assuming canvas origin is (0, 0) top L
   * translat (x - x / 2r, y - y / 2r), view point at canvas center
   */

  //float new_span = span / 1.2;
  float new_span = span ;
  float normx = (x - new_span);
  float normy = (y - new_span);
  //printf("(normx, normy) = (%f, %f)\n", normx, normy);

  Matrix3x3 translate;
  translate(0,0) = 1.0; translate(0,1) = 0.0; translate(0,2) = (-1.0)*normx;
  translate(1,0) = 0.0; translate(1,1) = 1.0; translate(1,2) = (-1.0)*normy;
  translate(2,0) = 0.0; translate(2,1) = 0.0; translate(2,2) = 1.0;
  //std::cout << "translate: \n" << translate << std::endl;

  float view_w = new_span * 2;
  float r = 1 / view_w;
  Matrix3x3 scale;
  scale(0,0) = r ; scale(0,1) = 0.0; scale(0,2) = 0.0;
  scale(1,0) = 0.0; scale(1,1) = r ; scale(1,2) = 0.0;
  scale(2,0) = 0.0; scale(2,1) = 0.0; scale(2,2) = 1.0;
  //std::cout << "scale: \n" << scale << std::endl;

  svg_2_norm = scale * translate;
  //std::cout << "overall: \n" << svg_2_norm << std::endl;

  /*
  Vector3D p1 ( 190.0, 140.0, 1.0 );
  std::cout << svg_2_norm * p1 << std::endl;
  Vector3D p2 ( 210.0, 160.0, 1.0 );
  std::cout << svg_2_norm * p2 << std::endl;
  */
  this->x = x;
  this->y = y;
  this->span = span;

  //printf("x = %f, y = %f, span = %f\n", x, y, span);

}

void ViewportImp::update_viewbox( float dx, float dy, float scale ) { 
  
  this->x -= dx;
  this->y -= dy;
  this->span *= scale;
  set_viewbox( x, y, span );
}

} // namespace CMU462
