#pragma once

#include <iostream>
#include <fstream>
using namespace std;

#include <string>
#include <vector>

typedef enum
{
	SUCCESS,
	FAIL
} DSNReturn;


typedef enum
{
	FILEOPENFAILED,
	FORMATERROR,
	UNKNOWNFIELD,
	FAILFAIL
} DSNExceptions;

typedef enum
{
	BLACK,RED,GREEN,BLUE,YELLOW,CYAN,MAGENTA,WHITE
} DSNCOLOUR;


#define PI 3.14159265



extern int		DListLen;
extern std::wstring* DList[1000];

extern void AddToLog(const std::wstring FileName);


class vertex
{
public:

	void Rotate(double angle)
	{
		double cangle=cos(angle*PI/180.0);
		double sangle=sin(angle*PI/180.0);

		double xin=x,yin=y;
		x=xin*cangle-yin*sangle;
		y=xin*sangle+yin*cangle;
	};

	vertex RotateA(double angle)
	{
		vertex temp=*this;

		temp.Rotate(angle);

		return temp;
	};

	vertex operator+(const vertex R)
	{
		vertex copy;

		copy.x=this->x+R.x;
		copy.y=this->y+R.y;

		return copy;
	};

	vertex& operator+=(const vertex R)
	{
		this->x+=R.x;
		this->y+=R.y;

		return *this;
	};

	double x,y;
};

#define wstringicmp(a,b) _wcsicmp(a.c_str(),b.c_str())

typedef struct Element_t
{
	wstring Name;
	wstring Body;
	int	Depth=0;

	struct Element_t* FindSub(wstring Str)
	{
		Element *Ret=0;

		for(unsigned int i=0; i<SubElements.size(); i++)
		{
			if(wstringicmp(SubElements[i].Name,Str)==0)
			{
				Ret=&SubElements[i];
				break;
			}
		}
		return Ret;
	};


	struct Element_t* FindSub(wstring Str,wstring Body)
	{
		Element* Ret=0;

		for(unsigned int i=0; i<SubElements.size(); i++)
		{
			if(wstringicmp(SubElements[i].Name,Str)==0 && wstringicmp(SubElements[i].Body,Body)==0)
			{
				Ret=&SubElements[i];
				break;
			}
		}
		return Ret;
	};


	std::vector<struct Element_t> SubElements;

}  Element;

class DSNPen
{
public:
	DSNPen(DSNCOLOUR A)
	{
		Colour=A;
		Width=1;
	};

	void SetWidth(double A)
	{
		Width=A;
	};

	void GetWidth(double&A)
	{
		A=Width;
	};

	void GetColour(DSNCOLOUR& A)
	{
		A=Colour;
	};

	void SetColour(DSNCOLOUR& A)
	{
		Colour=A;
	};

private:
		DSNCOLOUR Colour;
		double	Width;


};

class DSNFile
{
public:
	DSNFile();
	~DSNFile();

	DSNFile(std::wstring &FileName);

	DSNReturn FileOut(wstring File);

protected:
	DSNReturn Parse();
	DSNReturn ReadElement(Element &Current);
	DSNReturn WriteElement(Element& Current);
	DSNReturn DeleteElement(Element& Current);

	void DrawComponentImage(const Element& Image,vertex xy,double angle);
	void DrawComponentOfType(const Element& Part,const Element& Image);
	void DrawPCBOutline(const Element& path);

	virtual void DrawLine(DSNPen& pen,vertex& a,vertex& b) = 0;
	virtual void DrawCircle(DSNPen& pen,vertex& a,double radius) =0 ;
	virtual	void DrawRectangle(DSNPen& pen,vertex& a,vertex& b) =0;

	void Paint();
	
	wifstream File;
	wofstream OFile;
	int Depth;

	Element Root;

	wstring OpenFileName;

	Element *Get(wstring a,wstring b,wstring c);

	double gscaletodevice;

	double	gscalefromfile,gxofffromfile,gyofffromfile;
	double	gfilewidth,gfileheight;



private:


};

