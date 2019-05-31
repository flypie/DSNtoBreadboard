#include "DSNFile.h"


int		DListLen;
std::wstring* DList[1000];

void AddToLog(const std::wstring FileName)
{
	if(!DList[DListLen])
	{
		DList[DListLen]=new std::wstring;
	}

	*DList[DListLen++]+=FileName;
}

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


wstring trim(wstring str)
{
	size_t start=0,end=str.size();
	wstring trimmed;
	
	for(wchar_t c:str)
	{
		if(!isspace(c))
			break;
		start++;
	}
	
	for(;end>start;)
	{
		if(!isspace(str[end-1]))
			break;
		end--;
	}

	trimmed=str.substr(start,end);
	return trimmed;
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
			if(c=='\"' && wstringicmp(Current.Name,wstring(L"string_quote")))
			{
				NotInQuotes=!NotInQuotes;
			}
			Current.Body+=c;
			SpaceCount=0;
		};
	}
	Current.Body=trim(Current.Body);
	Current.Name=trim(Current.Name);
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


#define PI 3.14159265


void DSNFile::DrawComponentImage(const Element& Image,vertex inxy,double angle)
{
	size_t curfield,nextdelimter;

	double	linewidth;
	vertex	xy;
	std::vector<vertex> vertices;
	double AngleComp;

	for(auto const& ImageEl:Image.SubElements)
	{

		if(wstringicmp(ImageEl.Name,wstring(L"outline"))==0)
		{
			DSNPen	pen(BLUE);

			for(auto const& OutlineEl:ImageEl.SubElements)
			{
				if(wstringicmp(OutlineEl.Name,wstring(L"path"))==0)
				{
					curfield=0;
					vertices.clear();

					nextdelimter=OutlineEl.Body.find_first_of(' ',curfield);
					wstring layer_id=OutlineEl.Body.substr(0,nextdelimter);
					curfield=OutlineEl.Body.find_first_not_of(' ',nextdelimter);

					nextdelimter=OutlineEl.Body.find_first_of(' ',curfield);
					linewidth=stod(OutlineEl.Body.substr(curfield,nextdelimter-curfield));
					curfield=OutlineEl.Body.find_first_not_of(' ',nextdelimter);

					while(nextdelimter!=wstring::npos)
					{
						nextdelimter=OutlineEl.Body.find_first_of(' ',curfield);
						xy.x=stod(OutlineEl.Body.substr(curfield,nextdelimter-curfield));
						curfield=OutlineEl.Body.find_first_not_of(' ',nextdelimter);

						nextdelimter=OutlineEl.Body.find_first_of(' ',curfield);
						xy.y=stod(OutlineEl.Body.substr(curfield,nextdelimter-curfield));
						curfield=OutlineEl.Body.find_first_not_of(' ',nextdelimter);

						xy.Rotate(angle);

						vertices.push_back(xy+inxy);
					}

					pen.SetWidth(linewidth/gscalefromfile);

					for(unsigned int i=0; i<vertices.size()-1; i++)
					{
						DrawLine(pen,vertices[i],vertices[i+1]);
					}
				}
			}
		}
		else if(wstringicmp(ImageEl.Name,wstring(L"pin"))==0)
		{
			vertex	pinoffset;

			DSNPen	pen(GREEN);


			curfield=0;

			nextdelimter=ImageEl.Body.find_first_of(' ',curfield);
			wstring name=ImageEl.Body.substr(0,nextdelimter-curfield);
			curfield=ImageEl.Body.find_first_not_of(' ',nextdelimter);

			nextdelimter=ImageEl.Body.find_first_of(' ',curfield);
			wstring dimension_unit=ImageEl.Body.substr(curfield,nextdelimter-curfield);
			curfield=ImageEl.Body.find_first_not_of(' ',nextdelimter);

			nextdelimter=ImageEl.Body.find_first_of(' ',curfield);
			pinoffset.x=stod(ImageEl.Body.substr(curfield,nextdelimter-curfield));
			curfield=ImageEl.Body.find_first_not_of(' ',nextdelimter);

			nextdelimter=ImageEl.Body.find_first_of(' ',curfield);
			pinoffset.y=stod(ImageEl.Body.substr(curfield,nextdelimter-curfield));
			curfield=ImageEl.Body.find_first_not_of(' ',nextdelimter);

			if(ImageEl.SubElements.size())
			{
				AngleComp=stod(ImageEl.SubElements[0].Body);
			}
			else
			{
				AngleComp=0;
			}

			Element* library,* padstack;

			library=Root.FindSub(L"library");
			if(library)
			{
				padstack=library->FindSub(L"padstack",name);
				if(padstack)
				{
					for(auto const& Shape:padstack->SubElements)
					{
						if(wstringicmp(Shape.Name,wstring(L"shape"))==0)
						{
							for(auto const& Shape:Shape.SubElements)
							{
								curfield=0;
								if(wstringicmp(Shape.Name,wstring(L"circle"))==0)
								{
									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									wstring side=Shape.Body.substr(curfield,nextdelimter-curfield);
									curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									double radius=stod(Shape.Body.substr(curfield,nextdelimter-curfield));
									curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

									vertex temp;

									vertex temp1=pinoffset.RotateA(angle);

									temp.x=inxy.x+temp1.x;
									temp.y=inxy.y+temp1.y;

									DrawCircle(pen,temp,radius);
								}
								else if(wstringicmp(Shape.Name,wstring(L"rect"))==0)
								{
									vertex xy1,xy2;

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									wstring side=Shape.Body.substr(curfield,nextdelimter-curfield);
									curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									xy1.x=stod(Shape.Body.substr(curfield,nextdelimter-curfield));
									curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									xy1.y=stod(Shape.Body.substr(curfield,nextdelimter-curfield));
									curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									xy2.x=stod(Shape.Body.substr(curfield,nextdelimter-curfield));
									curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									xy2.y=stod(Shape.Body.substr(curfield,nextdelimter-curfield));
									curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

									vertex temp1=pinoffset.RotateA(angle);
									vertex temp2=xy1.RotateA(angle+AngleComp);
									vertex temp3=xy2.RotateA(angle+AngleComp);

									vertex tempa=inxy+temp1+temp2;
									vertex tempb=inxy+temp1+temp3;

									DrawRectangle(pen,tempa,tempb);
								}
								else if(wstringicmp(Shape.Name,wstring(L"path"))==0)
								{
									size_t curfield,nextdelimter,linewidth;
									std::vector<vertex> vertices;

									vertex	xy;

									curfield=0;

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									wstring layer_id=Shape.Body.substr(0,nextdelimter);
									curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									linewidth=stoi(Shape.Body.substr(curfield,nextdelimter-curfield));
									curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

									pen.SetWidth(linewidth/gscalefromfile);

									while(nextdelimter!=wstring::npos)
									{
										nextdelimter=Shape.Body.find_first_of(' ',curfield);
										xy.x=stod(Shape.Body.substr(curfield,nextdelimter-curfield));
										curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

										nextdelimter=Shape.Body.find_first_of(' ',curfield);
										xy.y=stod(Shape.Body.substr(curfield,nextdelimter-curfield));
										curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

										vertex temp1=pinoffset.RotateA(angle);
										vertex temp2=xy.RotateA(angle+AngleComp);

										vertex tempa=inxy+temp1+temp2;

										vertices.push_back(tempa);
									}

									for(unsigned int i=0; i<vertices.size()-1; i++)
									{
										DrawLine(pen,vertices[i],vertices[i+1]);
									}
								}
								else
								{

								}
							}
						}
						else
						{

						}
					}
				}
			}
		}
		else if(wstringicmp(ImageEl.Name,wstring(L"keepout"))==0)
		{
			DSNPen	Pen(RED);

			curfield=0;

			for(auto const& Shape:ImageEl.SubElements)
			{
				vertex offset;
				if(wstringicmp(Shape.Name,wstring(L"circle"))==0)
				{
					nextdelimter=Shape.Body.find_first_of(' ',curfield);
					wstring side=Shape.Body.substr(curfield,nextdelimter-curfield);
					curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

					nextdelimter=Shape.Body.find_first_of(' ',curfield);
					double radius=stod(Shape.Body.substr(curfield,nextdelimter-curfield));
					curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

					if(nextdelimter!=wstring::npos)
					{
						nextdelimter=Shape.Body.find_first_of(' ',curfield);
						offset.x=stod(Shape.Body.substr(curfield,nextdelimter-curfield));
						curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

						nextdelimter=Shape.Body.find_first_of(' ',curfield);
						offset.y=stod(Shape.Body.substr(curfield,nextdelimter-curfield));
						curfield=Shape.Body.find_first_not_of(' ',nextdelimter);

						offset.Rotate(angle);
					}
					else
					{
						offset.x=0;
						offset.y=0;
					}

					vertex temp;

					temp.x=inxy.x+offset.x;
					temp.y=inxy.y+offset.y;

					DrawCircle(Pen,temp,radius);
				}
				else
				{
					AddToLog(L"TO BE DONE:"+Shape.Name+L" Keepout not handled.");
				}
			}
		}
		else
		{
			AddToLog(L"error "+ImageEl.Name);
		}
	}
}

void DSNFile::DrawComponentOfType(const Element& placement,const Element& Image)
{
	vertex pxy;

	for(auto const& place:placement.SubElements)
	{
		size_t curfield=0,nextdelimter;

		nextdelimter=place.Body.find_first_of(' ',curfield);
		wstring cname=place.Body.substr(0,nextdelimter);
		curfield=place.Body.find_first_not_of(' ',nextdelimter);

		nextdelimter=place.Body.find_first_of(' ',curfield);
		pxy.x=stod(place.Body.substr(curfield,nextdelimter-curfield));
		curfield=place.Body.find_first_not_of(' ',nextdelimter);

		nextdelimter=place.Body.find_first_of(' ',curfield);
		pxy.y=stod(place.Body.substr(curfield,nextdelimter-curfield));
		curfield=place.Body.find_first_not_of(' ',nextdelimter);

		nextdelimter=place.Body.find_first_of(' ',curfield);
		wstring cforb=place.Body.substr(curfield,nextdelimter-curfield);
		curfield=place.Body.find_first_not_of(' ',nextdelimter);

		nextdelimter=place.Body.find_first_of(' ',curfield);
		double angle=stod(place.Body.substr(curfield,nextdelimter-curfield));
		curfield=place.Body.find_first_not_of(' ',nextdelimter);

		DrawComponentImage(Image,pxy,angle);
	}
}

void DSNFile::DrawPCBOutline(const Element& path)
{
	size_t curfield,nextdelimter,aperture_width;
	std::vector<vertex> vertices;


	vertex	xy;
	DSNPen	Pen(CYAN);
	vertex ul,lr;

	ul.x=DBL_MAX;
	ul.y=-DBL_MAX;
	lr.x=-DBL_MAX;
	lr.y=DBL_MAX;

	curfield=0;

	nextdelimter=path.Body.find_first_of(' ',curfield);
	wstring layer_id=path.Body.substr(0,nextdelimter);
	curfield=nextdelimter+1;

	nextdelimter=path.Body.find_first_of(' ',curfield);
	aperture_width=stoi(path.Body.substr(curfield,nextdelimter-curfield));
	curfield=path.Body.find_first_not_of(' ',nextdelimter);

	while(nextdelimter!=wstring::npos)
	{
		nextdelimter=path.Body.find_first_of(' ',curfield);
		xy.x=stod(path.Body.substr(curfield,nextdelimter-curfield));
		curfield=path.Body.find_first_not_of(' ',nextdelimter);

		nextdelimter=path.Body.find_first_of(' ',curfield);
		xy.y=stod(path.Body.substr(curfield,nextdelimter-curfield));
		curfield=path.Body.find_first_not_of(' ',nextdelimter);

		vertices.push_back(xy);

		if(xy.x<ul.x)
		{
			ul.x=xy.x;
		}
		if(xy.y>ul.y)
		{
			ul.y=xy.y;
		}

		if(xy.x>lr.x)
		{
			lr.x=xy.x;
		}
		if(xy.y<lr.y)
		{
			lr.y=xy.y;
		}
	}

	gfilewidth=lr.x-ul.x;
	gxofffromfile=ul.x;
	gfileheight=(ul.y-lr.y);
	gyofffromfile=lr.y;

	if(gfilewidth>gfileheight)
	{
		gscalefromfile=gfilewidth;
	}
	else
	{
		gscalefromfile=gfileheight;
	}

	Pen.SetWidth(1.0/gscalefromfile);

	for(unsigned int i=0; i<vertices.size()-1; i++)
	{
		DrawLine(Pen,vertices[i],vertices[i+1]);
	}
}


void DSNFile::Paint()
{
	Element* structure,* boundary,* path;
	Element* placements;
	Element* library,* image;


	structure=Root.FindSub(L"structure");
	if(structure)
	{
		boundary=structure->FindSub(L"boundary");
		if(boundary)
		{
			path=boundary->FindSub(L"path");
			if(path)
			{
				DrawPCBOutline(*path);
			}
		}
	}
	placements=Root.FindSub(L"placement");
	if(placements)
	{
		for(auto const& placement:placements->SubElements)
		{
			if(wstringicmp(placement.Name,wstring(L"component"))==0)
			{
				library=Root.FindSub(L"library");
				if(library)
				{
					image=library->FindSub(L"image",placement.Body);
					if(image)
					{
						DrawComponentOfType(placement,*image);
					}
				}
			}
		}
	}
	placements=Root.FindSub(L"network");
	if(placements)
	{
		for(auto const& placement:placements->SubElements)
		{
			if(wstringicmp(placement.Name,wstring(L"net"))==0)
			{
			}
		}
	}
}
