#include "software_renderer.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>

#include "triangulation.h"

using namespace std;

namespace CMU462 {


// Implements SoftwareRenderer //

void SoftwareRendererImp::draw_svg( SVG& svg ) {

  // set top level transformation
  transformation = canvas_to_screen;

  // draw all elements
  for ( size_t i = 0; i < svg.elements.size(); ++i ) {
    draw_element(svg.elements[i]);
  }

  // draw canvas outline
  Vector2D a = transform(Vector2D(    0    ,     0    )); a.x--; a.y++;
  Vector2D b = transform(Vector2D(svg.width,     0    )); b.x++; b.y++;
  Vector2D c = transform(Vector2D(    0    ,svg.height)); c.x--; c.y--;
  Vector2D d = transform(Vector2D(svg.width,svg.height)); d.x++; d.y--;

  rasterize_line(a.x, a.y, b.x, b.y, Color::Black);
  rasterize_line(a.x, a.y, c.x, c.y, Color::Black);
  rasterize_line(d.x, d.y, b.x, b.y, Color::Black);
  rasterize_line(d.x, d.y, c.x, c.y, Color::Black);

  // resolve and send to render target
  resolve();

}

void SoftwareRendererImp::set_sample_rate( size_t sample_rate ) {

  // Task 3: 
  // You may want to modify this for supersampling support
  this->sample_rate = sample_rate;

}

void SoftwareRendererImp::set_render_target( unsigned char* render_target,
                                             size_t width, size_t height ) {

  // Task 3: 
  // You may want to modify this for supersampling support
  this->render_target = render_target;
  this->target_w = width;
  this->target_h = height;

  this->sample_buffer.resize(this->sample_rate *  target_w *  this->sample_rate * target_h * 4, 255);
  this->sample_width = this->sample_rate * width;
  this->sample_height = this->sample_rate * height;
}

void SoftwareRendererImp::draw_element( SVGElement* element ) {

  // Task 4 (part 1):
  // Modify this to implement the transformation stack
  transformation = transformation * element->transform;

  switch(element->type) {
    case POINT:
      draw_point(static_cast<Point&>(*element));
      break;
    case LINE:
      draw_line(static_cast<Line&>(*element));
      break;
    case POLYLINE:
      draw_polyline(static_cast<Polyline&>(*element));
      break;
    case RECT:
      draw_rect(static_cast<Rect&>(*element));
      break;
    case POLYGON:
      draw_polygon(static_cast<Polygon&>(*element));
      break;
    case ELLIPSE:
      draw_ellipse(static_cast<Ellipse&>(*element));
      break;
    case IMAGE:
      draw_image(static_cast<Image&>(*element));
      break;
    case GROUP:
      draw_group(static_cast<Group&>(*element));
      break;
    default:
      break;
  }
  transformation = transformation * element->transform.inv();

}



// Primitive Drawing //

void SoftwareRendererImp::draw_point( Point& point ) {

  Vector2D p = transform(point.position);
  rasterize_point( p.x, p.y, point.style.fillColor );
  //muestra(p.x,p.y,p.x,p.y,point.style.fillColor);
}

void SoftwareRendererImp::draw_line( Line& line ) { 

  Vector2D p0 = transform(line.from);
  Vector2D p1 = transform(line.to);
  rasterize_line( p0.x, p0.y, p1.x, p1.y, line.style.strokeColor );

}

void SoftwareRendererImp::draw_polyline( Polyline& polyline ) {

  Color c = polyline.style.strokeColor;

  if( c.a != 0 ) {
    int nPoints = polyline.points.size();
    for( int i = 0; i < nPoints - 1; i++ ) {
      Vector2D p0 = transform(polyline.points[(i+0) % nPoints]);
      Vector2D p1 = transform(polyline.points[(i+1) % nPoints]);
      rasterize_line( p0.x, p0.y, p1.x, p1.y, c );
    }
  }
}

void SoftwareRendererImp::draw_rect( Rect& rect ) {

  Color c;
  
  // draw as two triangles
  float x = rect.position.x;
  float y = rect.position.y;
  float w = rect.dimension.x;
  float h = rect.dimension.y;

  Vector2D p0 = transform(Vector2D(   x   ,   y   ));
  Vector2D p1 = transform(Vector2D( x + w ,   y   ));
  Vector2D p2 = transform(Vector2D(   x   , y + h ));
  Vector2D p3 = transform(Vector2D( x + w , y + h ));
  
  // draw fill
  c = rect.style.fillColor;
  if (c.a != 0 ) {
    rasterize_triangle( p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, c );
    rasterize_triangle( p2.x, p2.y, p1.x, p1.y, p3.x, p3.y, c );
  }

  // draw outline
  c = rect.style.strokeColor;
  if( c.a != 0 ) {
    rasterize_line( p0.x, p0.y, p1.x, p1.y, c );
    rasterize_line( p1.x, p1.y, p3.x, p3.y, c );
    rasterize_line( p3.x, p3.y, p2.x, p2.y, c );
    rasterize_line( p2.x, p2.y, p0.x, p0.y, c );
  }

}

void SoftwareRendererImp::draw_polygon( Polygon& polygon ) {

  Color c;

  // draw fill
  c = polygon.style.fillColor;
  if( c.a != 0 ) {

    // triangulate
    vector<Vector2D> triangles;
    triangulate( polygon, triangles );

    // draw as triangles
    for (size_t i = 0; i < triangles.size(); i += 3) {
      Vector2D p0 = transform(triangles[i + 0]);
      Vector2D p1 = transform(triangles[i + 1]);
      Vector2D p2 = transform(triangles[i + 2]);
      rasterize_triangle( p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, c );
    }
  }

  // draw outline
  c = polygon.style.strokeColor;
  if( c.a != 0 ) {
    int nPoints = polygon.points.size();
    for( int i = 0; i < nPoints; i++ ) {
      Vector2D p0 = transform(polygon.points[(i+0) % nPoints]);
      Vector2D p1 = transform(polygon.points[(i+1) % nPoints]);
      rasterize_line( p0.x, p0.y, p1.x, p1.y, c );
    }
  }
}

void SoftwareRendererImp::draw_ellipse( Ellipse& ellipse ) {

  // Extra credit 

}

void SoftwareRendererImp::draw_image( Image& image ) {

  Vector2D p0 = transform(image.position);
  Vector2D p1 = transform(image.position + image.dimension);

  rasterize_image( p0.x, p0.y, p1.x, p1.y, image.tex );
}

void SoftwareRendererImp::draw_group( Group& group ) {

  for ( size_t i = 0; i < group.elements.size(); ++i ) {
    draw_element(group.elements[i]);
  }

}

// Rasterization //

// The input arguments in the rasterization functions 
// below are all defined in screen space coordinates

void SoftwareRendererImp::rasterize_point( float x, float y, Color color ) {

  // fill in the nearest pixel
  int sx = (int) floor(x);
  int sy = (int) floor(y);

  // check bounds
  if ( sx < 0 || sx >= target_w ) return;
  if ( sy < 0 || sy >= target_h ) return;

  // fill sample - NOT doing alpha blending!
  render_target[4 * (sx + sy * target_w)    ] = (uint8_t) (color.r * 255);
  render_target[4 * (sx + sy * target_w) + 1] = (uint8_t) (color.g * 255);
  render_target[4 * (sx + sy * target_w) + 2] = (uint8_t) (color.b * 255);
  render_target[4 * (sx + sy * target_w) + 3] = (uint8_t) (color.a * 255);

}

void SoftwareRendererImp::rasterize_line( float x0, float y0,
                                          float x1, float y1,
                                          Color color) {

  // Task 1: 
  // Implement line rasterization
	//wikipedia español, el de ingles no sirve	
	int dx, dy, i, deci;
    int incx, incy, inc1, inc2;
    int x,y;

    dx = x1 - x0;
    dy = y1 - y0;

    if(dx < 0) 
    	dx = -dx;
    if(dy < 0) 
    	dy = -dy;

    incx = 1;
    if(x1 < x0) 
    	incx = -1;

    incy = 1;
    if(y1 < y0) 
    	incy = -1;

    x=x0;
    y=y0;

    if(dx > dy){
        rasterize_point(x,y, color);
        //muestra(x,y,x,y,color);
        deci = 2*dy - dx;
        inc1 = 2*( dy -dx);
        inc2 = 2*dy;
        for(i = 0; i < dx; i++)
        {
            if(deci >= 0)
            {
                y += incy;
                deci += inc1;
            }
            else deci += inc2;
            x += incx;
            rasterize_point(x,y, color);
            //muestra(x,y,x,y,color);
        }
    }
    else
    {
        rasterize_point(x,y, color);
        //muestra(x,y,x,y,color);
        deci = 2*dx - dy;
        inc1 = 2*( dx - dy);
        inc2 = 2*dx;
        for(i = 0; i < dy; i++)
        {
            if(deci >= 0)
            {
                x += incx;
                deci += inc1;
            }
            else deci += inc2;
            y += incy;
            
            rasterize_point(x,y, color);
            //muestra(x,y,x,y,color);
        }
    }
}

float SoftwareRendererImp::maximo(float x0, float x1, float x2)
{
  float max = x0;
  if(x1 > max)
    max = x1;
  if(x2 > max)
    max = x2;
  return max;
}

float SoftwareRendererImp::minimo(float x0, float x1, float x2)
{
  float min = x0;
  if(x1 < min)
    min = x1;
  if(x2 < min)
    min = x2;
  return min;
}

void SoftwareRendererImp::muestra(float x, float y, float ix, float iy, Color color)
{
  //Igual que raster point pero aqui guardamos el "buffer" de muestras de color que nos pide la tarea 3 para un mejor efecto
  int sx = (int) floor(x);
  int sy = (int) floor(y);

  if ( sx < 0 || sx >= target_w ) return;
  if ( sy < 0 || sy >= target_h ) return;

  int a = 4 * (ix + sx + (sy + iy) * target_w);
  sample_buffer[a] = (uint8_t) (color.r * 255);
  sample_buffer[a + 1] = (uint8_t) (color.g * 255);
  sample_buffer[a + 2] = (uint8_t) (color.b * 255);
  sample_buffer[a + 3] = (uint8_t) (color.a * 255);
}

float SoftwareRendererImp::determinante(float a,float b,float c,float d){
	return (a*d) - (b*c);
}

bool SoftwareRendererImp::checa_frontera( float x0, float y0, float x1, float y1, float x2, float y2, float x, float y){
  float det1, det2, det3;

  float ax = x1 - x0;
  float ay = y1 - y0;
  float bx = x - x0;
  float by = y - y0;
  det1 = determinante(ax,ay,bx,by);

  ax = x2 - x1;
  ay = y2 - y1;
  bx = x - x1;
  by = y - y1;
  det2 = determinante(ax,ay,bx,by);

  ax = x0 - x2;
  ay = y0 - y2;
  bx = x - x2;
  by = y - y2;
  det3 = determinante(ax,ay,bx,by);

  if(det1 == 0.0 || det2 == 0.0 || det3 == 0.0){
    return true;
  } 
  else if(det1 < 0.0 && det2 < 0.0 && det3 < 0.0){
    return true;
  }
  else if(det1 > 0.0 && det2 > 0.0 && det3 > 0.0){
    return true;
  }
  return false;
}

void SoftwareRendererImp::rasterize_triangle( float x0, float y0,
                                              float x1, float y1,
                                              float x2, float y2,
                                              Color color ) {
  // Task 2: 
  // Implement triangle rasterization
	int minX = minimo(x0, x1, x2);
    int minY = minimo(y0, y1, y2);
    int maxX = maximo(x0, x1, x2);
    int maxY = maximo(y0, y1, y2);

    //minX = max(minX, 0);
    float x, y;
    rasterize_line(x0,y0,x1,y1,color);
    rasterize_line(x1,y1,x2,y2,color);
    rasterize_line(x0,y0,x2,y2,color);
  	x = minX;
  	while(x <= maxX){
    	y = minY;
    	while(y <= maxY){
      /* samples per pixel */
      		float i = 0.5 / this->sample_rate;
      		for(int ix = 0; ix < this->sample_rate; ix++){
        		for(int iy = 0; iy < this->sample_rate; iy++){
          			if(checa_frontera(x0, y0, x1, y1, x2, y2, x + (0.5 * ix + i), y + (0.5 * iy + i)))
          			//if(inside_triangle(x0, y0, x1, y1, x2, y2, x, y))
            			//muestra(x, y, ix, iy, color);
            			rasterize_point(x,y,color);
            	}
      		}
      		y += 1;
    	}
    	x += 1;
  	}
}

void SoftwareRendererImp::rasterize_image( float x0, float y0,
                                           float x1, float y1,
                                           Texture& tex ) {
  // Task ?: 
  // Implement image rasterization

}

// resolve samples to render target

void SoftwareRendererImp::resolve( void ) {

  // Task 3: 
  // Implement supersampling
  // You may also need to modify other functions marked with "Task 3".
  /*
  int supers = this->sample_rate * this->sample_rate;

  for(int x = 0; x <= target_w; x++)
  {
    for (int y = 0; y <= target_h; y++) {
      float fill = 0;
      int cred = 255;
      int cgreen = 255;
      int cblue = 255;

      // Revisar supersampling 
      for(int ix = 0; ix < this->sample_rate; ix++)
      {
        for (int iy = 0; iy < this->sample_rate; iy++) {
          // Color de la muestra 
          int index = 4 * (x + ix + (y + iy) * target_w);
          if(sample_buffer[index] != 255 || sample_buffer[index + 1] != 255
              || sample_buffer[index + 2] != 255){
            // No es blanco.. 
            fill++;
            cred = sample_buffer[index];
            cgreen = sample_buffer[index + 1];
            cblue = sample_buffer[index + 2];
          }
        }
      }

      // Intensidad del color 
      float ity = 1 - fill/supers; // todos llenos: 0 => no cambio, ninguno lleno: 1 => blanco 
      render_target[4 * (x + y * target_w)    ] = (uint8_t) cred;
      render_target[4 * (x + y * target_w) + 1] = (uint8_t) cgreen;
      render_target[4 * (x + y * target_w) + 2] = (uint8_t) cblue;
      render_target[4 * (x + y * target_w) + 3] = (uint8_t) 255;
    }
  }

*/
}


} // namespace CMU462
