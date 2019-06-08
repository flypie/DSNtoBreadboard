#pragma once

#include "DSNPen.h"
#include "Vertex.h"
#include <string>

typedef enum
{
	FILE_IS_X_GOES_UP,
	FILE_IS_X_GOES_DOWN
} FILEXDIRECTION;


class DSNTools
{


public:
	void SetFileOrientation(FILEXDIRECTION a)
	{
		FileDir=a;
	};
	
	FILEXDIRECTION GetFileOrientation()
	{
		return FileDir;
	};

	virtual void DrawLine(DSNPen& pen,vertex& a,vertex& b)=0;
	virtual void DrawCircle(DSNPen& pen,vertex& a,double radius)=0;
	virtual	void DrawRectangle(DSNPen& pen,vertex& a,vertex& b)=0;
	virtual void DrawString(DSNPen& PenIn,vertex& xy,vertex& wh,std::wstring& b)=0;

	double	gscalefromfile,gxofffromfile,gyofffromfile;
	double	gfilewidth,gfileheight;

private:
	FILEXDIRECTION FileDir;

};



extern std::wstring trim(std::wstring &str);
extern std::wstring qtrim(std::wstring& str);