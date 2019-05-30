#include "MSWDSNFile.h"

MSWDSNFile::MSWDSNFile(wstring a) : DSNFile(a)
{
	scalef=0;
	sheight=0;
	swidth=0;
	gxoff=0;
	gyoff=0;
	gscale=0;
	graphics=0;
};

extern void AddToLog(const std::wstring FileName);

void MSWDSNFile::DrawLine(Pen& pen,vertex &a,vertex &b)
{
	vertex	xy1,xy2;

	xy1.x=(a.x-gxoff)/gscale;
	xy1.y=(a.y-gyoff)/gscale;
	xy2.x=(b.x-gxoff)/gscale;
	xy2.y=(b.y-gyoff)/gscale;

	int ix1,iy1,ix2,iy2;

	ix1=(int)(xy1.x*scalef);
	iy1=(int)(sheight-(xy1.y*scalef));
	ix2=(int)(xy2.x*scalef);
	iy2=(int)(sheight-(xy2.y*scalef));

	graphics->DrawLine(&pen,ix1,iy1,ix2,iy2);
}

void MSWDSNFile::DrawCircle(Pen& pen,vertex& a,double radius)
{
	vertex	xy1;
	int ix1,iy1,r2;

	xy1.x=(a.x-gxoff)/gscale;
	xy1.y=(a.y-gyoff)/gscale;
	double r1=radius/gscale;


	ix1=(int)(xy1.x*scalef);
	iy1=(int)(sheight-(xy1.y*scalef));
	r2=(int)(r1*scalef);

	Color PColour;
	pen.GetColor(&PColour);

	Brush* brush=new SolidBrush(PColour);

	graphics->FillEllipse(brush,ix1,iy1,r2,r2);
}


void MSWDSNFile::DrawRectangle(Pen& pen,vertex& a,vertex& b)
{
	vertex	xy1;
	int ix1,iy1;

	xy1.x=(a.x-gxoff)/gscale;
	xy1.y=(a.y-gyoff)/gscale;

	ix1=(int)(xy1.x*scalef);
	iy1=(int)(sheight-(xy1.y*scalef));

	int w=(int)((scalef*(b.x-a.x))/gscale);
	int h=(int)((scalef*(-b.y+a.y))/gscale);

	if(w<0)
	{
		ix1+=w;
		w=-w;
	}

	if(h<0)
	{
		iy1+=h;
		h=-h;
	}

	Color PColour;
	pen.GetColor(&PColour);

	Brush* brush=new SolidBrush(PColour);

	graphics->FillRectangle(brush,ix1,iy1,w,h);
}


void MSWDSNFile::DrawComponentImage(const Element& Image,vertex inxy,double angle)
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
			Pen	pen(Color(255,0,0,255));
			pen.SetStartCap(LineCapRound);
			pen.SetEndCap(LineCapRound);

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

					pen.SetWidth((REAL)((linewidth/gscale)*scalef));

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

			Pen	pen(Color(255,0,128,255));
			pen.SetStartCap(LineCapRound);
			pen.SetEndCap(LineCapRound);

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

									temp.x=inxy.x+temp1.x-radius/2;
									temp.y=inxy.y+temp1.y+radius/2;

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

									pen.SetWidth((REAL)((linewidth/gscale)* scalef));

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
			Pen	pen(Color(255,0,255,255));
			pen.SetStartCap(LineCapRound);
			pen.SetEndCap(LineCapRound);
			
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

					temp.x=inxy.x+offset.x-radius/2;
					temp.y=inxy.y+offset.y+radius/2;

					DrawCircle(pen,temp,radius);
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

void MSWDSNFile::DrawComponentOfType(const Element &placement,const Element &Image)
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

void MSWDSNFile::DrawPCBOutline(const Element &path)
{
	size_t curfield,nextdelimter,aperture_width;
	std::vector<vertex> vertices;
	double xscale,yscale;

	vertex	xy;
	Pen	pen(Color(255,0,0,255));
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

	xscale=lr.x-ul.x;
	gxoff=ul.x;
	yscale=(ul.y-lr.y);
	gyoff=lr.y;

	if(xscale>yscale)
	{
		gscale=yscale;
	}
	else
	{
		gscale=xscale;
	}


	for(unsigned int i=0; i<vertices.size()-1; i++)
	{
		DrawLine(pen,vertices[i],vertices[i+1]);
	}
}

void MSWDSNFile::Paint(HDC hdc)
{
	Pen	pen(Color(255,0,0,255));


	Element *structure,*boundary,*path;
	Element *placements;
	Element* library,*image;

	double AngleComp;

	RECT    rcCli;

	graphics = new Graphics(hdc);

	GetClientRect(WindowFromDC(hdc),&rcCli);

	swidth=rcCli.right-rcCli.left-1;
	sheight=rcCli.bottom-rcCli.top-1;

	scalef=sheight;

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
				if(placement.Body==L"ACE5SMT:78XXL")
				{
					AngleComp=0;
				}

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

	delete graphics;
}
