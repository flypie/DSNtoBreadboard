#include "MSWDSNFile.h"



MSWDSNFile::MSWDSNFile(wstring a) : DSNFile(a)
{



};


extern void AddToLog(const std::wstring FileName);


void MSWDSNFile::DrawBlob(const wstring &Str)
{
	Pen	pen(Color(255,0,0,255));

	unsigned int curfield=0,nextdelimter;

	nextdelimter=Str.find_first_of(' ',curfield);
	wstring cname=Str.substr(0,nextdelimter);
	curfield=nextdelimter+1;

	nextdelimter=Str.find_first_of(' ',curfield);
	double px=stod(Str.substr(curfield,nextdelimter));
	curfield=nextdelimter+1;

	nextdelimter=Str.find_first_of(' ',curfield);
	double py=stod(Str.substr(curfield,nextdelimter));
	curfield=nextdelimter+1;

	nextdelimter=Str.find_first_of(' ',curfield);
	wstring cforb=Str.substr(curfield,nextdelimter);
	curfield=nextdelimter+1;

	nextdelimter=Str.find_first_of(' ',curfield);
	double angle=stod(Str.substr(curfield,nextdelimter));
	curfield=nextdelimter+1;

	vertex	xy,xy1;

	xy1.x=(px-gxoff)/gscale;
	xy1.y=(py-gyoff)/gscale;


	xy.x=(xy1.x*scalef);
	xy.y=(xy1.y*scalef);


	graphics->DrawLine(&pen,(int)xy.x,(int)xy.y,(int)xy.x+1,(int)xy.y);
	graphics->DrawLine(&pen,(int)xy.x+1,(int)xy.y,(int)xy.x+1,(int)xy.y+1);
	graphics->DrawLine(&pen,(int)xy.x+1,(int)xy.y+1,(int)xy.x,(int)xy.y+1);
	graphics->DrawLine(&pen,(int)xy.x,(int)xy.y+1,(int)xy.x,(int)xy.y);
}


void MSWDSNFile::DrawLine(Pen& pen,double x1,double y1,double x2,double y2)
{
	vertex	xy1,xy2;

	xy1.x=(x1-gxoff)/gscale;
	xy1.y=(y1-gyoff)/gscale;
	xy2.x=(x2-gxoff)/gscale;
	xy2.y=(y2-gyoff)/gscale;

	int ix1,iy1,ix2,iy2;

	ix1=(int)(xy1.x*scalef);
	iy1=(int)(sheight-(xy1.y*scalef));
	ix2=(int)(xy2.x*scalef);
	iy2=(int)(sheight-(xy2.y*scalef));

	graphics->DrawLine(&pen,ix1,iy1,ix2,iy2);
}

void MSWDSNFile::DrawCircle(Pen& pen,double x1,double y1,double radius)
{
	vertex	xy1;
	int ix1,iy1,r2;

	xy1.x=(x1-gxoff)/gscale;
	xy1.y=(y1-gyoff)/gscale;
	double r1=radius/gscale;


	ix1=(int)(xy1.x*scalef);
	iy1=(int)(sheight-(xy1.y*scalef));
	r2=(int)(r1*scalef);


	Brush* brush=new SolidBrush(Color(255,250,0,0));

	graphics->FillEllipse(brush,ix1,iy1,r2,r2);
}


void MSWDSNFile::DrawRectangle(Pen& pen,double x1,double y1,double x2,double y2)
{
	vertex	xy1;
	int ix1,iy1;

	xy1.x=(x1-gxoff)/gscale;
	xy1.y=(y1-gyoff)/gscale;

	ix1=(int)(xy1.x*scalef);
	iy1=(int)(sheight-(xy1.y*scalef));

	int w=(int)((scalef*(x2-x1))/gscale);
	int h=(int)((scalef*(-y2+y1))/gscale);

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

	Brush* brush=new SolidBrush(Color(255,250,0,0));

	graphics->FillRectangle(brush,ix1,iy1,w,h);
}


void MSWDSNFile::DrawPaths(const double x1,const double y1,const Element& path)
{
	unsigned int curfield,nextdelimter,aperture_width;
	std::vector<vertex> vertices;

	vertex	xy;
	Pen	pen(Color(255,0,255,255));

	curfield=0;

	nextdelimter=path.Body.find_first_of(' ',curfield);
	wstring layer_id=path.Body.substr(0,nextdelimter);
	curfield=nextdelimter+1;

	nextdelimter=path.Body.find_first_of(' ',curfield);
	aperture_width=stoi(path.Body.substr(curfield,nextdelimter));
	curfield=nextdelimter+1;


	while(nextdelimter!=wstring::npos)
	{
		nextdelimter=path.Body.find_first_of(' ',curfield);
		xy.x=stod(path.Body.substr(curfield,nextdelimter));
		curfield=nextdelimter+1;

		nextdelimter=path.Body.find_first_of(' ',curfield);
		xy.y=stod(path.Body.substr(curfield,nextdelimter));
		curfield=nextdelimter+1;

		vertices.push_back(xy);
	}

	for(unsigned int i=0; i<vertices.size()-1; i++)
	{
		DrawLine(pen,vertices[i].x+x1,vertices[i].y+y1,vertices[i+1].x+x1,vertices[i+1].y+y1);
	}
}

#define PI 3.14159265

void MSWDSNFile::DrawComponentImage(const Element& Image,double inx1,double iny1,double angle)
{
	unsigned int curfield,nextdelimter;
		
	double	aperture_width;
	vertex	pinoffset,xy;
	std::vector<vertex> vertices;

	Pen	pen(Color(255,0,0,255));

	double cangle=cos(angle*PI/180.0);
	double sangle=sin(angle*PI/180.0);

	for(auto const& ImageEl:Image.SubElements)
	{
		vertices.clear();
		if(wstringicmp(ImageEl.Name,wstring(L"outline"))==0)
		{
			for(auto const& OutlineEl:ImageEl.SubElements)
			{
				if(wstringicmp(OutlineEl.Name,wstring(L"path"))==0)
				{
					curfield=0;

					nextdelimter=OutlineEl.Body.find_first_of(' ',curfield);
					wstring layer_id=OutlineEl.Body.substr(0,nextdelimter);
					curfield=nextdelimter+1;

					nextdelimter=OutlineEl.Body.find_first_of(' ',curfield);
					aperture_width=stod(OutlineEl.Body.substr(curfield,nextdelimter));
					curfield=nextdelimter+1;

					while(nextdelimter!=wstring::npos)
					{
						nextdelimter=OutlineEl.Body.find_first_of(' ',curfield);
						xy.x=stod(OutlineEl.Body.substr(curfield,nextdelimter));
						curfield=nextdelimter+1;

						nextdelimter=OutlineEl.Body.find_first_of(' ',curfield);
						xy.y=stod(OutlineEl.Body.substr(curfield,nextdelimter));
						curfield=nextdelimter+1;

						vertices.push_back(xy);
					}



					for(unsigned int i=0; i<vertices.size()-1; i++)
					{
						double x1=vertices[i].x*cangle-vertices[i].y*sangle;
						double y1=vertices[i].x*sangle+vertices[i].y*cangle;

						double x2=vertices[i+1].x*cangle-vertices[i+1].y*sangle;
						double y2=vertices[i+1].x*sangle+vertices[i+1].y*cangle;

						DrawLine(pen,inx1+x1,iny1+y1,x2+inx1,y2+iny1);
					}
				}
			}
		}
		else if(wstringicmp(ImageEl.Name,wstring(L"pin"))==0)
		{
			curfield=0;

			nextdelimter=ImageEl.Body.find_first_of(' ',curfield);
			wstring name=ImageEl.Body.substr(0,nextdelimter);
			curfield=nextdelimter+1;

			nextdelimter=ImageEl.Body.find_first_of(' ',curfield);
			wstring dimension_unit=ImageEl.Body.substr(curfield,nextdelimter);
			curfield=nextdelimter+1;

			nextdelimter=ImageEl.Body.find_first_of(' ',curfield);
			pinoffset.x=stod(ImageEl.Body.substr(curfield,nextdelimter));
			curfield=nextdelimter+1;

			nextdelimter=ImageEl.Body.find_first_of(' ',curfield);
			pinoffset.y=stod(ImageEl.Body.substr(curfield,nextdelimter));
			curfield=nextdelimter+1;
			

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
								if(wstringicmp(Shape.Name,wstring(L"circle"))==0)
								{
									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									wstring side=Shape.Body.substr(curfield,nextdelimter);
									curfield=nextdelimter+1;

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									double radius=stod(Shape.Body.substr(curfield,nextdelimter));
									curfield=nextdelimter+1;


									DrawCircle(pen,inx1+pinoffset.x-radius/2,iny1+pinoffset.y+radius/2,radius);
								}
								else if(wstringicmp(Shape.Name,wstring(L"rect"))==0)
								{
									vertex xy1,xy2;

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									wstring side=Shape.Body.substr(curfield,nextdelimter);
									curfield=nextdelimter+1;

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									xy1.x=stod(Shape.Body.substr(curfield,nextdelimter));
									curfield=nextdelimter+1;

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									xy1.y=stod(Shape.Body.substr(curfield,nextdelimter));
									curfield=nextdelimter+1;

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									xy2.x=stod(Shape.Body.substr(curfield,nextdelimter));
									curfield=nextdelimter+1;

									nextdelimter=Shape.Body.find_first_of(' ',curfield);
									xy2.y=stod(Shape.Body.substr(curfield,nextdelimter));
									curfield=nextdelimter+1;

									double x1=pinoffset.x*cangle-pinoffset.y*sangle;
									double y1=pinoffset.x*sangle+pinoffset.y*cangle;

									double x2=xy1.x*cangle-xy1.y*sangle;
									double y2=xy1.x*sangle+xy1.y*cangle;

									double x3=xy2.x*cangle-xy2.y*sangle;
									double y3=xy2.x*sangle+xy2.y*cangle;

									DrawRectangle(pen,inx1+x1+x2,iny1+y1+y2,inx1+x1+x3,iny1+y1+y3);
								}
								else if(wstringicmp(Shape.Name,wstring(L"path"))==0)
								{
									DrawPaths(inx1,iny1,Shape);
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
//					DrawComponentOfType(placement,*image);
				}
			}
		}
		else if(wstringicmp(ImageEl.Name,wstring(L"keepout"))==0)
		{
		}
		else
		{
			AddToLog(L"error "+ImageEl.Name);
		}
	}
}

void MSWDSNFile::DrawComponentOfType(const Element &placement,const Element &Image)
{
	for(auto const& place:placement.SubElements)
	{
		unsigned int curfield=0,nextdelimter;

		nextdelimter=place.Body.find_first_of(' ',curfield);
		wstring cname=place.Body.substr(0,nextdelimter);
		curfield=nextdelimter+1;

		nextdelimter=place.Body.find_first_of(' ',curfield);
		double px=stod(place.Body.substr(curfield,nextdelimter));
		curfield=nextdelimter+1;

		nextdelimter=place.Body.find_first_of(' ',curfield);
		double py=stod(place.Body.substr(curfield,nextdelimter));
		curfield=nextdelimter+1;

		nextdelimter=place.Body.find_first_of(' ',curfield);
		wstring cforb=place.Body.substr(curfield,nextdelimter);
		curfield=nextdelimter+1;

		nextdelimter=place.Body.find_first_of(' ',curfield);
		double angle=stod(place.Body.substr(curfield,nextdelimter));
		curfield=nextdelimter+1;

		DrawComponentImage(Image,px,py,angle);
	}
}

void MSWDSNFile::DrawPCBOutline(const Element &path)
{
	unsigned int curfield,nextdelimter,aperture_width;
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
	aperture_width=stoi(path.Body.substr(curfield,nextdelimter));
	curfield=nextdelimter+1;


	while(nextdelimter!=wstring::npos)
	{
		nextdelimter=path.Body.find_first_of(' ',curfield);
		xy.x=stod(path.Body.substr(curfield,nextdelimter));
		curfield=nextdelimter+1;

		nextdelimter=path.Body.find_first_of(' ',curfield);
		xy.y=stod(path.Body.substr(curfield,nextdelimter));
		curfield=nextdelimter+1;

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
		DrawLine(pen,vertices[i].x,vertices[i].y,vertices[i+1].x,vertices[i+1].y);
	}
}

void MSWDSNFile::Paint(HDC hdc)
{
	Pen	pen(Color(255,0,0,255));


	Element *structure,*boundary,*path;
	Element *placements;
	Element* library,*image;

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

	delete graphics;
}
