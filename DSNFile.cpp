#include "DSNFile.h"

DSNFile::DSNFile(std::wstring& FileName)
{
	Depth=0;

	OpenFileName=FileName;

	File.open(OpenFileName);
	
	if(File.is_open())
	{
		Parse();
	}
	else
	{
		throw DSNExceptions::FILEOPENFAILED;
	}
	
	File.close();


}


DSNFile::~DSNFile()
{
	DeleteElement(Root);
}



DSNReturn DSNFile::ReadElement(Element &Current)
{
	wchar_t c=0;
	int SpaceCount=0;
	bool NotInQuotes=true;


	File>>Current.Name;

	Current.Depth=++Depth;
			
	while(((c=File.get())!=')' || !NotInQuotes)&& c!=WEOF)
	{
		if(c=='(' && NotInQuotes)
		{
			Current.SubElements.resize(Current.SubElements.size()+1);
			ReadElement(Current.SubElements.back());
			SpaceCount=0;
		}
		else if(c<' ')
		{

		}
		else if(c==' ')
		{
			if(SpaceCount==0 && Current.Body.length()>0)
			{
				Current.Body+=c;
		    }
			SpaceCount++;
		}
		else 
		{
			if(c=='\"' && Current.Name!=L"string_quote")
			{
				NotInQuotes=!NotInQuotes;
			}
			Current.Body+=c;
			SpaceCount=0;
		};
	}
	Depth--;


	return SUCCESS;
}



DSNReturn DSNFile::WriteElement(Element	& Current)
{
	wchar_t c=0;
	int SpaceCount=0;
	bool NotInQuotes=true;

	for(int i=0; i<Current.Depth-1; i++)
	{
		OFile<<"\t";
	}

	OFile<<L"("+Current.Name+L" "+Current.Body;

	if(Current.SubElements.size()>0)
	{
		OFile<<"\n";
	}

	for(Element n:Current.SubElements)
	{
		WriteElement(n);
	}

	if(Current.SubElements.size()==0)
	{
		OFile<<")\n";
	}
	else
	{
		for(int i=0; i<Current.Depth-1; i++)
		{
			OFile<<"\t";
		}
		OFile<<")\n";

	}

	return SUCCESS;
}



DSNReturn DSNFile::DeleteElement(Element& Current)
{
	wchar_t c=0;
	int SpaceCount=0;
	bool NotInQuotes=true;

	for(Element n:Current.SubElements)
	{
		DeleteElement(n);
	}

	Current.SubElements.clear();

	return SUCCESS;
}




DSNReturn DSNFile::Parse()
{
	Depth=0;
	wchar_t c;

	while((c=File.get())!='('); //Read start (

	ReadElement(Root);

	return SUCCESS;
}


DSNReturn DSNFile::FileOut(wstring Name)
{
	if(Name==L"")
	{
		OFile.open(Name);
	}
	else
	{
		OFile.open(OpenFileName);
	}
	WriteElement(Root);

	OFile.close();

	return SUCCESS;
}





