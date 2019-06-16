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

}

void MSWTools::DrawLine(Entity& Ent,DSNPen& PenIn,vertex& a,vertex& b)
{
	vertex	xy1,xy2;
	DSNCOLOUR Colour;
	Pen	MSWPen(Color(255,0,0,255));
	double Width;

	xy1.x=Ent.ToMM(a.x-Ent.Getxofffromfile());
	if(GetFileOrientation()==FILE_IS_X_GOES_DOWN)
	{
		xy1.y=Ent.ToMM(a.y-Ent.Getyofffromfile());

	}
	else
	{
		xy1.y=Ent.ToMM(Ent.Getfileheight()-(a.y-Ent.Getyofffromfile()));
	}
	
	xy2.x=Ent.ToMM(b.x-Ent.Getxofffromfile());
	if(GetFileOrientation()==FILE_IS_X_GOES_DOWN)
	{
		xy2.y=Ent.ToMM(b.y-Ent.Getyofffromfile());

	}
	else
	{
		xy2.y=Ent.ToMM(Ent.Getfileheight()-(b.y-Ent.Getyofffromfile()));
	}

	REAL ix1,iy1,ix2,iy2;

	ix1=(REAL)(xy1.x);
	iy1=(REAL)(xy1.y);
	ix2=(REAL)(xy2.x);
	iy2=(REAL)(xy2.y);

	PenIn.GetColour(Colour);
	MSWPen.SetColor(GetMSWColour(Colour));
	PenIn.GetWidth(Width);
	MSWPen.SetWidth((REAL)Width);
	MSWPen.SetStartCap(LineCapRound);
	MSWPen.SetEndCap(LineCapRound);

	graphics->DrawLine(&MSWPen,ix1,iy1,ix2,iy2);
}

void MSWTools::DrawCircle(Entity& Ent,DSNPen& PenIn,vertex& a,double radius)
{
	vertex	xy1;
	REAL ix1,iy1,r2;
	DSNCOLOUR Colour;
	Brush* brush;

	xy1.x=Ent.ToMM(a.x-Ent.Getxofffromfile());
	if(GetFileOrientation()==FILE_IS_X_GOES_DOWN)
	{
		xy1.y=Ent.ToMM(a.y-Ent.Getyofffromfile());

	}
	else
	{
		xy1.y=Ent.ToMM(Ent.Getfileheight()-(a.y-Ent.Getyofffromfile()));
	}

	double r1=Ent.ToMM(radius);


	ix1=(REAL)(xy1.x);
	iy1=(REAL)(xy1.y);
	r2=(REAL)(r1);

	PenIn.GetColour(Colour);

	brush=new SolidBrush(GetMSWColour(Colour));

	graphics->FillEllipse(brush,ix1-r2/2,iy1-r2/2,r2,r2);
}


void MSWTools::DrawRectangle(Entity& Ent,DSNPen& PenIn,vertex& a,vertex& b)
{
	vertex	xy1;
	REAL ix1,iy1;
	DSNCOLOUR Colour;
	Brush* brush;
	REAL h=(REAL)-Ent.ToMM(b.y-a.y);


	xy1.x=Ent.ToMM(a.x-Ent.Getxofffromfile());

	if(GetFileOrientation()==FILE_IS_X_GOES_DOWN)
	{
		xy1.y=Ent.ToMM(a.y-Ent.Getyofffromfile());

	}
	else
	{
		xy1.y=Ent.ToMM(Ent.Getfileheight()-(a.y-Ent.Getyofffromfile()));
	}

	ix1=(REAL)(xy1.x);
	iy1=(REAL)(xy1.y);

	REAL w=(REAL)Ent.ToMM((b.x-a.x));
	if(GetFileOrientation()==FILE_IS_X_GOES_DOWN)
	{
		h=(REAL)Ent.ToMM(b.y-a.y);
	}
	else
	{
		h=(REAL)-Ent.ToMM(b.y-a.y);
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


void MSWTools::DrawString(Entity& Ent,DSNPen& PenIn,vertex& xy,vertex& wh,std::wstring& b)
{
	vertex	xy1;
	double ix1,iy1;
	DSNCOLOUR Colour;
	Brush* brush;

	xy1.x=Ent.ToMM(xy.x-Ent.Getxofffromfile());
	
	if(GetFileOrientation()==FILE_IS_X_GOES_DOWN)
	{
		xy1.y=Ent.ToMM(xy.y-Ent.Getyofffromfile());

	}
	else
	{
		xy1.y=Ent.ToMM(Ent.Getfileheight()-(xy.y-Ent.Getyofffromfile()));
	}


	ix1=(xy1.x);
	iy1=(xy1.y);

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