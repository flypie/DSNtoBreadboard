#pragma once

#include "dsntools.h"
#include "DSNFile.h"
#include "BreadBoard.h"




class BreadBoards
{
public:

	~BreadBoards();
	BreadBoards(DSNFile& a, DSNTools &b);

	void Paint(double Start,double Length);
	void Print() {};
	
	double GetTotoalHeightmm()
	{
		return Boards.size()*BreadBoard::BoardHeight;
	};

protected:

	std::vector<BreadBoard> Boards;

	DSNFile* PCB;

private:

	DSNTools *Tools;



};

