#pragma once

#include "framework.h"
#include "commdlg.h"

#include "shobjidl_core.h"

#include <gdiplus.h>
using namespace Gdiplus;

#include "DSNFile.h"


class MSWDSNFile :
	public DSNFile
{
public:
	MSWDSNFile(wstring a);


	void Paint(HDC hdc);
	void Print(HDC hdc);

protected:

private:

	void DrawLine(DSNPen&pen,vertex & a,vertex & b);
	void DrawCircle(DSNPen& pen,vertex& a,double radius);
	void DrawRectangle(DSNPen& pen,vertex& a,vertex& b);



	Graphics *graphics;

};

