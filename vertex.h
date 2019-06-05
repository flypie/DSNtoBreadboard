#pragma once

#include <math.h>



#define PI 3.14159265

class vertex
{
public:

	void Rotate(double angle)
	{
		double cangle=cos(angle*PI/180.0);
		double sangle=sin(angle*PI/180.0);

		double xin=x,yin=y;
		x=xin*cangle-yin*sangle;
		y=xin*sangle+yin*cangle;
	};

	vertex RotateA(double angle)
	{
		vertex temp=*this;

		temp.Rotate(angle);

		return temp;
	};

	vertex operator+(const vertex R)
	{
		vertex copy;

		copy.x=this->x+R.x;
		copy.y=this->y+R.y;

		return copy;
	};

	vertex& operator+=(const vertex R)
	{
		this->x+=R.x;
		this->y+=R.y;

		return *this;
	};

	double x,y;
};
