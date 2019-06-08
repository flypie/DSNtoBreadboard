#include "MSWBreadBoards.h"


// UnitMillimeter


void MSWBreadBoards::Paint(HDC hdc)
{
	RECT    rcCli;
	int	devheight,devwidth;

	Tools->graphics=new Graphics(hdc);

	if(GetClientRect(WindowFromDC(hdc),&rcCli))
	{
		devwidth=rcCli.right-rcCli.left-1;
		devheight=rcCli.bottom-rcCli.top-1;

		if(devwidth<devheight)
		{
			Tools->gscaletodevice=1;
		}
		else
		{
			Tools->gscaletodevice=1;
		}

		Status Res=Tools->graphics->SetPageUnit(UnitMillimeter);
	}
	else
	{
		devwidth=GetDeviceCaps(hdc,HORZRES);
		devheight=GetDeviceCaps(hdc,VERTRES);

		if(devwidth<devheight)
		{
			Tools->gscaletodevice=1;
		}
		else
		{
			Tools->gscaletodevice=1;
		}
		Status Res=Tools->graphics->SetPageUnit(UnitMillimeter);
	}

	Tools->gfilewidth=100;
	Tools->gxofffromfile=0;
	Tools->gfileheight=100;
	Tools->gyofffromfile=0;

	Tools->gscalefromfile=1;

	BreadBoards::Paint();

	delete Tools->graphics;
}