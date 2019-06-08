#include "MSWTools.h"

#include <gdiplus.h>
using namespace Gdiplus;

#include <string>


extern void AddToLog(const std::wstring FileName);


Color GetMSWColour(DSNCOLOUR In)
{
	Color Ret=Color(255,0,0,0);

	switch(In)
	{
	case RED:
		Ret=Color(255,255,0,0);
		break;
	case GREEN:
		Ret=Color(255,0,255,0);
		break;
	case BLUE:
		Ret=Color(255,0,0,255);
		break;
	case YELLOW:
		Ret=Color(255,255,255,0);
		break;
	case CYAN:
		Ret=Color(255,0,255,255);
		break;
	case MAGENTA:
		Ret=Color(255,255,0,255);
		break;
	case WHITE:
		Ret=Color(255,255,255,255);
		break;
	}

return Ret;
}

MSWTools::MSWTools()
{
	gscalefromfile=0;
	gfileheight=0;
	gfilewidth=0;
	gxofffromfile=0;
	gyofffromfile=0;
}

void MSWTools::DrawLine(DSNPen& PenIn,vertex& a,vertex& b)
{
	vertex	xy1,xy2;
	DSNCOLOUR Colour;
	Pen	MSWPen(Color(255,0,0,255));
	double Width;

	xy1.x=(a.x-gxofffromfile)/gscalefromfile;
	if(GetFileOrientation()==FILE_IS_X_GOES_DOWN)
	{
		xy1.y=(a.y-gyofffromfile)/gscalefromfile;

	}
	else
	{
		xy1.y=(gfileheight-(a.y-gyofffromfile))/gscalefromfile;
	}
	
	xy2.x=(b.x-gxofffromfile)/gscalefromfile;
	if(GetFileOrientation()==FILE_IS_X_GOES_DOWN)
	{
		xy2.y=(b.y-gyofffromfile)/gscalefromfile;

	}
	else
	{
		xy2.y=(gfileheight-(b.y-gyofffromfile))/gscalefromfile;
	}

	REAL ix1,iy1,ix2,iy2;

	ix1=(REAL)(xy1.x*gscaletodevice);
	iy1=(REAL)(xy1.y*gscaletodevice);
	ix2=(REAL)(xy2.x*gscaletodevice);
	iy2=(REAL)(xy2.y*gscaletodevice);

	PenIn.GetColour(Colour);
	MSWPen.SetColor(GetMSWColour(Colour));
	PenIn.GetWidth(Width);
	MSWPen.SetWidth((REAL)(Width*gscaletodevice));
	MSWPen.SetStartCap(LineCapRound);
	MSWPen.SetEndCap(LineCapRound);

	graphics->DrawLine(&MSWPen,ix1,iy1,ix2,iy2);
}

void MSWTools::DrawCircle(DSNPen& PenIn,vertex& a,double radius)
{
	vertex	xy1;
	REAL ix1,iy1,r2;
	DSNCOLOUR Colour;
	Brush* brush;

	xy1.x=(a.x-gxofffromfile)/gscalefromfile;
	if(GetFileOrientation()==FILE_IS_X_GOES_DOWN)
	{
		xy1.y=(a.y-gyofffromfile)/gscalefromfile;

	}
	else
	{
		xy1.y=(gfileheight-(a.y-gyofffromfile))/gscalefromfile;
	}

	double r1=radius/gscalefromfile;


	ix1=(REAL)(xy1.x*gscaletodevice);
	iy1=(REAL)(xy1.y*gscaletodevice);
	r2=(REAL)(r1*gscaletodevice);

	PenIn.GetColour(Colour);

	brush=new SolidBrush(GetMSWColour(Colour));

	graphics->FillEllipse(brush,ix1-r2/2,iy1-r2/2,r2,r2);
}


void MSWTools::DrawRectangle(DSNPen& PenIn,vertex& a,vertex& b)
{
	vertex	xy1;
	REAL ix1,iy1;
	DSNCOLOUR Colour;
	Brush* brush;
	REAL h=(REAL)-(gscaletodevice*(b.y-a.y)/gscalefromfile);


	xy1.x=(a.x-gxofffromfile)/gscalefromfile;

	if(GetFileOrientation()==FILE_IS_X_GOES_DOWN)
	{
		xy1.y=(a.y-gyofffromfile)/gscalefromfile;

	}
	else
	{
		xy1.y=(gfileheight-(a.y-gyofffromfile))/gscalefromfile;
	}

	ix1=(REAL)(xy1.x*gscaletodevice);
	iy1=(REAL)(xy1.y*gscaletodevice);

	REAL w=(REAL)(gscaletodevice*(b.x-a.x)/gscalefromfile);
	if(GetFileOrientation()==FILE_IS_X_GOES_DOWN)
	{
		h=(REAL)(gscaletodevice*(b.y-a.y)/gscalefromfile);
	}
	else
	{
		h=(REAL)-(gscaletodevice*(b.y-a.y)/gscalefromfile);
	}

	if(w<0)
	{
		ix1+=w;
		w=-w;
	}

	if(h<0)
	{
		iy1+=h;
		h=-h;
	}

	PenIn.GetColour(Colour);

	brush=new SolidBrush(GetMSWColour(Colour));

	graphics->FillRectangle(brush,ix1,iy1,w,h);
}


void MSWTools::DrawString(DSNPen& PenIn,vertex& xy,vertex& wh,std::wstring& b)
{
	vertex	xy1;
	double ix1,iy1;
	DSNCOLOUR Colour;
	Brush* brush;

	xy1.x=(xy.x-gxofffromfile)/gscalefromfile;
	
	if(GetFileOrientation()==FILE_IS_X_GOES_DOWN)
	{
		xy1.y=(xy.y-gyofffromfile)/gscalefromfile;

	}
	else
	{
		xy1.y=(gfileheight-(xy.y-gyofffromfile))/gscalefromfile;
	}


	ix1=(xy1.x*gscaletodevice);
	iy1=(xy1.y*gscaletodevice);

	PenIn.GetColour(Colour);

	brush=new SolidBrush(GetMSWColour(Colour));

	Font Fon(FontFamily::GenericMonospace(),8);
	StringFormat StrF;

	StrF.SetAlignment(StringAlignmentCenter);
	StrF.SetLineAlignment(StringAlignmentCenter);


	RectF A;

	A.Width=(REAL)wh.x;
	A.Height=(REAL)wh.y;

	A.X=(REAL)(ix1-wh.x/2);
	A.Y=(REAL)(iy1-wh.y/2);

//	graphics->RotateTransform(180);
	graphics->DrawString(b.c_str(),-1,&Fon,A,&StrF,brush);
//	graphics->RotateTransform(-180);
}