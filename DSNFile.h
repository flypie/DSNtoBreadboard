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


typedef struct Element_t
{
	wstring Name;
	wstring Body;
	int	Depth;

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

private:
	DSNReturn Parse();
	DSNReturn ReadElement(Element &Current);
	DSNReturn WriteElement(Element& Current);
	DSNReturn DeleteElement(Element& Current);

	wifstream File;
	wofstream OFile;
	int Depth;

	Element Root;

	wstring OpenFileName;
};

