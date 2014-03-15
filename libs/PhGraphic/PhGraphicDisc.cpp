#include "PhGraphicDisc.h"

PhGraphicDisc::PhGraphicDisc(int x, int y, int radius, int resolution)
	: PhGraphicObject(x, y),
	_radius(radius),
	_resolution(resolution)
{

}

void PhGraphicDisc::draw()
{
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3f( _x, _y, _z);
		for( int i = 0; i <= _resolution; i++ ) {
			float angle = i * 2 * M_PI / _resolution;
			glVertex3f( _x + sin( angle ) * _radius, _y + cos( angle ) * _radius, _z);
		}
	}
	glEnd();
}