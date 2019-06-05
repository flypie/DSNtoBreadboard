
#include "BreadBoards.h"

typedef struct
{
	wstring Component;
	Element	Instance;
} thing;




BreadBoards::BreadBoards(DSNFile& PCBIn, DSNTools &ToolsIn)
{
	Element* structure,* boundary,* path;
	Element* placements;
	Element* library,* image;
	Element* networks;

	thing item;

	std::vector<thing> Items;
//	std::vector<Element> Types;

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
					item.Instance=place;

					Items.push_back(item);
				}
			}
		}
	}


	for(auto const& Item:Items)
	{
		library=PCBIn.Root.FindSub(L"library");
		if(library)
		{
//			image=library->FindSub(L"image",Item.Body);
//			if(image)
			{
//				DrawComponentOfType(place,*image);
			}
		}
	}


};

BreadBoards::~BreadBoards()
{

};


void BreadBoards::Paint()
{
	double voffset=0;

	for(auto & Board:Boards)
	{
		Board.Paint(0);
	}
}
