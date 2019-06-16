#include "MSWBreadBoards.h"


// UnitMillimeter


void MSWBreadBoards::Paint(HDC hdc,double Start,double Length)
{
	RECT    rcCli;
	int	devheight,devwidth;

	Tools->graphics=new Graphics(hdc);

	if(GetClientRect(WindowFromDC(hdc),&rcCli))
	{
		devwidth=rcCli.right-rcCli.left-1;
		devheight=rcCli.bottom-rcCli.top-1;

		Status Res=Tools->graphics->SetPageUnit(UnitMillimeter);
	}
	else
	{
		devwidth=GetDeviceCaps(hdc,HORZRES);
		devheight=GetDeviceCaps(hdc,VERTRES);

		Status Res=Tools->graphics->SetPageUnit(UnitMillimeter);
	}

	BreadBoards::Paint(Start,Length);

	delete Tools->graphics;
}