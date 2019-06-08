#pragma once


#include <vector>

#define	CM(a)	(a*10)
#define	MM(a)	(a)

#define BoardWidthDef CM(16.51)
#define BoardHeightDef CM(5.6)

#define BoardColumnsDef 60
#define	BoardRowsDef 10

#define CenterGapDef MM(7.62)
#define	PinPitchDef MM(2.54)



#include "DSNTools.h"

class BreadBoard
{
public:

	BreadBoard(DSNTools* ToolsIn) 
	{ 
		BoardNumber=NumberofBoards;
		NumberofBoards++;
		Tools=ToolsIn;
	};


	const void Paint();

//
	
public:
	static const double BoardWidth;
	static const double BoardHeight;

	static const double BoardColumns;
	static const double BoardRows;

	static const double CenterGap;
	static const double PinPitch;

protected:
	DSNTools* Tools;

	static	int NumberofBoards;
	int BoardNumber;
};


