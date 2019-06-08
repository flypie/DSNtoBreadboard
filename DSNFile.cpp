
#include "DSNFile.h"

extern void AddToLog(const std::wstring FileName);


DSNFile::DSNFile(std::wstring& FileName,DSNTools& ToolsIn)
{
	Depth=0;

	OpenFileName=FileName;

	File.open(OpenFileName);

	Tools=&ToolsIn;

	if(File.is_open())
	{
		Parse();
	}
	else
	{
		throw DSNExceptions::FILEOPENFAILED;
	}

	Element* structure;

	std::vector<Pin> DPins;

	structure=Root.FindSub(L"unit");

	if(structure)
	{
		if(structure->Body==L"inch")
		{
			convertion=1/25.4;
		}
		else if(structure->Body==L"mil")
		{
			convertion=39.37;
		}
		else if(structure->Body==L"cm")
		{
			convertion=0.1;
		}
		else if(structure->Body==L"mm")
		{
			convertion=1;
		}
		else if(structure->Body==L"um")
		{
			convertion=1000;
		}
		else
		{
			throw;
		}
	}


	File.close();


}


DSNFile::~DSNFile()
{
	DeleteElement(Root);
}



DSNReturn DSNFile::ReadElement(Element& Current)
{
	wchar_t c=0;
	int SpaceCount=0;
	bool NotInQuotes=true;


	File>>Current.Name;

	Current.Depth=++Depth;

	while(((c=File.get())!=')'||!NotInQuotes)&&c!=WEOF)
	{
		if(c=='('&&NotInQuotes)
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
			if(SpaceCount==0&&Current.Body.length()>0)
			{
				Current.Body+=c;
			}
			SpaceCount++;
		}
		else
		{
			if(c=='\"'&&wstringicmp(Current.Name,wstring(L"string_quote")))
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



DSNReturn DSNFile::WriteElement(Element& Current)
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


DSNReturn DSNFile::FileOut(wstring& Name)
{
	DSNReturn Ret=FAIL;

	if(Name!=L"")
	{
		OFile.open(Name);
	}
	else
	{
		OFile.open(OpenFileName);
	}


	if(!OFile.good())
	{
		std::cout<<"After: Not good...\n";
	}

	if(OFile.is_open())
	{
		WriteElement(Root);

		OFile.close();

		Ret=SUCCESS;

	}

	return Ret;
}


#define PI 3.14159265


void DSNFile::DrawComponentImage(const Element&Image,vertex inxy,double angle)
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

					pen.SetWidth(linewidth/Tools->gscalefromfile);

					for(unsigned int i=0; i<vertices.size()-1; i++)
					{
						Tools->DrawLine(pen,vertices[i],vertices[i+1]);
					}
				}
			}
		}
		else if(wstringicmp(ImageEl.Name,wstring(L"pin"))==0)
		{
			vertex	pinoffset;

			curfield=0;

			nextdelimter=ImageEl.Body.find_first_of(' ',curfield);
			wstring name=ImageEl.Body.substr(0,nextdelimter-curfield);
			curfield=ImageEl.Body.find_first_not_of(' ',nextdelimter);

			nextdelimter=ImageEl.Body.find_first_of(' ',curfield);
			wstring dimension_unit=ImageEl.Body.substr(curfield,nextdelimter-curfield);
			dimension_unit=qtrim(dimension_unit);
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
									DSNPen	pen(RED);

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

									Tools->DrawCircle(pen,temp,radius);
								}
								else if(wstringicmp(Shape.Name,wstring(L"rect"))==0)
								{
									DSNPen pen(GREEN);

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

									Tools->DrawRectangle(pen,tempa,tempb);
								}
								else if(wstringicmp(Shape.Name,wstring(L"path"))==0)
								{
									DSNPen	pen(BLUE);

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

									pen.SetWidth(linewidth/Tools->gscalefromfile);

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

									if(vertices.size()==2&&(vertices[0].x==vertices[1].x)&&(vertices[0].y==vertices[1].y))
									{
										pen.SetColour(CYAN);
										Tools->DrawCircle(pen,vertices[0],linewidth);
									}
									else
									{
										pen.SetColour(MAGENTA);
										for(unsigned int i=0; i<vertices.size()-1; i++)
										{
											Tools->DrawLine(pen,vertices[i],vertices[i+1]);
										}
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

					Tools->DrawCircle(Pen,temp,radius);
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
	curfield=path.Body.find_first_not_of(' ',nextdelimter);

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

	Tools->gfilewidth=lr.x-ul.x;
	Tools->gxofffromfile=ul.x;
	Tools->gfileheight=(ul.y-lr.y);
	Tools->gyofffromfile=lr.y;

	if(Tools->gfilewidth>Tools->gfileheight)
	{
		Tools->gscalefromfile=Tools->gfilewidth;
	}
	else
	{
		Tools->gscalefromfile=Tools->gfileheight;
	}

	Pen.SetWidth(1.0/Tools->gscalefromfile);

	for(unsigned int i=0; i<vertices.size()-1; i++)
	{
		Tools->DrawLine(Pen,vertices[i],vertices[i+1]);
	}
}


void DSNFile::DrawNets(std::vector<Pin> PinList)
{
	Element* placements;
	Element* library,* image;
	vector<vertex> thisnet;

	for(Pin PinPad:PinList)
	{
		if(PinPad.Device==L"Z5-6"&&PinList.size()==2)
		{
			printf("ded\n");

		}
		placements=Root.FindSub(L"placement");
		if(placements)
		{
			for(auto const& place:placements->SubElements)
			{
				if(wstringicmp(place.Name,wstring(L"component"))==0)
				{
					for(auto const& Comp:place.SubElements)
					{
						size_t curfield=0,nextdelimter;

						nextdelimter=Comp.Body.find_first_of(' ',curfield);
						wstring cname=Comp.Body.substr(0,nextdelimter);

						if(Comp.Body[curfield]==L'"')
						{
							cname=Comp.Body.substr(1,Comp.Body.find_first_of('"',1)-1);
						}
						else
						{
							int delimter=Comp.Body.find_first_of(' ',0);
							cname=Comp.Body.substr(0,delimter);
						}

						curfield=Comp.Body.find_first_not_of(' ',nextdelimter);

						if(cname==PinPad.Device)
						{
							vertex pxy;

							nextdelimter=Comp.Body.find_first_of(' ',curfield);
							pxy.x=stod(Comp.Body.substr(curfield,nextdelimter-curfield));
							curfield=Comp.Body.find_first_not_of(' ',nextdelimter);

							nextdelimter=Comp.Body.find_first_of(' ',curfield);
							pxy.y=stod(Comp.Body.substr(curfield,nextdelimter-curfield));
							curfield=Comp.Body.find_first_not_of(' ',nextdelimter);

							nextdelimter=Comp.Body.find_first_of(' ',curfield);
							wstring cforb=Comp.Body.substr(curfield,nextdelimter-curfield);
							curfield=Comp.Body.find_first_not_of(' ',nextdelimter);

							nextdelimter=Comp.Body.find_first_of(' ',curfield);
							double angle=stod(Comp.Body.substr(curfield,nextdelimter-curfield));
							curfield=Comp.Body.find_first_not_of(' ',nextdelimter);

							library=Root.FindSub(L"library");
							if(library)
							{
								image=library->FindSub(L"image",place.Body);
								if(image)
								{
									for(auto const& Item:image->SubElements)
									{
										if(Item.Name==L"pin")
										{
											curfield=0;

											nextdelimter=Item.Body.find_first_of(' ',curfield);
											wstring name=Item.Body.substr(0,nextdelimter-curfield);
											curfield=Item.Body.find_first_not_of(' ',nextdelimter);

											nextdelimter=Item.Body.find_first_of(' ',curfield);

											int PinNum;
											wstring PinName;

											if(!isdigit(Item.Body.substr(curfield).c_str()[0]))
											{
												// conversion failed because the input wasn't a number
												PinNum=-2;
												PinName=Item.Body.substr(curfield,nextdelimter-curfield);
												PinName=qtrim(PinName);
											}
											else
											{
												PinNum=stoi(Item.Body.substr(curfield));
												// use converted
											}

											curfield=Item.Body.find_first_not_of(' ',nextdelimter);

											if(PinNum==PinPad.Number||(PinPad.Number==-1&&PinNum==-2&&PinName==PinPad.pid))
											{
												vertex	pinoffset;

												nextdelimter=Item.Body.find_first_of(' ',curfield);
												pinoffset.x=stod(Item.Body.substr(curfield,nextdelimter-curfield));
												curfield=Item.Body.find_first_not_of(' ',nextdelimter);

												nextdelimter=Item.Body.find_first_of(' ',curfield);
												pinoffset.y=stod(Item.Body.substr(curfield,nextdelimter-curfield));
												curfield=Item.Body.find_first_not_of(' ',nextdelimter);

												pinoffset.Rotate(angle);

												thisnet.push_back(pxy+pinoffset);


												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}


	if(thisnet.size()==PinList.size())
	{
		DSNPen	pen(BLACK);
		pen.SetWidth(0.0004);

		for(unsigned int i=0; i<thisnet.size()-1; i++)
		{
			Tools->DrawLine(pen,thisnet[i],thisnet[i+1]);
		}
	}
	else
	{
		printf("test\n");
	}
}


void DSNFile::Paint()
{
	Element* structure,* boundary,* path;
	Element* placements;
	Element* library,* image;
	Element* networks;

	size_t curfield,nextdelimter;
	std::vector<Pin> DPins;

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
		for(auto const& place:placements->SubElements)
		{
			if(wstringicmp(place.Name,wstring(L"component"))==0)
			{
				library=Root.FindSub(L"library");
				if(library)
				{
					image=library->FindSub(L"image",place.Body);
					if(image)
					{
						DrawComponentOfType(place,*image);
					}
				}
			}
		}
	}
	networks=Root.FindSub(L"network");
	if(networks)
	{
		for(auto const& net:networks->SubElements)
		{
			if(wstringicmp(net.Name,wstring(L"net"))==0)
			{
				for(auto const& pins:net.SubElements)
				{
					curfield=0;
					nextdelimter=0;

					while(nextdelimter!=wstring::npos)
					{
						Pin temp;

						nextdelimter=pins.Body.find_first_of(' ',curfield);
						temp.Body=pins.Body.substr(curfield,nextdelimter-curfield);

						if(pins.Body[curfield]==L'"')
						{
							temp.Device=temp.Body.substr(1,temp.Body.find_first_of('"',1)-1);
							temp.pid=temp.Body.substr(temp.Body.find_first_of('"',1)+2);

						}
						else
						{
							int delimter=temp.Body.find_first_of('-',0);
							temp.Device=temp.Body.substr(0,delimter);

							temp.pid=temp.Body.substr(delimter+1);

						}

						curfield=pins.Body.find_first_not_of(' ',nextdelimter);
						if(!isdigit(temp.pid[0]))
						{
							// conversion failed because the input wasn't a number
							temp.Number=-1;
						}
						else
						{
							temp.Number=stoi(temp.pid);
							// use converted
						}

						DPins.push_back(temp);
					}
					if(DPins.size()>1)
					{
						DrawNets(DPins);
					}
					DPins.clear();
				}
			}
		}
	}
}
