#pragma once

#include "Breadboard.h"

#include "MSWTools.h"

class MSWBreadBoard :  public BreadBoard

{
public:
	MSWBreadBoard(MSWTools* ToolsIn) :BreadBoard (ToolsIn)
	{
		Tools=ToolsIn;
	};

	void DrawLine(DSNPen& a,vertex& b,vertex& c)
	{
		DrawLine(a,b,c);
	};

private:
		MSWTools* ToolsIn;

};

