#include "BreadBoard.h"


#include "DSNFile.h"
#include "vertex.h"


const	double BreadBoard::BoardWidth=BoardWidthDef;
const	double BreadBoard::BoardHeight=BoardHeightDef;

const	double BreadBoard::BoardColumns=BoardColumnsDef;
const	double BreadBoard::BoardRows=BoardRowsDef;

const	double BreadBoard::CenterGap=CenterGapDef;
const	double BreadBoard::PinPitch=PinPitchDef;



const void BreadBoard::Paint(int num)
{
	DSNPen	Pen(BLACK);
	
	vertex	verticesa,verticesb,wh;

	wh.x=PinPitch*2;
	wh.y=PinPitch;

	verticesa.x=0;
	verticesa.y=0;

	verticesb.x=BoardWidth+2;
	verticesb.y=BoardHeight+2;

	Pen.SetWidth(0.5);

	Tools->DrawRectangle(Pen,verticesa,verticesb);

	verticesa.x=1;
	verticesa.y=1;

	verticesb.x=BoardWidth;
	verticesb.y=BoardHeight;

	Pen.SetWidth(0.5);

	Pen.SetColour(WHITE);

	Tools->DrawRectangle(Pen,verticesa,verticesb);

	Pen.SetWidth(0.25);
	Pen.SetColour(BLUE);

	verticesa.x=0;
	verticesb.x=BoardWidth;

	verticesa.y=verticesb.y=PinPitch;
	Tools->DrawLine(Pen,verticesa,verticesb);

	verticesa.y=verticesb.y=BoardHeight-PinPitch*4;
	Tools->DrawLine(Pen,verticesa,verticesb);

	Pen.SetColour(RED);

	verticesa.y=verticesb.y=BoardHeight-PinPitch;
	Tools->DrawLine(Pen,verticesa,verticesb);

	verticesa.y=verticesb.y=PinPitch*4;
	Tools->DrawLine(Pen,verticesa,verticesb);

	double xoff=(BoardWidth-BoardColumns*PinPitch)/2;
	double ycent=BoardHeight/2;

	Pen.SetColour(BLACK);

	verticesa.y=verticesb.y=ycent;
	Tools->DrawLine(Pen,verticesa,verticesb);

	for(int i=0; i<=BoardColumns; i++)
	{
		verticesa.x=xoff+i*PinPitch;

		for(int j=0; j<5; j++)
		{
			verticesa.y=ycent-CenterGap/2-j*PinPitch;

			Tools->DrawCircle(Pen,verticesa,PinPitch/2);

			verticesa.y=ycent+CenterGap/2+j*PinPitch;

			Tools->DrawCircle(Pen,verticesa,PinPitch/2);

		}

		if(i%5==0)
		{
			verticesa.y=ycent-CenterGap/2-6*PinPitch;

			wstring aStr=std::to_wstring(i);
			
			Tools->DrawString(Pen,verticesa,wh,aStr);

			verticesa.y=ycent+CenterGap/2+6*PinPitch;

			aStr=std::to_wstring((int)(BoardColumns-i));

			Tools->DrawString(Pen,verticesa,wh,aStr);

		}

		if((i)%6)
		{

			for(int j=0; j<2; j++)
			{
				verticesa.y=(j+2)*PinPitch;
				Tools->DrawCircle(Pen,verticesa,PinPitch/2);

				verticesa.y=BoardHeight-(j+2)*PinPitch;
				Tools->DrawCircle(Pen,verticesa,PinPitch/2);

			}
		}
	}
}