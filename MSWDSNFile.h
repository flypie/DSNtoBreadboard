#pragma once

#include "framework.h"
#include "commdlg.h"

#include "shobjidl_core.h"

#include <gdiplus.h>
using namespace Gdiplus;

#include "DSNFile.h"

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

	vertex &operator+=(const vertex R)
	{
		this->x+=R.x;
		this->y+=R.y;

		return *this;
	};

	double x,y;
};

class MSWDSNFile :
	public DSNFile
{
public:
	MSWDSNFile(wstring a);


	void Paint(HDC hdc);

protected:

private:
	void DrawBlob(const wstring &Str);
	void DrawComponentImage(const Element& Image,vertex xy,double angle);
	void DrawComponentOfType(const Element& Part,const Element& Image);
	void DrawPCBOutline(const Element& path);

	void DrawLine(Pen &pen,vertex & a,vertex & b);
	void DrawCircle(Pen& pen,vertex& a,double radius);
	void DrawRectangle(Pen& pen,vertex& a,vertex& b);

	double scalef;

	double gscale,gxoff,gyoff;

	int	sheight,swidth;


	Graphics *graphics;

};

