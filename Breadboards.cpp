
#include "BreadBoards.h"

typedef struct
{
	wstring Component;
	Element	Instance;
} thing;

typedef struct
{
	wstring name;
	Element	body;
	int		PinCount;

	vertex ul,lr;

} part;



BreadBoards::BreadBoards(DSNFile& PCBIn,DSNTools& ToolsIn)
{
	Element* placements;
	Element* library;
	Element* networks;
	Element* image;

	size_t curfield,nextdelimter;

	thing item;
	std::vector<thing> Items;

	part Comp;
	std::vector<part> Comps;


	PCB=&PCBIn;
	Tools=&ToolsIn;

	Tools->gfilewidth=100;
	Tools->gxofffromfile=0;
	Tools->gfileheight=100;
	Tools->gyofffromfile=0;

	if(Tools->gfilewidth>Tools->gfileheight)
	{
		Tools->gscalefromfile=Tools->gfilewidth;
	}
	else
	{
		Tools->gscalefromfile=Tools->gfileheight;
	}

	placements=PCBIn.Root.FindSub(L"placement");
	if(placements)
	{
		for(auto const& place:placements->SubElements)
		{
			if(wstringicmp(place.Name,wstring(L"component"))==0)
			{
				item.Component=place.Body;

				for(auto const& type:place.SubElements)
				{
					item.Instance=type;

					Items.push_back(item);
				}
			}
		}
	}

	int TotalPinSideCount=0;

	for(auto const& Item:Items)
	{
		library=PCBIn.Root.FindSub(L"library");
		if(library)
		{
			for(auto const& Image:library->SubElements)
			{
				if(Image.Body==Item.Component)
				{
					Comp.name=Item.Component;
					Comp.body=Item.Instance;

					Comp.PinCount=0;

					Comp.ul.x=DBL_MAX;
					Comp.ul.y=-DBL_MAX;
					Comp.lr.x=-DBL_MAX;
					Comp.lr.y=DBL_MAX;

					for(auto const& ImageEl:Image.SubElements)
					{
						if(wstringicmp(ImageEl.Name,wstring(L"outline"))==0)
						{
						}
						else if(wstringicmp(ImageEl.Name,wstring(L"pin"))==0)
						{
							vertex	pinoffset;

							Comp.PinCount++;

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

							pinoffset.x=PCBIn.ToMM(pinoffset.x);
							pinoffset.y=PCBIn.ToMM(pinoffset.y);

							if(pinoffset.x<Comp.ul.x)
							{
								Comp.ul.x=pinoffset.x;
							}
							if(pinoffset.y>Comp.ul.y)
							{
								Comp.ul.y=pinoffset.y;
							}

							if(pinoffset.x>Comp.lr.x)
							{
								Comp.lr.x=pinoffset.x;
							}
							if(pinoffset.y<Comp.lr.y)
							{
								Comp.lr.y=pinoffset.y;
							}

						}
					}
					TotalPinSideCount+=((Comp.PinCount+1)/2);
					Comps.push_back(Comp);
					break;
				}
			}
		}
	}


	for(int i=0; i<=TotalPinSideCount/60; i++)
	{
		Boards.push_back(*new BreadBoard(Tools));
	}
};

BreadBoards::~BreadBoards()
{

};


void BreadBoards::Paint()
{
	double voffset=0;

	for(auto& Board:Boards)
	{
		Board.Paint();
	}
}
