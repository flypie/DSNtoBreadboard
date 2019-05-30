#pragma once

#include "framework.h"
#include "commdlg.h"

#include "shobjidl_core.h"

#include <gdiplus.h>
using namespace Gdiplus;

#include "DSNFile.h"


typedef struct
{
	double x,y;
} vertex;

class MSWDSNFile :
	public DSNFile
{
public:
	MSWDSNFile(wstring a);


	void Paint(HDC hdc);

protected:

private:
	void DrawBlob(const wstring &Str);
	void DrawComponentImage(const Element& Image,double xoff,double yoff,double angle);
	void DrawComponentOfType(const Element& Part,const Element& Image);
	void DrawPCBOutline(const Element& path);
	
	
	void DrawLine(Pen &pen,double x1,double y1,double x2,double y2);
	void DrawCircle(Pen& pen,double x1,double y1,double radius);
	void DrawRectangle(Pen& pen,double x1,double y1,double x2,double y2);
	void DrawPaths(const double xoff,const double yoff,const Element& path);

	double scalef;

	double gscale,gxoff,gyoff;

	int	sheight,swidth;


	Graphics *graphics;

};

