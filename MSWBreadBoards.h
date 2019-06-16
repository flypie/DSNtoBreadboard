#pragma once

#include "shobjidl_core.h"

#include <gdiplus.h>
using namespace Gdiplus;

#include "MSWBreadBoard.h"
#include "BreadBoards.h"


class MSWBreadBoards : public BreadBoards
{
public:
	MSWBreadBoards(DSNFile& , MSWTools &Tools);


	void Paint(HDC hdc,double Start,double Length);
	void Print(HDC hdc) {};

protected:
	MSWTools *Tools;

private:

};

