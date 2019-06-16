#pragma once

#include "Entity.h"
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

	virtual void DrawLine(Entity& Ent,DSNPen& pen,vertex& a,vertex& b)=0;
	virtual void DrawCircle(Entity& Ent,DSNPen& pen,vertex& a,double radius)=0;
	virtual	void DrawRectangle(Entity& Ent,DSNPen& pen,vertex& a,vertex& b)=0;
	virtual void DrawString(Entity& Ent,DSNPen& PenIn,vertex& xy,vertex& wh,std::wstring& b)=0;



private:
	FILEXDIRECTION FileDir;

};



extern std::wstring trim(std::wstring &str);
extern std::wstring qtrim(std::wstring& str);