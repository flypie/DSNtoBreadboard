
#include <string>

#include "DSNTools.h"

using namespace std;




std::wstring trim(std::wstring &str)
{
	size_t start=0,end=str.size();
	wstring trimmed;

	for(wchar_t c:str)
	{
		if(!isspace(c))
			break;
		start++;
	}

	for(; end>start;)
	{
		if(!isspace(str[end-1]))
			break;
		end--;
	}

	trimmed=str.substr(start,end);
	return trimmed;
}


std::wstring qtrim(std::wstring& str)
{
	size_t start=0,end=str.size();
	wstring trimmed;

	for(wchar_t c:str)
	{
		if(c!=L'"')
			break;
		start++;
	}

	for(; end>start;)
	{
		if((str[end-1])!=L'\"')
			break;
		end--;
	}

	trimmed=str.substr(start,end);
	return trimmed;
}




