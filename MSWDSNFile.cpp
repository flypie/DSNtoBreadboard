
#include "MSWDSNFile.h"


using namespace Gdiplus;

MSWDSNFile::MSWDSNFile(wstring a,MSWTools& b) : DSNFile(a,b)
{

	Tools=&b;
};																		 


extern void AddToLog(const std::wstring FileName);


void MSWDSNFile::Paint(HDC hdc)
{
	RECT    rcCli;
	int	devheight,devwidth;

	Tools->SetFileOrientation(FILE_IS_X_GOES_UP);

	Tools->graphics = new Graphics(hdc);

	if(GetClientRect(WindowFromDC(hdc),&rcCli))
	{
		devwidth=rcCli.right-rcCli.left-1;
		devheight=rcCli.bottom-rcCli.top-1;

		if(devwidth<devheight)
		{
			Tools->gscaletodevice=devwidth;
		}
		else
		{
			Tools->gscaletodevice=devheight;
		}
		Status Res=Tools->graphics->SetPageUnit(UnitPixel);
	}
	else
	{
		devwidth=GetDeviceCaps(hdc,HORZRES);
		devheight=GetDeviceCaps(hdc,VERTRES);

		if(devwidth<devheight)
		{
			Tools->gscaletodevice=devwidth;
		}
		else
		{
			Tools->gscaletodevice=devheight;
		}
		Status Res=Tools->graphics->SetPageUnit(UnitPixel);
	}
	DSNFile::Paint();


	delete Tools->graphics;
}		 


