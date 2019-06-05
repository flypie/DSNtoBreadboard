#pragma once

#include "DSNPen.h"
#include "Vertex.h"
#include <string>


class DSNTools
{

public:
	virtual void DrawLine(DSNPen& pen,vertex& a,vertex& b)=0;
	virtual void DrawCircle(DSNPen& pen,vertex& a,double radius)=0;
	virtual	void DrawRectangle(DSNPen& pen,vertex& a,vertex& b)=0;
	virtual void DrawString(DSNPen& PenIn,vertex& xy,vertex& wh,std::wstring& b)=0;

	double	gscalefromfile,gxofffromfile,gyofffromfile;
	double	gfilewidth,gfileheight;

};

