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


#define wstringicmp(a,b) _wcsicmp(a.c_str(),b.c_str())

typedef struct Element_t
{
	wstring Name;
	wstring Body;
	int	Depth;

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

	wifstream File;
	wofstream OFile;
	int Depth;

	Element Root;

	wstring OpenFileName;

	Element *Get(wstring a,wstring b,wstring c);

private:


};

