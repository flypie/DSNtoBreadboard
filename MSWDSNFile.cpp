#include "MSWDSNFile.h"

using namespace Gdiplus;

MSWDSNFile::MSWDSNFile(wstring a) : DSNFile(a)
{
	gscalefromfile=0;
	gfileheight=0;
	gfilewidth=0;
	gxofffromfile=0;
	gyofffromfile=0;
	graphics=0;
};																		 

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


void MSWDSNFile::DrawLine(DSNPen& PenIn,vertex &a,vertex &b)
{
	vertex	xy1,xy2;
	DSNCOLOUR Colour;
	Pen	MSWPen(Color(255,0,0,255));
	double Width;

	xy1.x=(a.x-gxofffromfile)/gscalefromfile;
	xy1.y=(gfileheight-(a.y-gyofffromfile))/gscalefromfile;
	xy2.x=(b.x-gxofffromfile)/gscalefromfile;
	xy2.y=(gfileheight-(b.y-gyofffromfile))/gscalefromfile;

	int ix1,iy1,ix2,iy2;

	ix1=(int)(xy1.x*gscaletodevice);
	iy1=(int)(xy1.y*gscaletodevice);
	ix2=(int)(xy2.x*gscaletodevice);
	iy2=(int)(xy2.y*gscaletodevice);
	
	PenIn.GetColour(Colour);
	MSWPen.SetColor(GetMSWColour(Colour));
	PenIn.GetWidth(Width);
	MSWPen.SetWidth((REAL)(Width*gscaletodevice));
	MSWPen.SetStartCap(LineCapRound);							
	MSWPen.SetEndCap(LineCapRound);

	graphics->DrawLine(&MSWPen,ix1,iy1,ix2,iy2);
}

void MSWDSNFile::DrawCircle(DSNPen& PenIn,vertex& a,double radius)
{
	vertex	xy1;
	int ix1,iy1,r2;
	DSNCOLOUR Colour;
	Brush* brush;

	xy1.x=(a.x-gxofffromfile)/gscalefromfile;
	xy1.y=(gfileheight-(a.y-gyofffromfile))/gscalefromfile;
	double r1=radius/gscalefromfile;


	ix1=(int)(xy1.x*gscaletodevice);
	iy1=(int)(xy1.y*gscaletodevice);
	r2=(int)(r1*gscaletodevice);

	PenIn.GetColour(Colour);

	brush=new SolidBrush(GetMSWColour(Colour));

	graphics->FillEllipse(brush,ix1-r2/2,iy1-r2/2,r2,r2);
}


void MSWDSNFile::DrawRectangle(DSNPen& PenIn,vertex& a,vertex& b)
{
	vertex	xy1;
	int ix1,iy1;
	DSNCOLOUR Colour;
	Brush* brush;

	xy1.x=(a.x-gxofffromfile)/gscalefromfile;
	xy1.y=(gfileheight-(a.y-gyofffromfile))/gscalefromfile;

	ix1=(int)(xy1.x*gscaletodevice);
	iy1=(int)(xy1.y*gscaletodevice);

	int w=(int)(gscaletodevice*(b.x-a.x)/gscalefromfile);
	int h=(int)-(gscaletodevice*(b.y-a.y)/gscalefromfile);

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



void MSWDSNFile::Paint(HDC hdc)
{
	RECT    rcCli;
	int	devheight,devwidth;

	graphics = new Graphics(hdc);

	if(GetClientRect(WindowFromDC(hdc),&rcCli))
	{
		devwidth=rcCli.right-rcCli.left-1;
		devheight=rcCli.bottom-rcCli.top-1;

		if(devwidth<devheight)
		{
			gscaletodevice=devwidth;
		}
		else
		{
			gscaletodevice=devheight;
		}
	}
	else
	{
		devwidth=GetDeviceCaps(hdc,HORZRES);
		devheight=GetDeviceCaps(hdc,VERTRES);

		if(devwidth<devheight)
		{
			gscaletodevice=devwidth;
		}
		else
		{
			gscaletodevice=devheight;
		}
		Status Res=graphics->SetPageUnit(UnitPixel);
	}
	DSNFile::Paint();

	delete graphics;
}

