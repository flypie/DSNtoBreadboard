#pragma once
class Entity
{
public:
	Entity()
	{
		gfileheight=0;
		gfilewidth=0;
		gxofffromfile=0;
		gyofffromfile=0;
		conversion=1;
	};

	double	Getxofffromfile() { return gxofffromfile; };
	double	Getyofffromfile() { return gyofffromfile; };
	double	Getfilewidth() { return gfilewidth; };
	double	Getfileheight() { return gfileheight; };

	double GetConversion() { return conversion; };

	double ToMM(double dimension) { return dimension/conversion; };

protected :

	double conversion;

	double	gxofffromfile,gyofffromfile;
	double	gfilewidth,gfileheight;
};

