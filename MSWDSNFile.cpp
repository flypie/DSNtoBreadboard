
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

		Status Res=Tools->graphics->SetPageUnit(UnitMillimeter);
	}
	else
	{
		devwidth=GetDeviceCaps(hdc,HORZRES);
		devheight=GetDeviceCaps(hdc,VERTRES);

		Status Res=Tools->graphics->SetPageUnit(UnitMillimeter);
	}
	DSNFile::Paint();


	delete Tools->graphics;
}		 


