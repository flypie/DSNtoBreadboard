#pragma once

#include "framework.h"
#include "commdlg.h"

#include "shobjidl_core.h"

#include <gdiplus.h>
using namespace Gdiplus;

#include <string>

#include "Entity.h"
#include "DSNTools.h"
#include "DSNPen.h"
#include "Vertex.h"


extern Color GetMSWColour(DSNCOLOUR In);



class MSWTools: virtual public DSNTools
{
public:

	MSWTools();

	void DrawLine(Entity &Ent,DSNPen& pen,vertex& a,vertex& b);
	void DrawCircle(Entity& Ent,DSNPen& pen,vertex& a,double radius);
	void DrawRectangle(Entity& Ent,DSNPen& pen,vertex& a,vertex& b);
	void DrawString(Entity& Ent,DSNPen& PenIn,vertex& xy,vertex& wh,std::wstring&b);

	Graphics* graphics;

//	double gscaletodevice;
};

