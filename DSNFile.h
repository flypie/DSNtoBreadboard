#pragma once

#include <iostream>
#include <fstream>
using namespace std;

#include <vector>

#include <string>
#include <vector>

#include "Entity.h"
#include "Vertex.h"
#include "DSNPen.h"
#include "DSNTools.h"


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



typedef struct
{
	wstring Body;
	wstring Device;
	int	Number;
	wstring pid; //When Number=-1 yje ise his string
} Pin;

class DSNFile : public Entity
{
public:
	DSNFile();
	~DSNFile();

	DSNFile(std::wstring &FileName, DSNTools &ToolsIn);

	DSNReturn FileOut(wstring &File);




protected:
	DSNReturn Parse();
	DSNReturn ReadElement(Element &Current);
	DSNReturn WriteElement(Element& Current);
	DSNReturn DeleteElement(Element& Current);

	void DrawComponentImage(const Element& Image,vertex xy,double angle);
	void DrawComponentOfType(const Element& Part,const Element& Image);
	void DrawPCBOutline(const Element& path);
	void DrawNets(std::vector<Pin> PinList);

	void Paint();
	
	wifstream File;
	wofstream OFile;
	int Depth;

	Element Root;

	wstring OpenFileName;

	Element *Get(wstring a,wstring b,wstring c);

	friend class BreadBoards;
private:
	DSNTools *Tools;

};


