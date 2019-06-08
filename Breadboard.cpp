#include "BreadBoard.h"


#include "DSNFile.h"
#include "vertex.h"


const	double BreadBoard::BoardWidth=BoardWidthDef;
const	double BreadBoard::BoardHeight=BoardHeightDef;

const	double BreadBoard::BoardColumns=BoardColumnsDef;
const	double BreadBoard::BoardRows=BoardRowsDef;

const	double BreadBoard::CenterGap=CenterGapDef;
const	double BreadBoard::PinPitch=PinPitchDef;


int BreadBoard::NumberofBoards=0;



const void BreadBoard::Paint()
{
	DSNPen	Pen(BLACK);
	
	vertex	verticesa,verticesb,verticesc,verticesd,wh;
	double xoff=(BoardWidth-BoardColumns*PinPitch)/2;

	double voff=BoardNumber*(BoardHeight+10);
	double ycent=BoardHeight/2+5+voff;


	wh.x=wh.y=PinPitch*2;

	verticesa.x=0;
	verticesa.y=ycent-CenterGap/2-10*PinPitch;

	verticesb.x=BoardWidth;
	verticesb.y=ycent-CenterGap/2-10*PinPitch;

	verticesc.x=BoardWidth;
	verticesc.y=ycent+CenterGap/2+10*PinPitch;

	verticesd.x=0;
	verticesd.y=ycent+CenterGap/2+10*PinPitch;

	Pen.SetWidth(0.5);

	Tools->DrawLine(Pen,verticesa,verticesb);
	Tools->DrawLine(Pen,verticesb,verticesc);
	Tools->DrawLine(Pen,verticesc,verticesd);
	Tools->DrawLine(Pen,verticesd,verticesa);


	Pen.SetWidth(0.25);
	Pen.SetColour(BLUE);

	verticesa.x=0;
	verticesb.x=BoardWidth;

	verticesa.y=verticesb.y=ycent-CenterGap/2-9*PinPitch;
	Tools->DrawLine(Pen,verticesa,verticesb);

	verticesa.y=verticesb.y=ycent+CenterGap/2+6*PinPitch;
	Tools->DrawLine(Pen,verticesa,verticesb);

	Pen.SetColour(RED);

	verticesa.y=verticesb.y=ycent+CenterGap/2+9*PinPitch;
	Tools->DrawLine(Pen,verticesa,verticesb);

	verticesa.y=verticesb.y=ycent-CenterGap/2-6*PinPitch;
	Tools->DrawLine(Pen,verticesa,verticesb);

	verticesa.x=xoff-PinPitch;
	verticesb.x=BoardWidth-xoff+PinPitch;

	wstring aStr;

	for(int j=0; j<5; j++)
	{
		aStr=(wchar_t)((wchar_t)'E'-j);
		verticesa.y=ycent-CenterGap/2-(j)*PinPitch;
		Tools->DrawString(Pen,verticesa,wh,aStr);

		verticesb.y=ycent+CenterGap/2+(j)*PinPitch;
		Tools->DrawString(Pen,verticesb,wh,aStr);


		aStr=(wchar_t)((wchar_t)'F'+j);
		verticesa.y=ycent+CenterGap/2+(j)*PinPitch;
		Tools->DrawString(Pen,verticesa,wh,aStr);

		verticesb.y=ycent-CenterGap/2-(j)*PinPitch;
		Tools->DrawString(Pen,verticesb,wh,aStr);
	}

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
			wstring aStr=std::to_wstring(i);

			verticesa.y=ycent-CenterGap/2-5*PinPitch;
			
			Tools->DrawString(Pen,verticesa,wh,aStr);

			verticesa.y=ycent+CenterGap/2+5*PinPitch;

			aStr=std::to_wstring((int)(BoardColumns-i));

			Tools->DrawString(Pen,verticesa,wh,aStr);

		}

		if((i)%6)
		{

			for(int j=0; j<2; j++)
			{
				verticesa.y=ycent-CenterGap/2-(7+j)*PinPitch;
				Tools->DrawCircle(Pen,verticesa,PinPitch/2);

				verticesa.y=ycent+CenterGap/2+(7+j)*PinPitch;
				Tools->DrawCircle(Pen,verticesa,PinPitch/2);

			}
		}
	}
}