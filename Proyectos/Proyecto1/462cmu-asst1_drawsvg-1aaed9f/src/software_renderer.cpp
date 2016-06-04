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
  size_t super_w, super_h, super_size;
  super_w = this->target_w * sample_rate;
  super_h = this->target_h * sample_rate;
  unsigned char* old_buf = this->render_target;
  unsigned char* super_buf = new unsigned char [super_w * super_h * 4];
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


  this->sample_buffer = super_buf;
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

}

void SoftwareRendererImp::draw_element( SVGElement* element ) {

  // Task 4 (part 1):
  // Modify this to implement the transformation stack

  /*switch(element->type) {
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
      }*/
  switch(element->type) {
  case POINT:
    {
      transformation =  transformation * (element->transform);
      draw_point(static_cast<Point&>(*element));
      transformation =  transformation * (element->transform.inv());
      break;
    }
  case LINE:
    {
      transformation =  transformation * (element->transform);
      draw_line(static_cast<Line&>(*element));
      transformation =  transformation * (element->transform.inv());
      break;
    }
  case POLYLINE:
    {
      transformation =  transformation * (element->transform);
      draw_polyline(static_cast<Polyline&>(*element));
      transformation =  transformation * (element->transform.inv());
      break;
    }
  case RECT:
    {
      transformation =  transformation * (element->transform);
      draw_rect(static_cast<Rect&>(*element));
      transformation =  transformation * (element->transform.inv());
      break;
    }
  case POLYGON:
    {
      transformation =  transformation * (element->transform);
      draw_polygon(static_cast<Polygon&>(*element));
      transformation =  transformation * (element->transform.inv());
      break;
    }
  case ELLIPSE:
    {
      transformation =  transformation * (element->transform);
      draw_ellipse(static_cast<Ellipse&>(*element));
      transformation =  transformation * (element->transform.inv());
      break;
    }
  case IMAGE:
    {
      transformation =  transformation * (element->transform);
      draw_image(static_cast<Image&>(*element));
      transformation =  transformation * (element->transform.inv());
      break;
    }
  case GROUP:
    {
      transformation =  transformation * (element->transform);
      draw_group(static_cast<Group&>(*element));
      transformation =  transformation * (element->transform.inv());
      break;
    }
  default:
    break;
  }

}


// Primitive Drawing //

void SoftwareRendererImp::draw_point( Point& point ) {

  Vector2D p = transform(point.position);
  rasterize_point( p.x, p.y, point.style.fillColor );

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
void SoftwareRendererImp::rasterize_point( float x, float y, Color color ) {

  int sx = (int) floor(x);
  int sy = (int) floor(y);

  if ( sx < 0 || sx >= target_w ) return;
  if ( sy < 0 || sy >= target_h ) return;

  render_target[4 * (sx + sy * target_w)    ] = (uint8_t) (color.r * 255);
  render_target[4 * (sx + sy * target_w) + 1] = (uint8_t) (color.g * 255);
  render_target[4 * (sx + sy * target_w) + 2] = (uint8_t) (color.b * 255);
  render_target[4 * (sx + sy * target_w) + 3] = (uint8_t) (color.a * 255);

}

  //Métodos auxiliares que encontré:
  void scale_point ( float& x, float& y, float r ) {
    x = r*x;
    y = r*y;
  }

  void swap (float& a, float& b) {
    float tmp = a;
    a = b;
    b = tmp;
  }
  void swapPoints(float& x0, float& y0, float& x1, float& y1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  bool lineSide(float x, float y,
		float x0, float y0, float x1, float y1) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    return (x - x0)*dy - (y - y0)*dx <= 0;
  }

  bool cclock( float x0, float y0,
	       float x1, float y1,
	       float x2, float y2 ) {
    Matrix3x3 m;
    m(0,0) = x0; m(0,1) = x1; m(0,2) = x2;
    m(1,0) = y0; m(1,1) = y1; m(1,2) = y2;
    m(2,0) = 1 ; m(2,1) = 1 ; m(2,2) = 1 ;
    if(m.det() >= 0.0)
      return true;
    return lineSide(x2 - x0, y2 - y0, x0, y0, x1, y1);
    
  } 

  //TERMINAN MÉTODOS AUXILIARES
  
void SoftwareRendererImp::rasterize_line( float x0, float y0,
                                          float x1, float y1,
                                          Color color) {

  // Task 1: 
  // Implement line rasterization  
  scale_point(x0, y0, sample_rate);
  scale_point(x1, y1, sample_rate);
  if (x0 > x1) {
    swapPoints(x0, y0, x1, y1);
  }
  int sx0 = (int) floor(x0);
  int sy0 = (int) floor(y0);
  int sx1 = (int) floor(x1);
  int sy1 = (int) floor(y1);

  float k;
  if (sx1 == sx0) {
    int low = min(sy0, sy1);
    int high = max(sy0, sy1);
    for ( int y = low; y <= high; y++ ) {
      SoftwareRendererImp::rasterize_point((float) x0, (float) y, color);
    }
  }
  
  else {
    k = (y1 - y0)/(x1 - x0);
    
    //fitst positive oct
    if (0.0 <= k && k <= 1.0) {
      int dx  = sx1 - sx0,
	dy  = sy1 - sy0,
	y   = sy0,
	eps = 0;
      
      for ( int x = sx0; x <= sx1; x++ )  {
	SoftwareRendererImp::rasterize_point((float) x, (float) y, color);
	eps += dy;
	if ( (eps << 1) >= dx )  {
	  y++;  eps -= dx;
	}
      }
    }
    if (k > 1.0 && sx1 != sx0) {
      int dx  = sx1 - sx0,
	dy  = sy1 - sy0,
	x   = sx0,
	eps = 0;

      for ( int y = sy0; y <= sy1; y++ )  {
	SoftwareRendererImp::rasterize_point((float) x, (float) y, color);
	eps += dx;
	if ( (eps << 1) >= dy )  {
	  x++;  eps -= dy;
	}
      }
    }

    if (k <= -1.0) {
      int dx  = sx1 - sx0,
	dy  = sy1 - sy0,
	x   = sx0,
	eps = 0;

      for ( int y = sy0; y >= sy1; y-- )  {
	SoftwareRendererImp::rasterize_point((float) x, (float) y, color);
	eps -= dx;
	if ( (eps << 1) <= dy )  {
	  x++;  eps -= dy;
	}
      }
    }

    if (-1.0 <= k && k <= 0.0) {
      int dx  = sx1 - sx0,
	dy  = sy1 - sy0,
	y   = sy0,
	eps = 0;

      for ( int x = sx0; x <= sx1; x++ )  {
	SoftwareRendererImp::rasterize_point((float) x, (float) y, color);
	eps += dy;
	if ( (eps << 1) <= -dx )  {
	  y--;  eps += dx;
	}
      }
    }
    } 
}

  

void SoftwareRendererImp::rasterize_triangle( float x0, float y0,
                                              float x1, float y1,
                                              float x2, float y2,
                                              Color color ) {
  // Task 2: 
  // Implement triangle rasterization
  //modify task 2: scale up the point location:
  
  scale_point(x0, y0, sample_rate);
  scale_point(x1, y1, sample_rate);
  scale_point(x2, y2, sample_rate);

  // Task 2:
  // Implement triangle rasterization
  if (!cclock(x0, y0, x1, y1, x2, y2)) {
    swapPoints(x1, y1, x2, y2);
  }

  float lowX, highX, lowY, highY;
  lowX = (floor(min(min(x0, x1), x2))) + 0.5;
  lowY = (floor(min(min(y0, y1), y2))) + 0.5;
  highX = (ceil(max(max(x0, x1), x2))) - 0.5;
  highY = (ceil(max(max(y0, y1), y2))) - 0.5;

  for ( float x = lowX; x <= highX; x += 1 ) {
    for (float y = lowY; y <= highY; y += 1 )  {
      if (lineSide(x, y, x0, y0, x1, y1) &&
	  lineSide(x, y, x1, y1, x2, y2) &&
	  lineSide(x, y, x2, y2, x0, y0)) {
	rasterize_point(x, y, color);
      }
    }
    }
}

void SoftwareRendererImp::rasterize_image( float x0, float y0,
                                           float x1, float y1,
                                           Texture& tex ) {
}

// resolve samples to render target
void SoftwareRendererImp::resolve( void ) {

  // Task 3: 
  // Implement supersampling
  // You may also need to modify other functions marked with "Task 3".
  return;
}


} // namespace CMU462
