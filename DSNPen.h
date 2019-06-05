

#pragma once

typedef enum
{
	BLACK,RED,GREEN,BLUE,YELLOW,CYAN,MAGENTA,WHITE
} DSNCOLOUR;





class DSNPen
{
public:
	DSNPen(DSNCOLOUR A)
	{
		Colour=A;
		Width=1;
	};

	void SetWidth(double A)
	{
		Width=A;
	};

	void GetWidth(double& A)
	{
		A=Width;
	};

	void GetColour(DSNCOLOUR& A)
	{
		A=Colour;
	};

	void SetColour(DSNCOLOUR A)
	{
		Colour=A;
	};

private:
	DSNCOLOUR Colour;
	double	Width;


};
